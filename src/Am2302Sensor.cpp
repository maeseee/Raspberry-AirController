#include <Am2302Sensor.hpp>
#include <GpioConstants.hpp>

#include <algorithm> // std::fill
#include <iostream>
#include <unistd.h> // sleep

namespace sensor {

static constexpr size_t MAX_TIMINGS = 85;
static constexpr size_t BYTE_SIZE = 8;
static constexpr size_t ONE_WIRE_COM_TIMEOUT_US =
    150; // In real signals last for 80us

static const size_t CALL_INTERVALL = 60; // call upcate intervall for thread

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr &sensor) : m_sensor(sensor) {
  m_thread = std::thread(&Am2302Sensor::threadFn, this);
  m_sensor->setDirection(gpio::Direction::IN);
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

int Am2302Sensor::readBit() const {
  for (size_t stopCounter = 0; gpio::Value::LOW == m_sensor->getValue();
       ++stopCounter) {
    if (stopCounter < ONE_WIRE_COM_TIMEOUT_US) {
      // sensor will 80 us pulls low
      std::cout << "sensor will not pull up" << std::endl;
      return -1;
    }
    usleep(1);
  }

  size_t counter = 0;
  while (gpio::Value::HIGH == m_sensor->getValue()) {
    ++counter;
    if (counter < 100) {
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

int Am2302Sensor::readByte() const {
  int byte = 0;
  for (size_t i = 0; i < 8; ++i) {
    int bit = readBit();
    if (bit < 0) {
      return -1;
    }
    byte <<= 1;
    byte |= bit;
  }

  byte &= 0xFF; // delete unused bits
  return byte;
}

void Am2302Sensor::recall() {
  std::cout << "--read am2302 sensor" << std::endl;
  gpio::Value laststate = gpio::Value::HIGH;

  std::fill(m_buffer.begin(), m_buffer.end(), 0);

  m_sensor->setDirection(gpio::Direction::OUT);
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(10000); // just wait a little (the normal position is high trough a
                 // pull up resistor)
  m_sensor->setValue(gpio::Value::LOW);
  usleep(1500); // min low 1000 us
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(25); // high for 20 - 40 us

  // prepare to read the pin
  m_sensor->setDirection(gpio::Direction::IN);
  if (readBit() == -1) {
    // sensor will change after 80 us
    std::cout << "Am2302 start sequence missing" << std::endl;
    return;
  }

  uint8_t sum = 0;
  for (size_t byte = 0; byte < 4; ++byte) {
    int newByte = readByte();
    m_buffer[byte] = newByte;
    sum += newByte;
    if (newByte < 0) {
      std::cout << "Invalid byte received" << std::endl;
      return;
    }
  }

  int checksum = readByte();
  if (checksum < 0) {
    std::cout << "Invalid checksum received" << std::endl;
    return;
  }
  if (static_cast<uint8_t>(checksum) != sum) {
    std::cout << "Wrong checksum" << std::endl;
    return;
  }

  uint16_t humidity = (m_buffer[0] << 8) | m_buffer[1];
  uint16_t temperature = (m_buffer[3] << 8) | m_buffer[4];
  m_humidity = humidity / 10;
  m_temperature = temperature / 10;
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
