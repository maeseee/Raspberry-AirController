#ifndef GPIO_CONSTANTS_H
#define GPIO_CONSTANTS_H

#include <cstddef>

namespace gpio {

enum class Direction { OUT, IN, UNSET };
enum class Value { LOW, HIGH, INVALID };
enum class Function : size_t {
  Main = 3,
  Roti = 5,
  Time = 8,
  Reserve = 7,
  Am2302 = 9
};

} // gpio
#endif
