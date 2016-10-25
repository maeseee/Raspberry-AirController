#ifndef SENSOR_AM2302_H
#define SENSOR_AM2302_H

#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

namespace sensor {

class Am2302Sensor : public sensor::ISensor, public threading::Threading {
public:
  Am2302Sensor(const gpio::IGpioPtr &sensor);

  SensorData getData() const override;

  void recall() override;

private:
  int waitForBit(const gpio::Value val) const;
  int readBit() const;
  int readByte() const;

  float m_temperature; // [°C]
  float m_humidity;    // [%]

  gpio::IGpioPtr m_sensor;
};
}
#endif
