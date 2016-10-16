#include <iostream>
#include <unistd.h>

#include <Am2302Sensor.hpp>
#include <Gpio.hpp>
#include <GpioCollector.hpp>
#include <NightAir.hpp>
#include <RotiController.hpp>
#include <SensorSim.hpp>
#include <TempController.hpp>
#include <TimeTrigger.hpp>
#include <WeatherStation.hpp>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * 60 * 60;
static constexpr size_t END_NIGHT_CONDITION = 7 * 60 * 60;
static constexpr size_t SAFETY_CONDITION = 30 * 60;

int main() {

  // initialize sensor for outdoor values
  sensor::ISensorPtr weatherStation = std::make_shared<sensor::SensorSim>();

  // initialize sensor for outdoor values
  // initialize gpio for one-wire-bus
  gpio::IGpioPtr am2302 = std::make_shared<gpio::Gpio>(gpio::Function::Am2302);
  // create sensor
  sensor::ISensorPtr measuredValues =
      std::make_shared<sensor::Am2302Sensor>(am2302);

  // just wait a little to make sure all sensor threads received initial values
  sleep(2);

  // setup timer
  gpio::IGpioPtr timer = std::make_shared<gpio::Gpio>(
      gpio::Function::NightTime, gpio::Direction::OUT, gpio::Value::LOW);
  time_trigger::TimeTrigger timeTrigger(
      START_NIGHT_CONDITION + SAFETY_CONDITION,
      END_NIGHT_CONDITION - SAFETY_CONDITION, timer);

  // setup collector and gpio for main system
  gpio::IGpioPtr mainSystem = std::make_shared<gpio::Gpio>(
      gpio::Function::Main, gpio::Direction::OUT, gpio::Value::LOW);
  gpio::GpioCollectorPtr collector =
      std::make_shared<gpio::GpioCollector>(mainSystem);

  // setup roti
  gpio::IGpioPtr roti = std::make_shared<gpio::Gpio>(
      gpio::Function::Roti, gpio::Direction::OUT, gpio::Value::LOW);
  roti_controller::RotiController humidityController(
      measuredValues, weatherStation, roti);

  // setup night air
  night_air::NightAir nightAir(collector);

  // setup temperature
  temperature::TemperatureController temperature(collector);

  sleep(10);

  return 0;
}
