#include <iostream>
#include <unistd.h>

#include <GpioManager.hpp>
#include <WeatherStation.hpp>

int main() {
  weather_station::WeatherStation weatherStation;
  sleep(1); // wait until data is available
  weather_station::WeatherData data = weatherStation.getData();
  std::cout << "Temperature: " << data.temperature
            << "\t Humadity: " << data.humidity << std::endl;

  gpio::GpioManager gpioManager;
  gpioManager.setValue(gpio::Function::Main, gpio::Value::ON);
  gpioManager.setValue(gpio::Function::Roti, gpio::Value::ON);
  gpioManager.setValue(gpio::Function::Time, gpio::Value::ON);

  sleep(1);

  gpioManager.setValue(gpio::Function::Main, gpio::Value::OFF);
  gpioManager.setValue(gpio::Function::Roti, gpio::Value::OFF);
  gpioManager.setValue(gpio::Function::Time, gpio::Value::OFF);

  return 0;
}
