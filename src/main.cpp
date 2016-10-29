#include <Controller/NightAir.hpp>
#include <Controller/RotiController.hpp>
#include <Controller/TempController.hpp>
#include <Gpio/Gpio.hpp>
#include <Sensor/Am2302Sensor.hpp>
#include <Sensor/SensorSim.hpp>
#include <Sensor/WeatherStation.hpp>
#include <SysLogger.hpp>
#include <TimeTrigger.hpp>

#include <iostream>
#include <signal.h>
#include <unistd.h>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * 60 * 60;
static constexpr size_t END_NIGHT_CONDITION = 7 * 60 * 60;
static constexpr size_t SAFETY_CONDITION = 30 * 60;

// signal handler
bool m_runProgram{true};
void sigHandler(int signo) {
  if (signo == SIGINT) {
    std::cout << "received SIGINT" << std::endl;
    m_runProgram = false;
  } else if (signo == SIGKILL) {
    std::cout << "received SIGKILL" << std::endl;
    m_runProgram = false;
  } else if (signo == SIGSTOP) {
    std::cout << "received SIGSTOP" << std::endl;
    m_runProgram = false;
  }
}

int main() {
  // implement signal handler
  if (signal(SIGINT, sigHandler) == SIG_ERR) {
    std::cout << "can't catch SIGINT" << std::endl;
  }
  if (signal(SIGKILL, sigHandler) == SIG_ERR) {
    std::cout << "can't catch SIGKILL" << std::endl;
  }
  if (signal(SIGSTOP, sigHandler) == SIG_ERR) {
    std::cout << "can't catch SIGSTOP" << std::endl;
  }

  // initialize sensor for outdoor values
  sensor::ISensorPtr weatherStation =
      std::make_shared<sensor::WeatherStation>();

  // initialize sensor for outdoor values
  // initialize gpio for one-wire-bus
  gpio::IGpioPtr am2302 = std::make_shared<gpio::Gpio>(gpio::Function::Am2302);
  // create sensor
  sensor::ISensorPtr measuredValues =
      std::make_shared<sensor::Am2302Sensor>(am2302);

  // just wait a little to make sure all sensor threads received initial values
  sleep(2);

  // setup Gpio's
  gpio::IGpioPtr timer = std::make_shared<gpio::Gpio>(
      gpio::Function::NightTime, gpio::Direction::OUT, gpio::Value::LOW);
  gpio::IGpioPtr mainSystem = std::make_shared<gpio::Gpio>(
      gpio::Function::Main, gpio::Direction::OUT, gpio::Value::LOW);
  gpio::IGpioPtr roti = std::make_shared<gpio::Gpio>(
      gpio::Function::Roti, gpio::Direction::OUT, gpio::Value::LOW);

  // setup timer
  time_trigger::TimeTrigger timeTrigger(
      START_NIGHT_CONDITION + SAFETY_CONDITION,
      END_NIGHT_CONDITION - SAFETY_CONDITION, timer);

  // setup roti
  roti_controller::RotiController humidityController(measuredValues,
                                                     weatherStation, roti);

  // setup night air
  night_air::NightAir nightAir(mainSystem);

  // setup temperature
  temp_controller::TempController temp_controller(mainSystem);

  while (true == m_runProgram) {
    sleep(1);
  }

  return 0;
}
