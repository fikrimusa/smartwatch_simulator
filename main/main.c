/**
 * @file main.c
 * @brief Smartwatch simulator main application
 * 
 * @author Fikri
 * @version 0.2
 * @date 2025-05-15
 * @copyright Copyright (c) 2025 Fikri
 * @license MIT
 */

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "heart_rate.h"
#include "step_counter.h"
#include "esp_log.h"
#include "cli.h"
#include "datetime.h"
#include "heap_tracker.h"

/* Logging Configuration */
static const char *TAG = "SMARTWATCH";
#define WORKOUT_DELAY_MS 5000
#define NORMAL_MODE_DELAY_MS 10000

/* Global Variables */
static EventGroupHandle_t state_group;

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
 * @brief Enhanced display task now shows both HR and steps
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
 * @brief Main application with full initialization
 */
void app_main(void) {
    //Initialize memory systems
    print_memory_stats();

    // Initialize event group
    state_group = xEventGroupCreate();
    
    // Initialize components
    init_hr_sensor(state_group);
    init_step_counter(state_group);
    init_cli();
    
    // Create tasks
    xTaskCreate(time_task, "TimeKeeper", 2048, NULL, 1, NULL);
    xTaskCreate(display_task, "Monitor", 3072, NULL, 2, NULL);
    xTaskCreate(cli_task, "CLI", 2048, NULL, 1, NULL);
    
    ESP_LOGI(TAG, "Smartwatch started with simple CLI");
}