#pragma once

#include <cstddef>

// Sensors
static const float SET_TEMP = 22.0; // set point for temerautre [°C]
static const float SET_HUM = 50.0;  // set point for humidity [%]

// Time
static const size_t SEC_TO_HOUR = 60 * 60;         // [s]
static const size_t CALL_INTERVALL_AM2302 = 60;    // [s]
static const size_t CALL_INTERVALL_ROTI = 5 * 60;  // [s]
static const size_t CALL_INTERVALL_TEMP = 60 * 60; // [s]
static const size_t CALL_INTERVALL_TIMER = 30;     // [s]
static const size_t CALL_INTERVALL_WEB = 20 * 60;  // [s]
