// Copyright (c) 2014 Adafruit Industries
// Author: Tony DiCola

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <stdbool.h>
#include <stdlib.h>

#include <Sensor/pi_2_dht_read.hpp>
#include <Sensor/pi_2_mmio.hpp>

// This is the only processor specific magic value, the maximum amount of time
// to
// spin in a loop before bailing out and considering the read a timeout.  This
// should
// be a high value, but if you're running on a much faster platform than a
// Raspberry
// Pi or Beaglebone Black then it might need to be increased.
static constexpr uint32_t DHT_MAXCOUNT = 32000;

// Number of bit pulses to expect from the DHT.  Note that this is 41 because
// the first pulse is a constant 50 microsecond pulse, with 40 pulses to
// represent
// the data afterwards.
static constexpr uint32_t DHT_PULSES = 41;

SensorResult pi_2_dht_read(SensorType type, size_t pin)
{
    SensorResult sensorResult {DhtState::UNKNOWN, 0.0, 0.0};

    // Initialize GPIO library.
    if (pi_2_mmio_init() != MmioState::SUCCESS) {
        sensorResult.state = DhtState::GPIO_ERROR;
        return sensorResult;
    }

    // Store the count that each DHT bit pulse is low and high.
    // Make sure array is initialized to start at zero.
    size_t pulseCounts[DHT_PULSES * 2] = {0};

    // Set pin to output.
    pi_2_mmio_set_output(pin);

    // Bump up process priority and change scheduler to try to try to make process
    // more 'real time'.
    set_max_priority();

    // Set pin high for ~500 milliseconds.
    pi_2_mmio_set_high(pin);
    sleep_milliseconds(500);

    // The next calls are timing critical and care should be taken
    // to ensure no unnecssary work is done below.

    // Set pin low for ~20 milliseconds.
    pi_2_mmio_set_low(pin);
    busy_wait_milliseconds(20);

    // Set pin at input.
    pi_2_mmio_set_input(pin);
    // Need a very short delay before reading pins or else value is sometimes
    // still low.
    for (volatile uint32_t i = 0; i < 50; ++i) {
    }

    // Wait for DHT to pull pin low.
    uint32_t count = 0;
    while (pi_2_mmio_input(pin)) {
        if (++count >= DHT_MAXCOUNT) {
            // Timeout waiting for response.
            set_default_priority();
            sensorResult.state = DhtState::TIMEOUT_ERROR;
            return sensorResult;
        }
    }

    // Record pulse widths for the expected result bits.
    for (uint32_t i = 0; i < DHT_PULSES * 2; i += 2) {
        // Count how long pin is low and store in pulseCounts[i]
        while (!pi_2_mmio_input(pin)) {
            if (++pulseCounts[i] >= DHT_MAXCOUNT) {
                // Timeout waiting for response.
                set_default_priority();
                sensorResult.state = DhtState::TIMEOUT_ERROR;
                return sensorResult;
            }
        }
        // Count how long pin is high and store in pulseCounts[i+1]
        while (pi_2_mmio_input(pin)) {
            if (++pulseCounts[i + 1] >= DHT_MAXCOUNT) {
                // Timeout waiting for response.
                set_default_priority();
                sensorResult.state = DhtState::TIMEOUT_ERROR;
                return sensorResult;
            }
        }
    }

    // Done with timing critical code, now interpret the results.

    // Drop back to normal priority.
    set_default_priority();

    // Compute the average low pulse width to use as a 50 microsecond reference
    // threshold.
    // Ignore the first two readings because they are a constant 80 microsecond
    // pulse.
    size_t threshold = 0;
    for (uint32_t i = 2; i < DHT_PULSES * 2; i += 2) {
        threshold += pulseCounts[i];
    }
    threshold /= DHT_PULSES - 1;

    // Interpret each high pulse as a 0 or 1 by comparing it to the 50us
    // reference.
    // If the count is less than 50us it must be a ~28us 0 pulse, and if it's
    // higher
    // then it must be a ~70us 1 pulse.
    uint8_t data[5] = {0};
    for (uint32_t i = 3; i < DHT_PULSES * 2; i += 2) {
        uint32_t index = (i - 3) / 16;
        data[index] <<= 1;
        if (pulseCounts[i] >= threshold) {
            // One bit for long pulse.
            data[index] |= 1;
        }
        // Else zero bit for short pulse.
    }

    // Useful debug info:
    // printf("Data: 0x%x 0x%x 0x%x 0x%x 0x%x\n", data[0], data[1], data[2],
    // data[3], data[4]);

    // Verify checksum of received data.
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        switch (type) {
            case SensorType::DHT11:
            {
                // Get humidity and temp for DHT11 sensor.
                sensorResult.humidity = static_cast<double>(data[0]);
                sensorResult.temperature = static_cast<double>(data[2]);
                break;
            }
            case SensorType::DHT22:
            case SensorType::AM2302:
            {
                // Calculate humidity and temp for DHT22 sensor.
                sensorResult.humidity = static_cast<double>(data[0] * 256 + data[1]) / 10.0;
                sensorResult.temperature = static_cast<double>((data[2] & 0x7F) * 256 + data[3]) / 10.0;
                if (data[2] & 0x80) {
                    sensorResult.temperature *= -1.0f;
                }
                break;
            }
        }

        sensorResult.state = DhtState::SUCCESS;
        return sensorResult;
    } else {
        sensorResult.state = DhtState::CHECKSUM_ERROR;
        return sensorResult;
    }
}
