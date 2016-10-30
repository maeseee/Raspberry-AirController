#pragma once

#include <atomic>
#include <cstddef>

namespace controller {

class IdGenerator {
public:
  static size_t generateId();

private:
  static std::atomic<size_t> idCounter;
};
}
