#include <Controller/HumLimitController.hpp>
#include <Controller/NightAir.hpp>
#include <Controller/OneTimeTrigger.hpp>
#include <Controller/RotiController.hpp>
#include <Controller/SensorController.hpp>
#include <Controller/TempController.hpp>
#include <Controller/TimeTrigger.hpp>
#include <Gpio/Gpio.hpp>
#include <Gpio/GpioOr.hpp>
#include <Http/HttpServer.hpp>
#include <Sensor/Am2302Sensor.hpp>
#include <Sensor/WeatherStation.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <csignal>
#include <execinfo.h> // for call stack
#include <iostream>
#include <unistd.h>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * HOUR_TO_SEC;
static constexpr size_t END_NIGHT_CONDITION = 7 * HOUR_TO_SEC;
static constexpr size_t SAFETY_CONDITION = 15 * MIN_TO_SEC;

/* Obtain a backtrace and print it to stdout. */
void printTrace()
{
    void* array[10];
    char** strings;

    const int size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    std::cout << "Obtained " << size << " stack frames." << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << strings[i] << std::endl;
    }

    free(strings);
}

// signal handler
bool m_runProgram{true};
void sigHandler(int signo)
{
    if (signo == SIGINT) {
        m_runProgram = false;
    } else if ((signo == SIGSEGV) || (signo == SIGABRT)) {
        printTrace();
        m_runProgram = false;
    }
}

int main()
{
    logger::SysLoggerPtr sysLogger = std::make_shared<logger::SysLogger>();
    const size_t loggerId = sysLogger->generateId("Main");
    sysLogger->logMsg(loggerId, "Start application");

    // implement signal handler
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGINT");
    } else if (signal(SIGSEGV, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGSEGV");
    } else if (signal(SIGABRT, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGABRT");
    }

    // initialize outputs
    gpio::IGpioPtr timer =
        gpio::createGpio(gpio::Function::NightTime, gpio::Direction::OUT, gpio::Value::LOW, sysLogger);
    gpio::IGpioPtr mainSystem =
        gpio::createGpio(gpio::Function::Main, gpio::Direction::OUT, gpio::Value::LOW, sysLogger);
    gpio::IGpioPtr roti = gpio::createGpio(gpio::Function::Roti, gpio::Direction::OUT, gpio::Value::LOW, sysLogger);

    // initialize sensors
    sensor::ISensorPtr outdoorSensor = std::make_shared<sensor::WeatherStation>(sysLogger);

    gpio::IGpioPtr am2302 = gpio::createGpio(gpio::Function::Am2302, gpio::Direction::OUT, gpio::Value::LOW,
                                             sysLogger); // initialize gpio for one-wire-bus
    sensor::ISensorPtr indoorSensor = std::make_shared<sensor::Am2302Sensor>(am2302, sysLogger);

    // just wait a little to make sure all sensor threads received initial values
    sleep(2);

    // setup timer
    time_trigger::TimeTrigger timeTrigger(START_NIGHT_CONDITION + SAFETY_CONDITION,
                                          END_NIGHT_CONDITION - SAFETY_CONDITION, timer, "NightPower", sysLogger);

    // setup sensor controller
    controller::SensorControllerPtr sensorController =
        std::make_shared<controller::SensorController>(indoorSensor->getData(), outdoorSensor->getData());
    // setup roti
    controller::RotiController humidityController(sensorController, roti, sysLogger);
    // setup main system
    gpio::IGpioPtr mainSystemOr = std::make_shared<gpio::GpioOr>(mainSystem, sensorController, sysLogger);
    // setup night air controller
    controller::NightAir nightAir(mainSystemOr, sysLogger);
    // setup temperature controller
    controller::TempController tempController(mainSystemOr, sysLogger);
    // setup humidity limit controller
    controller::HumLimitController humLimitController(sensorController, mainSystemOr, sysLogger);

    time_trigger::OneTimeTriggerPtr oneTimeTrigger =
        std::make_shared<time_trigger::OneTimeTrigger>(mainSystemOr, sysLogger);
    http_server::initHttpServer(oneTimeTrigger, sysLogger);

    sysLogger->logMsg(loggerId, "Set up finished");
    while (m_runProgram) {
        sleep(1);
    }

    sysLogger->logMsg(loggerId, "Finish application");
    return 0;
}
