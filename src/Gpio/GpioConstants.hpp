#pragma once

#include <cstddef>

namespace gpio
{

/**
 * @brief The Direction enum sets the gpio direction
 */
enum class Direction { OUT, IN, UNSET };

/**
 * @brief The Value enum sets/reads the value of the gpio
 */
enum class Value { LOW, HIGH, INVALID };

/**
 * @brief The Function enum maps a string to a pin number
 */
enum class Function {
    Am2302 = 4,    // Pin 07
    Main = 5,      // Pin 29
    Roti = 6,      // Pin 31
    Reserve = 13,  // Pin 33
    NightTime = 19 // Pin 35
};
} // namespace gpio
