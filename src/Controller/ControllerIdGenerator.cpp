#include "ControllerIdGenerator.hpp"

namespace controller {

std::atomic<size_t> IdGenerator::idCounter(0);

size_t IdGenerator::generateId() {
  size_t expectedId;
  size_t desiredId;
  do {
    expectedId = idCounter.load();
    desiredId = expectedId + 1;
  } while (!idCounter.compare_exchange_strong(expectedId, desiredId));

  return desiredId;
}
}
