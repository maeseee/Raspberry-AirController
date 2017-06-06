#pragma once

#include <cstddef>

// Sensors
static constexpr float SET_TEMP = 22.0; // set point for temerautre [°C]
static constexpr float SET_HUM = 50.0;  // set point for humidity [%]

// Time
static constexpr size_t MIN_TO_SEC = 60;                           // [s]
static constexpr size_t HOUR_TO_SEC = 60 * MIN_TO_SEC;             // [s]
static constexpr size_t CALL_INTERVALL_AM2302 = 5 * MIN_TO_SEC;    // [s]
static constexpr size_t CALL_INTERVALL_ROTI = 5 * MIN_TO_SEC;      // [s]
static constexpr size_t CALL_INTERVALL_TEMP = 60 * MIN_TO_SEC;     // [s]
static constexpr size_t CALL_INTERVALL_TIMER = 1 * MIN_TO_SEC;     // [s]
static constexpr size_t CALL_INTERVALL_WEB = 30 * MIN_TO_SEC;      // [s]
static constexpr size_t CALL_INTERVALL_HUMLIMIT = 20 * MIN_TO_SEC; // [s]
static constexpr size_t ONE_TIME_ON_DURATION = 30 * MIN_TO_SEC;    // [s]

// Helper functions
float relHumidityToAbs(const float tempC, const float humidityRel);
float absHumidityToRel(const float tempC, const float humidityAbs);
size_t getDaytime();
