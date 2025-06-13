/**
 * @file datetime.h
 * @brief Smartwatch Time Management Implementation
 * 
 * Handles all time-related functionality including:
 * - NVS-based persistent time storage
 * - Real-time clock simulation
 * - Time setting operations
 * 
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 */
#pragma once

#include <time.h>
#include "esp_sntp.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

// Time structure with validation
typedef struct {
    struct tm timeinfo;
    bool valid;
} smartwatch_time_t;

/**
 * @brief Initialize datetime system
 * 
 * Performs:
 * 1. NVS flash initialization
 * 2. Time loading from persistent storage
 * 3. Fallback to system time if no stored time exists
 * 
 * @note Will erase NVS partition if corrupted and retry initialization
 */
void datetime_init();

/**
 * @brief Get current smartwatch time
 * 
 * Simulates clock ticking by incrementing time by 1 second on each call.
 * Maintains all time components (hours, minutes, seconds) in valid ranges.
 * 
 * @return smartwatch_time_t Current time with validity flag
 */
smartwatch_time_t get_current_time();

/**
 * @brief Set new time for smartwatch
 * 
 * @param new_time struct tm containing new time values. Only valid fields will be updated.
 * 
 * @note Persists the new time to NVS flash storage automatically
 */
void set_time(struct tm new_time);