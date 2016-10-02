#ifndef GPIO_CONSTANTS_H
#define GPIO_CONSTANTS_H

#include <cstddef>
#include <functional>

namespace gpio {

enum class Direction { OUT, IN, UNSET };
enum class Value { ON, OFF, INVALID };
enum Function : size_t { Main = 0, Roti = 1, Time = 2, Reserve = 3, Size = 4 };

using GpioSetter = std::function<void(const gpio::Value)>;


} // gpio
#endif
