#include "Enabler.hpp"

namespace enabler {

Enabler::Enabler(const float setPoint) : m_setPoint(setPoint) {}

Enabler::~Enabler() {}

bool Enabler::shouldBeEnabled(const float indoor, const float outdoor) const {
  bool increaseIndoor = m_setPoint > indoor;
  bool isOutdoorHigher = indoor < outdoor;

  return increaseIndoor ^ isOutdoorHigher;
}
} // enabler
