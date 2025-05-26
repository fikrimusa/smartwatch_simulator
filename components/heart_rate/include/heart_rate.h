/**
 * @file heart_rate.h
 * @brief Heart rate sensor interface for smartwatch simulator
 * 
 * @author Fikri
 * @version 0.1
 * @date 2025-05-15
 * 
 * @copyright Copyright (c) 2025 Fikri
 * @license MIT
 */

#ifndef HEART_RATE_H
#define HEART_RATE_H

#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the heart rate sensor simulation
 * 
 * @param state_group Event group handle for system state management
 * @note Requires an initialized Event Group handle
 */
void init_hr_sensor(EventGroupHandle_t state_group);

/**
 * @brief Gets the heart rate data queue handle
 * 
 * @return QueueHandle_t Queue containing integer BPM values
 * @warning Always check for NULL return before using the queue
 */
QueueHandle_t get_hr_queue();

/**
 * @def WORKOUT_BIT
 * @brief Event bit flag indicating workout mode
 * 
 * @value (1 << 0) Bit position 0 in event group
 * @see EventGroups.h
 */
#define WORKOUT_BIT (1 << 0) // Bit 0

#ifdef __cplusplus
}
#endif

#endif /* HEART_RATE_H */