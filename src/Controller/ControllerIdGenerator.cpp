#include "ControllerIdGenerator.hpp"

#include <SysLogger.hpp>

namespace controller {

std::atomic<size_t> IdGenerator::idCounter(0);

size_t IdGenerator::generateId(const std::string &name) {
  size_t expectedId;
  size_t desiredId;
  do {
    expectedId = idCounter.load();
    desiredId = expectedId + 1;
  } while (!idCounter.compare_exchange_strong(expectedId, desiredId));

  logger::SysLogger::instance().log("Controller " + name + " got the id " +
                                    std::to_string(desiredId));

  return desiredId;
}
}
