/**
 * @file main.c
 * @brief Smartwatch Simulator Core Application
 * 
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 * 
 * @note System Features:
 * - Real-time clock display
 * - Heart rate monitoring (simulated)
 * - Step counting with milestones
 * - Interactive CLI interface
 * - Dynamic workout detection
 * - Memory monitoring
 * 
 * @warning Ensure proper configuration in sdkconfig.h
 * @attention Set correct task priorities for optimal performance
 */
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "cli.h"
#include "datetime.h"
#include "heart_rate.h"
#include "heap_tracker.h"
#include "step_counter.h"

/* Logging Configuration */
static const char *TAG = "SMARTWATCH";
#define WORKOUT_DELAY_MS 5000
#define NORMAL_MODE_DELAY_MS 10000

/* Global Variables */
static EventGroupHandle_t state_group;

/**
 * @brief Real-Time Clock Management Task
 * 
 * Maintains and displays current time with:
 * - Automatic time synchronization
 * - Continuous time updates
 * - Validation checking
 * 
 * @param pvParam Unused task parameter
 */
void time_task(void *pvParam) {
    datetime_init();
    
    while(1) {
        smartwatch_time_t now = get_current_time();
        
        if(now.valid) {
            ESP_LOGI("CLOCK", "%02d:%02d:%02d",
                   now.timeinfo.tm_hour,
                   now.timeinfo.tm_min,
                   now.timeinfo.tm_sec);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief System Monitoring Task
 * 
 * Handles:
 * - Dynamic heart rate display
 * - Step counting with delta calculation
 * - Milestone detection
 * - Workout mode adaptation
 * 
 * @param pvParam Unused task parameter
 */
static void display_task(void *pvParam) {
    QueueHandle_t hr_queue = get_hr_queue();
    int hr_value;
    uint32_t last_steps = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while (1) {
        /* Dynamic Polling Rate */
        const TickType_t xPollingInterval = 
            (xEventGroupGetBits(state_group) & WORKOUT_BIT) ? 
            pdMS_TO_TICKS(300) :  // Faster in workout mode
            pdMS_TO_TICKS(1000);  // Slower in normal mode

        /* Heart Rate Monitoring (Non-blocking) */
        if (xQueueReceive(hr_queue, &hr_value, 0) == pdTRUE) {  // 0 timeout = non-blocking
            uint32_t current_steps = get_steps();
            int32_t step_delta = current_steps - last_steps;
            
            ESP_LOGI(TAG, "HR: %d BPM | Steps: %"PRIu32" (Δ%"PRId32")", 
                    hr_value, current_steps, step_delta);
            
            last_steps = current_steps;

            if (hr_value > 120) {
                ESP_LOGW(TAG, "High heart rate warning!");
            }
        }

        /* Step Milestone Check */
        EventBits_t bits = xEventGroupGetBits(state_group);
        if (bits & STEP_ALERT_BIT) {
            uint32_t current_steps = get_steps();
            ESP_LOGI(TAG, "Step milestone reached: %"PRIu32" steps", current_steps);
            
            if (current_steps >= 50) {
                reset_steps();
                last_steps = 0;
                ESP_LOGI(TAG, "Step counter reset!");
            }
            
            xEventGroupClearBits(state_group, STEP_ALERT_BIT);
        }

        /* Precision Delay */
        vTaskDelayUntil(&xLastWakeTime, xPollingInterval);
    }
}

/**
 * @brief Main Application Entry Point
 * 
 * Initializes all system components and starts core tasks:
 * 1. Memory system diagnostics
 * 2. Event group creation
 * 3. Peripheral initialization
 * 4. Task creation with appropriate priorities
 */
void app_main(void) {
    // System initialization
    print_memory_stats();
    state_group = xEventGroupCreate();
    
    // Component initialization
    init_hr_sensor(state_group);
    init_step_counter(state_group);
    init_cli();
    
    // Task creation (ordered by priority)
    xTaskCreate(time_task, "TimeKeeper", 2048, NULL, 1, NULL);
    xTaskCreate(display_task, "Monitor", 3072, NULL, 2, NULL);
    xTaskCreate(cli_task, "CLI", 2048, NULL, 1, NULL);
    
    ESP_LOGI(TAG, "Smartwatch started with simple CLI");
}