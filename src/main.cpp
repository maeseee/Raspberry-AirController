#include <iostream>
#include <unistd.h>

#include <GpioManager.hpp>
#include <TimeTrigger.hpp>
#include <WeatherStation.hpp>

int main() {
  gpio::GpioManager gpioManager;

  using std::placeholders::_1;
  gpio::GpioSetter fnGpioSetter = std::bind(
      &gpio::GpioManager::setValue, gpioManager, gpio::Function::Time, _1);
  time_trigger::TimeTrigger timeTrigger(22 * 60 * 60 + 30 * 60,
                                        5 * 60 * 60 + 30 * 60, fnGpioSetter);

  weather_station::WeatherStation weatherStation;
  sleep(1); // wait until data is available
  weather_station::WeatherData data = weatherStation.getData();
  std::cout << "Temperature: " << data.temperature
            << "\t Humadity: " << data.humidity << std::endl;

  gpioManager.setValue(gpio::Function::Main, gpio::Value::ON);
  gpioManager.setValue(gpio::Function::Roti, gpio::Value::ON);
  gpioManager.setValue(gpio::Function::Time, gpio::Value::ON);

  sleep(1);

  gpioManager.setValue(gpio::Function::Main, gpio::Value::OFF);
  gpioManager.setValue(gpio::Function::Roti, gpio::Value::OFF);
  gpioManager.setValue(gpio::Function::Time, gpio::Value::OFF);

  sleep(5);

  return 0;
}
