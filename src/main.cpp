#include <Constants.hpp>
#include <Controller/HumLimitController.hpp>
#include <Controller/NightAir.hpp>
#include <Controller/RotiController.hpp>
#include <Controller/TempController.hpp>
#include <Controller/TimeTrigger.hpp>
#include <Gpio/Gpio.hpp>
#include <Gpio/GpioOr.hpp>
#include <Sensor/Am2302Sensor.hpp>
#include <Sensor/SensorSim.hpp>
#include <Sensor/WeatherStation.hpp>
#include <SysLogger.hpp>

#include <signal.h>
#include <unistd.h>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * HOUR_TO_SEC;
static constexpr size_t END_NIGHT_CONDITION = 7 * HOUR_TO_SEC;
static constexpr size_t SAFETY_CONDITION = 30 * MIN_TO_SEC;

// signal handler
bool m_runProgram{true};
void sigHandler(int signo) {
  if (signo == SIGINT) {
    logger::SysLogger::instance().log("received SIGINT");
    m_runProgram = false;
  }
}

int main() {
  // implement signal handler
  if (signal(SIGINT, sigHandler) == SIG_ERR) {
    logger::SysLogger::instance().log("can't catch SIGINT");
  }

  // initialize outputs
  gpio::IGpioPtr timer = std::make_shared<gpio::Gpio>(
      gpio::Function::NightTime, gpio::Direction::OUT, gpio::Value::LOW);
  gpio::IGpioPtr mainSystem = std::make_shared<gpio::Gpio>(
      gpio::Function::Main, gpio::Direction::OUT, gpio::Value::LOW);
  gpio::IGpioPtr roti = std::make_shared<gpio::Gpio>(
      gpio::Function::Roti, gpio::Direction::OUT, gpio::Value::LOW);

  // initialize sensors
  sensor::ISensorPtr outdoorSensor = std::make_shared<sensor::WeatherStation>();

  gpio::IGpioPtr am2302 = std::make_shared<gpio::Gpio>(
      gpio::Function::Am2302, gpio::Direction::OUT,
      gpio::Value::LOW); // initialize gpio for one-wire-bus
  sensor::ISensorPtr indoorSensor =
      std::make_shared<sensor::Am2302Sensor>(am2302);

  // just wait a little to make sure all sensor threads received initial values
  sleep(2);

  // setup timer
  time_trigger::TimeTrigger timeTrigger(
      START_NIGHT_CONDITION + SAFETY_CONDITION,
      END_NIGHT_CONDITION - SAFETY_CONDITION, timer);

  // setup roti
  controller::RotiController humidityController(indoorSensor, outdoorSensor,
                                                roti);
  // setup main system
  gpio::IGpioPtr mainSystemOr = std::make_shared<gpio::GpioOr>(mainSystem);
  // setup night air controller
  controller::NightAir nightAir(mainSystemOr);
  // setup temperature controller
  controller::TempController tempController(mainSystemOr);
  // setup humidity limit controller
  controller::HumLimitController humLimitController(indoorSensor, outdoorSensor,
                                                    mainSystemOr);

  while (true == m_runProgram) {
    sleep(1);
  }

  return 0;
}
