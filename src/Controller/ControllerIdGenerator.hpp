#pragma once

#include <atomic>
#include <cstddef>
#include <string>

namespace controller {

class IdGenerator {
public:
  size_t generateId(const std::string& name);

private:
  static std::atomic<size_t> idCounter;
};
}
