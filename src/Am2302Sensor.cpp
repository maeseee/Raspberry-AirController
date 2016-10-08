#include <Am2302Sensor.hpp>
#include <GpioConstants.hpp>

#include <algorithm> // std::fill
#include <iostream>
#include <unistd.h> // sleep

namespace sensor {

static constexpr size_t MAX_TIMINGS = 85;
static constexpr size_t BYTE_SIZE = 8;

static const size_t CALL_INTERVALL = 60; // call upcate intervall for thread

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr &sensor) : m_sensor(sensor) {
  m_thread = std::thread(&Am2302Sensor::threadFn, this);
}

Am2302Sensor::~Am2302Sensor() {
  m_stopThread = true;
  m_thread.join();
}

SensorData Am2302Sensor::getData() const {
  return SensorData{m_temperature, m_humidity};
}

bool Am2302Sensor::isChecksumValid() const {
  uint8_t sum = m_buffer[BUFFER_SIZE - 1];
  for (size_t index = 0; index < BUFFER_SIZE - 1; ++index) {
    sum -= m_buffer[index];
  }
  std::cout << "isChecksumValid = " << (0 == sum) << std::endl;
  return 0 == sum;
}

int Am2302Sensor::readBit(size_t timeout_us) const {
  for (size_t stopCounter = 0; gpio::Value::LOW == m_sensor->getValue();
       ++stopCounter) {
    if (stopCounter < timeout_us) {
      // sensor will 80 us pulls low
      std::cout << "sensor will not pull up" << std::endl;
      return -1;
    }
    usleep(1);
  }

  size_t counter = 0;
  while (gpio::Value::HIGH == m_sensor->getValue()) {
    ++stopCounter;
    if (stopCounter < 100) {
      // sensor will 80 us pulls high
      std::cout << "sensor will not pull up" << std::endl;
      return -1;
    }
    usleep(1);
  }

  if (counter < 50) {
    // 0 for 27 us
    return 0;
  } else {
    // 1 for 70 us
    return 1;
  }
}

int Am2302Sensor::readByte(size_t timeout_us) const
{

}

void Am2302Sensor::recall() {
  std::cout << "--read am2302 sensor" << std::endl;
  gpio::Value laststate = gpio::Value::HIGH;

  std::fill(m_buffer.begin(), m_buffer.end(), 0);
  // m_inputData = 0;

  m_sensor->setDirection(gpio::Direction::OUT);
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(10000); // just wait a little
  m_sensor->setValue(gpio::Value::LOW);
  usleep(1500); // min low 1000 us
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(25); // high for 20 - 40 us

  // prepare to read the pin
  m_sensor->setDirection(gpio::Direction::IN);
  if(readBit(100) == -1) {
      // sensor will change after 80 us
      std::cout << "Am2302 start sequence missing" << std::endl;
      return;
  }

  // detect change and read data
  uint8_t counter = 0;
  size_t bitNumber = 0;
  for (int i = 0; i < MAX_TIMINGS; i++) {
    counter = 0;
    while (m_sensor->getValue() == laststate) {
      ++counter;
      usleep(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = m_sensor->getValue();

    if (counter == 255) {
      break;
    }

    // ignore first 3 transitions
    if ((i >= 4) && (i % 2 == 0)) {
      // shove each bit into the storage bytes
      m_buffer[bitNumber / BYTE_SIZE] <<= 1;
      if (counter > 16) {
        // Set bit
        m_buffer[bitNumber / BYTE_SIZE] |= 1;
      }
      bitNumber++;
    }
    std::cout << "--read am2302 sensor end" << std::endl;
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((bitNumber >= (BUFFER_SIZE * BYTE_SIZE)) && isChecksumValid()) {
    float t, h;
    h = (float)m_buffer[0] * 256 + (float)m_buffer[1];
    h /= 10.0;
    t = (float)(m_buffer[2] & 0x7F) * 256 + (float)m_buffer[3];
    t /= 10.0;
    if ((m_buffer[2] & 0x80) != 0)
      t *= -1;

    printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t);
    // return 1;
  } else {
    printf("Data not good, skip\n");
    std::cout << bitNumber << std::endl;
    // return 0;
  }
}

void Am2302Sensor::threadFn() {
  while (!m_stopThread) {
    static int timeCounter = 0;
    if (0 == timeCounter) {
      recall();
    }
    ++timeCounter;
    timeCounter %= CALL_INTERVALL;
    sleep(1);
  }
}
}
