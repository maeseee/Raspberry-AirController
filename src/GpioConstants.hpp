#ifndef GPIO_CONSTANTS_H
#define GPIO_CONSTANTS_H

#include <cstddef>
#include <functional>

namespace gpio {

enum class Direction { OUT, IN, UNSET };
enum class Value { LOW, HIGH, INVALID };
enum Function : size_t { Main = 0, Roti, Time, Reserve, Am2302, Size };

} // gpio
#endif
