#include "HumLimitController.hpp"
#include <Controller/SensorController.hpp>
#include <Gpio/IGpio.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <cmath>
#include <ctime>
#include <sstream>

namespace controller
{

HumLimitController::HumLimitController(const SensorControllerPtr& sensController,
                                       const gpio::IGpioPtr& gpioMainSystem,
                                       const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_HUMLIMIT)
    , m_sensController(sensController)
    , m_gpio(gpioMainSystem)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("HumLimitController"))
{
    assert(m_sensController);
    assert(m_gpio);

    setInitialized();
}

void HumLimitController::recall()
{
    if (m_sensController->shouldHumControllerBeEnabled()) {
        m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
    } else {
        m_gpio->setValue(m_loggerId, gpio::Value::LOW);
    }
}
}
