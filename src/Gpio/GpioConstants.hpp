#ifndef GPIO_CONSTANTS_H
#define GPIO_CONSTANTS_H

#include <cstddef>

namespace gpio {

/**
 * @brief The Direction enum sets the gpio direction
 */
enum class Direction : size_t { OUT, IN, UNSET };

/**
 * @brief The Value enum sets/reads the value of the gpio
 */
enum class Value : size_t { LOW, HIGH, INVALID };

/**
 * @brief The Function enum maps a string to a pin number
 */
enum class Function : size_t {
  Main = 3,
  Am2302 = 4,
  Roti = 5,
  Reserve = 7,
  NightTime = 8
};



} // gpio
#endif
