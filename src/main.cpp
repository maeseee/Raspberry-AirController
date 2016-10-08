#include <iostream>
#include <unistd.h>

#include <Am2302Sensor.hpp>
#include <Enabler.hpp>
#include <Gpio.hpp>
#include <Humidity.hpp>
#include <SensorSim.hpp>
#include <TimeTrigger.hpp>
#include <WeatherStation.hpp>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * 60 * 60;
static constexpr size_t END_NIGHT_CONDITION = 7 * 60 * 60;
static constexpr size_t SAFETY_CONDITION = 30 * 60;

int main() {

  // initialize actors
  gpio::IGpioPtr gpioTester =
      std::make_shared<gpio::Gpio>(gpio::Function::Main, gpio::Direction::OUT, gpio::Value::LOW);
  sleep(1);
  gpioTester->setValue(gpio::Value::HIGH);
  sleep(1);
  gpioTester->setValue(gpio::Value::LOW);
  sleep(1);

  // initialize sensors
  sensor::ISensorPtr weatherStation = std::make_shared<sensor::SensorSim>();

  gpio::IGpioPtr am2302 = std::make_shared<gpio::Gpio>(gpio::Function::Am2302);
  sensor::ISensorPtr measuredValues =
      std::make_shared<sensor::Am2302Sensor>(am2302);

  sleep(2); // wait until sensors have results

  // setup timer
  gpio::IGpioPtr timer = std::make_shared<gpio::Gpio>(gpio::Function::Time, gpio::Direction::OUT, gpio::Value::LOW);
  time_trigger::TimeTrigger timeTrigger(
      START_NIGHT_CONDITION + SAFETY_CONDITION,
      END_NIGHT_CONDITION - SAFETY_CONDITION, timer);

  // setup roti
  gpio::IGpioPtr roti = std::make_shared<gpio::Gpio>(gpio::Function::Roti, gpio::Direction::OUT, gpio::Value::LOW);
  humidity::HumidityController humidityController(measuredValues,
                                                  weatherStation, timer);

  sleep(10);

  return 0;
}
