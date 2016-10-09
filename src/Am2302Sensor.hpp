#ifndef SENSOR_AM2302_H
#define SENSOR_AM2302_H

#include <IGpio.hpp>
#include <ISensor.hpp>

#include <array>
#include <thread>

namespace sensor {

class Am2302Sensor : public sensor::ISensor {
public:
  Am2302Sensor(const gpio::IGpioPtr &sensor);
  ~Am2302Sensor();

  SensorData getData() const override;

private:
  int waitForBit(const gpio::Value val) const;
  int readBit() const;
  int readByte() const;

  void recall();
  void threadFn();

  float m_temperature; // [°C]
  float m_humidity;    // [%]

  gpio::IGpioPtr m_sensor;

  std::thread m_thread;
  bool m_stopThread{false};

  static constexpr size_t BUFFER_SIZE = 5;
  std::array<uint8_t, BUFFER_SIZE> m_buffer;
};
}
#endif
