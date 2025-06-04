/**
 * @file datetime.h
 * @brief NVS-backed real-time clock simulation
 */
#pragma once

#include <time.h>
#include "esp_sntp.h"

// Time structure with validation
typedef struct {
    struct tm timeinfo;
    bool valid;
} smartwatch_time_t;

/**
 * @brief Initialize persistent time storage
 */
void datetime_init();

/**
 * @brief Get current time (cached)
 */
smartwatch_time_t get_current_time();

/**
 * @brief Set new time
 * @param new_time Time structure (tm_sec, tm_min, etc.)
 */
void set_time(struct tm new_time);

/**
 * @brief Sync with NTP if WiFi available (simulated)
 */
void sync_ntp_time();