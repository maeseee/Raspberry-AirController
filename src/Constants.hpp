#pragma once

#include <cstddef>

// Sensors
static const float SET_TEMP = 22.0;          // set point for temerautre [°C]
static const float SET_HUM = 50.0;           // set point for humidity [%]
static const float HUM_LIMIT_TOLERANCE = 10; // +- This tolenrance for no action

// Time
static const size_t MIN_TO_SEC = 60;                           // [s]
static const size_t HOUR_TO_SEC = 60 * MIN_TO_SEC;             // [s]
static const size_t CALL_INTERVALL_AM2302 = 60;                // [s]
static const size_t CALL_INTERVALL_ROTI = 5 * MIN_TO_SEC;      // [s]
static const size_t CALL_INTERVALL_TEMP = 60 * MIN_TO_SEC;     // [s]
static const size_t CALL_INTERVALL_TIMER = 30;                 // [s]
static const size_t CALL_INTERVALL_WEB = 30 * MIN_TO_SEC;      // [s]
static const size_t CALL_INTERVALL_HUMLIMIT = 20 * MIN_TO_SEC; // [s]
static const size_t SENSOR_LOG_INTERVALL = 30 * MIN_TO_SEC;    // [s]

// Helper functions
float relHumidityToAbs(const float tempC, const float humidityRel);
float absHumidityToRel(const float tempC, const float humidityAbs);
