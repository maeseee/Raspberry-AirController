#include <Am2302Sensor.hpp>
#include <GpioConstants.hpp>

#include <algorithm> // std::fill
#include <cassert>
#include <iostream>
#include <unistd.h> // sleep

namespace sensor {

static constexpr size_t MAX_TIMINGS = 85;
static constexpr size_t BYTE_SIZE = 8;
static constexpr size_t ONE_WIRE_COM_TIMEOUT_US =
    150; // In real signals last for 80us

static const size_t CALL_INTERVALL = 60; // call upcate intervall for thread
static constexpr size_t BUFFER_SIZE = 4;

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

int Am2302Sensor::waitForBit(const gpio::Value val) const {
  int counter = 0;
  while (val != m_sensor->getValue()) {
    ++counter;
    if (counter > ONE_WIRE_COM_TIMEOUT_US) {
      std::cout << "sensor changes not to " << static_cast<size_t>(val)
                << std::endl;
      return -1;
    }
    usleep(1);
  }
  return counter;
}

int Am2302Sensor::readBit() const {
  assert(gpio::Value::LOW == m_sensor->getValue());

  if (waitForBit(gpio::Value::HIGH)) {
    std::cout << "not changing to high" << std::endl;
    return -1;
  }

  int counter = waitForBit(gpio::Value::LOW);

  if (counter < 0) {
    // invalid Bit
    std::cout << "invalid bit read" << std::endl;
    return -1;
  } else if (counter < 50) {
    // 0 for 27 us
    return 0;
  } else {
    // 1 for 70 us
    return 1;
  }
}

int Am2302Sensor::readByte() const {
  assert(gpio::Value::LOW == m_sensor->getValue());

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

  uint8_t buffer[BUFFER_SIZE] = {0, 0, 0, 0};

  m_sensor->setDirection(gpio::Direction::OUT);
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(10000); // just wait a little (the normal position is high trough a
                 // pull up resistor)
  m_sensor->setValue(gpio::Value::LOW);
  usleep(2000); // min low 1000 us
  m_sensor->setValue(gpio::Value::HIGH);
  usleep(1); // high for 20 - 40 us

  // prepare to read the pin
  m_sensor->setDirection(gpio::Direction::IN);

  if (waitForBit(gpio::Value::LOW) == -1) {
    // sensor will change after 80 us
    std::cout << "Am2302 start sequence missing" << std::endl;
    return;
  }

  std::cout << "Sensor put the signal to low" << std::endl;
  uint8_t sum = 0;
  for (size_t byte = 0; byte < BUFFER_SIZE; ++byte) {
    int newByte = readByte();
    buffer[byte] = newByte;
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

  uint16_t humidity = (buffer[0] << BYTE_SIZE) | buffer[1];
  uint16_t temperature = (buffer[3] << BYTE_SIZE) | buffer[4];
  m_humidity = humidity / 10;
  m_temperature = temperature / 10;
}

void Am2302Sensor::threadFn() {
  sleep(1);
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
