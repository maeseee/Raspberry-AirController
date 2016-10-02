#ifndef ENABLER_H
#define ENABLER_H

#include <memory>
#include <string>
#include <thread>

namespace enabler {

static constexpr float SET_TEMP = 22.0; // set point for temerautre [°C]
static constexpr float SET_HUM = 50.0;  // set point for humidity [%]

class Enabler {
public:
  explicit Enabler(const float setPoint);
  ~Enabler();

  bool shouldBeEnabled(const float indoor, const float outdoor) const;

private:
  float m_setPoint;
};

} // enabler
#endif
