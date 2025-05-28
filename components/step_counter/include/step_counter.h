/**
 * @file step_counter.h
 * @brief Pedometer simulation with FreeRTOS
 * 
 * Features:
 * - Auto-increment during movement
 * - Workout mode detection
 * - Reset functionality
 */
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

/**
 * @brief Initialize step counter module
 * @param state_group Event group for workout detection
 */
void init_step_counter(EventGroupHandle_t state_group);

/**
 * @brief Get current step count
 * @return Current step total
 */
uint32_t get_steps();

/**
 * @brief Reset step counter to zero
 */
void reset_steps();

// Event bit for step alerts
#define STEP_ALERT_BIT (1 << 2)

#define WORKOUT_BIT (1 << 0) // Bit 0