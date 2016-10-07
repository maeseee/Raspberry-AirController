#include <iostream>
#include <unistd.h>

#include <Am2302Sensor.hpp>
#include <Enabler.hpp>
#include <GpioManager.hpp>
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
  gpio::GpioManager gpioManager;

  // initialize sensors
  sensor::ISensorPtr weatherStation =
      std::make_shared<sensor::WeatherStation>();

  sensor::ISensorPtr measuredValues = std::make_shared<sensor::Am2302Sensor>(
      gpioManager.getGpio(gpio::Function::Am2302));

  sleep(2); // wait until sensors have results

  // setup timer
  time_trigger::TimeTrigger timeTrigger(
      START_NIGHT_CONDITION + SAFETY_CONDITION,
      END_NIGHT_CONDITION - SAFETY_CONDITION,
      gpioManager.getGpio(gpio::Function::Roti));

  // setup roti
  humidity::HumidityController humidityController(
      measuredValues, weatherStation,
      gpioManager.getGpio(gpio::Function::Roti));

  sleep(10);

  return 0;
}
