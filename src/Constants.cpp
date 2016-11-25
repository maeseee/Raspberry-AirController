#include <Constants.hpp>

#include <cmath>

float relHumidityToAbs(const float tempC, const float humidityRel) {
  // function calculated for
  // Temperatur [°C]  water for 100 % [g/m3]
  // -20              0.9
  // -15	          1.4
  // -10	          2.1
  // -5	              3.3
  // 0	              4.8
  // 5	              6.8
  // 10	              9.4
  // 15	              12.8
  // 20	              17.3
  // 25	              23
  // 30	              30.3
  // 35	              39.6
  // 40	              51.1

  return 4.2431796244 * exp(0.0666427637 * tempC) * humidityRel / 100;
}

float absHumidityToRel(const float tempC, const float humidityAbs) {
  float absHumidity100 = relHumidityToAbs(tempC, 100);
  return humidityAbs / absHumidity100 * 100;
}
