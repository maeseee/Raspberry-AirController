#include <Constants.hpp>
#include <Controller/HumLimitController.hpp>
#include <Controller/NightAir.hpp>
#include <Controller/OneTimeTrigger.hpp>
#include <Controller/RotiController.hpp>
#include <Controller/TempController.hpp>
#include <Controller/TimeTrigger.hpp>
#include <Gpio/Gpio.hpp>
#include <Gpio/GpioOr.hpp>
#include <Http/HttpServer.hpp>
#include <Sensor/Am2302Sensor.hpp>
#include <Sensor/WeatherStation.hpp>
#include <SysLogger.hpp>

#include <execinfo.h> // for call stack
#include <iostream>
#include <signal.h>
#include <unistd.h>

// timer constants
static constexpr size_t START_NIGHT_CONDITION = 22 * HOUR_TO_SEC;
static constexpr size_t END_NIGHT_CONDITION = 7 * HOUR_TO_SEC;
static constexpr size_t SAFETY_CONDITION = 30 * MIN_TO_SEC;

/* Obtain a backtrace and print it to stdout. */
void printTrace(void)
{
    void* array[10];
    size_t size;
    char** strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);

    free(strings);
}

// signal handler
bool m_runProgram{true};
void sigHandler(int signo)
{
    if (signo == SIGINT) {
        m_runProgram = false;
    } else if (signo == SIGSEGV) {
        printTrace();
        m_runProgram = false;
    } else if (signo == SIGABRT) {
        printTrace();
        m_runProgram = false;
    }
}

int main()
{
    logger::SysLoggerPtr sysLogger = std::make_shared<logger::SysLogger>();
    size_t loggerId = sysLogger->generateId("Main");
    sysLogger->logMsg(loggerId, "Start application");

    // implement signal handler
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGINT");
    } else if (signal(SIGSEGV, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGSEGV");
    } else if (signal(SIGABRT, sigHandler) == SIG_ERR) {
        sysLogger->logMsg(loggerId, "can't catch SIGSEGV");
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

    // setup roti
    controller::RotiController humidityController(indoorSensor, outdoorSensor, roti, sysLogger);
    // setup main system
    gpio::IGpioPtr mainSystemOr = std::make_shared<gpio::GpioOr>(mainSystem, sysLogger);
    // setup night air controller
    controller::NightAir nightAir(mainSystemOr, sysLogger);
    // setup temperature controller
    controller::TempController tempController(mainSystemOr, sysLogger);
    // setup humidity limit controller
    controller::HumLimitController humLimitController(indoorSensor, outdoorSensor, mainSystemOr, sysLogger);

    time_trigger::OneTimeTriggerPtr oneTimeTrigger =
        std::make_shared<time_trigger::OneTimeTrigger>(mainSystemOr, sysLogger);
    http_server::initHttpServer(oneTimeTrigger, sysLogger);

    sysLogger->logMsg(loggerId, "Set up finished");
    while (true == m_runProgram) {
        sleep(1);
    }

    sysLogger->logMsg(loggerId, "Finish application");
    return 0;
}
