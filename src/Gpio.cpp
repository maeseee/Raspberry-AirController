#include "Gpio.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace gpio {

static const std::string GPIO_PATH = "/sys/class/gpio/";

Gpio::Gpio() : m_gpioNumber{0}, m_dir{Direction::UNSET} {}

Gpio::Gpio(const size_t gnum) {
  m_gpioNumber = gnum; // Instatiate GPIOClass object for GPIO pin number "gnum"
}

bool Gpio::exportGpio() {
  std::string export_str = GPIO_PATH + "export";
  std::ofstream exportgpio(export_str.c_str(),
                           std::ofstream::out); // Open "export" file. Convert
                                                // C++ string to C string.
                                                // Required for all Linux
                                                // pathnames
  if (!exportgpio) {
    std::cout << " OPERATION FAILED: Unable to export GPIO" << m_gpioNumber
              << " ." << std::endl;
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
    std::cout << " OPERATION FAILED: Unable to unexport GPIO" << m_gpioNumber
              << " ." << std::endl;
    return false;
  }

  unexportgpio << m_gpioNumber; // write GPIO number to unexport
  unexportgpio.close();         // close unexport file
  return true;
}

bool Gpio::setDirection(const Direction dir) {

  std::string setdir_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/direction";
  std::ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
  if (!setdirgpio) {
    std::cout << " OPERATION FAILED: Unable to set direction of GPIO"
              << m_gpioNumber << " ." << std::endl;
    return false;
  }

  setdirgpio << ((dir == Direction::IN)
                     ? "in"
                     : "out"); // write direction to direction file
  setdirgpio.close();          // close direction file

  m_dir = dir;
  return true;
}

bool Gpio::setValue(const Value val) {
  // only set value if it is an output
  if (Direction::OUT != m_dir) {
    return false;
  }

  std::string setval_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/value";
  std::ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
  if (!setvalgpio) {
    std::cout << " OPERATION FAILED: Unable to set the value "
              << static_cast<size_t>(val) << " of GPIO" << m_gpioNumber << " ."
              << std::endl;
    return false;
  }

  setvalgpio << ((val == Value::ON ? "1" : "0")); // write value to value file
  setvalgpio.close();                             // close value file
  return true;
}

Value Gpio::getValue() const {

  std::string getval_str =
      GPIO_PATH + "gpio" + std::to_string(m_gpioNumber) + "/value";
  std::ifstream getvalgpio(getval_str.c_str()); // open value file for gpio
  if (!getvalgpio) {
    std::cout << " OPERATION FAILED: Unable to get value of GPIO"
              << m_gpioNumber << " ." << std::endl;
    return Value::INVALID;
  }

  std::string val;
  getvalgpio >> val;  // read gpio value
  getvalgpio.close(); // close the value file

  if (val != "0") {
    return Value::ON;
  } else {
    return Value::OFF;
  }
}

} // gpio
