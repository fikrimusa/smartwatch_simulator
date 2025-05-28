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

/* Logging Configuration */
static const char *TAG = "SMARTWATCH";
#define WORKOUT_DELAY_MS 5000
#define NORMAL_MODE_DELAY_MS 10000

/* Global Variables */
static EventGroupHandle_t state_group;

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
    ESP_LOGI(TAG, "Starting smartwatch simulator...");

    // Initialize event group
    if ((state_group = xEventGroupCreate()) == NULL) {
        ESP_LOGE(TAG, "Event group creation failed!");
        return;
    }

    // Initialize all modules
    init_hr_sensor(state_group);
    init_step_counter(state_group);

    // Create monitoring task
    if (xTaskCreate(display_task, 
                   "Monitor", 
                   3072,  // Increased stack for additional features
                   NULL, 
                   2,     // Higher priority than sensors
                   NULL) != pdPASS) {
        ESP_LOGE(TAG, "Monitor task creation failed!");
    }

    /*** Demo Sequence ***/
    ESP_LOGW(TAG, "Normal mode started");
    vTaskDelay(pdMS_TO_TICKS(WORKOUT_DELAY_MS));
    
    // Activate workout mode
    ESP_LOGW(TAG, "=== WORKOUT MODE ACTIVATED ===");
    xEventGroupSetBits(state_group, WORKOUT_BIT);
    
    // vTaskDelay(pdMS_TO_TICKS(NORMAL_MODE_DELAY_MS));
    
    // // Return to normal
    // ESP_LOGW(TAG, "=== RETURNING TO NORMAL MODE ===");
    // xEventGroupClearBits(state_group, WORKOUT_BIT);
    
    // Show final stats after 15s
    vTaskDelay(pdMS_TO_TICKS(15000));
    ESP_LOGW(TAG, "Session complete! Total steps: %d", get_steps());
}