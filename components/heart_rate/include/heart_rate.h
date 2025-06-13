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
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "event_bits.h"

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