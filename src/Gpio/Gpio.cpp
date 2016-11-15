#include "Gpio.hpp"
#include <SysLogger.hpp>

#include <fstream>
#include <string>

namespace gpio {

static const std::string GPIO_PATH = "/sys/class/gpio/";

bool isRealBoard() {
  std::ifstream stream(GPIO_PATH + "export");
  return stream.good();
}

Gpio::Gpio(const Function gnum, const Direction dir, const Value val)
    : m_gpioNumber(static_cast<size_t>(gnum)) {
  exportGpio();
  setDirection(0, dir);
  setValue(0, val);
}

Gpio::~Gpio() { setValue(0, Value::LOW); }

bool Gpio::exportGpio() {
  std::string export_str = GPIO_PATH + "export";
  std::ofstream exportgpio(export_str.c_str(),
                           std::ofstream::out); // Open "export" file. Convert
                                                // C++ string to C string.
                                                // Required for all Linux
                                                // pathnames
  if (!exportgpio) {
    logger::SysLogger::instance().log(
        "OPERATION FAILED: Unable to export GPIO " +
        std::to_string(m_gpioNumber));
    return false;
  }

  exportgpio << m_gpioNumber; // write GPIO number to export
  exportgpio.close();         // close export file

  return true;
}

bool Gpio::unexportGpio() {
  std::string unexport_str = GPIO_PATH + "unexport";
  std::ofstream unexportgpio(unexport_str.c_str()); // Open unexport file
  if (!unexportgpio) {
    logger::SysLogger::instance().log(
        "OPERATION FAILED: Unable to unexport GPIO " +
        std::to_string(m_gpioNumber));
    return false;
  }

  unexportgpio << m_gpioNumber; // write GPIO number to unexport
  unexportgpio.close();         // close unexport file

  return true;
}

bool Gpio::setDirection(const size_t /*controllerId*/, const Direction dir) {
  std::string setdir_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/direction";
  std::ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
  if (!setdirgpio) {
    logger::SysLogger::instance().log(
        "OPERATION FAILED: Unable to set direction of GPIO " +
        std::to_string(m_gpioNumber));
    return false;
  }

  setdirgpio << ((dir == Direction::IN)
                     ? "in"
                     : "out"); // write direction to direction file
  setdirgpio.close();          // close direction file

  return true;
}

Direction Gpio::getDirection() const {
  std::string setdir_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/direction";
  std::ifstream getdirgpio(setdir_str.c_str()); // open direction file for gpio
  if (!getdirgpio) {
    logger::SysLogger::instance().log(
        "OPERATION FAILED: Unable to get direction of GPIO " +
        std::to_string(m_gpioNumber));
    return Direction::UNSET;
  }

  std::string dir;
  getdirgpio >> dir;  // read gpio value
  getdirgpio.close(); // close the value file

  if ("in" == dir) {
    return Direction::IN;
  } else if ("out" == dir) {
    return Direction::OUT;
  } else {
    return Direction::UNSET;
  }
}

bool Gpio::setValue(const size_t /*controllerId*/, const Value val) {
  if (val == getValue()) {
    // Value is already set
    return true;
  }

  // only set value if it is an output
  if (Direction::OUT != getDirection()) {
    return false;
  }

  std::string valueString = ((val == Value::HIGH) ? "1" : "0");
  std::string setval_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/value";
  std::ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
  if (!setvalgpio) {
    logger::SysLogger::instance().log("OPERATION FAILED: Unable to set value " +
                                      valueString + " of GPIO " +
                                      std::to_string(m_gpioNumber));
    return false;
  }

  setvalgpio << valueString; // write value to value file
  setvalgpio.close();        // close value file

  logger::SysLogger::instance().log("GPIO " + std::to_string(m_gpioNumber) +
                                    " set to " + valueString);
  return true;
}

Value Gpio::getValue() const {

  std::string getval_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/value";
  std::ifstream getvalgpio(getval_str.c_str()); // open value file for gpio
  if (!getvalgpio) {
    logger::SysLogger::instance().log(
        "OPERATION FAILED: Unable to get value of GPIO " +
        std::to_string(m_gpioNumber));

    return Value::INVALID;
  }

  std::string val;
  getvalgpio >> val;  // read gpio value
  getvalgpio.close(); // close the value file

  if (val != "0") {
    return Value::HIGH;
  } else {
    return Value::LOW;
  }
}

size_t Gpio::getPinNumber() const { return m_gpioNumber; }

} // gpio
