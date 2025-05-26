/**
 * @file main.c
 * @brief Smartwatch simulator main application
 * 
 * @author Fikri
 * @version 0.1
 * @date 2025-05-15
 * @copyright Copyright (c) 2025 Fikri
 * @license MIT
 */

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "heart_rate.h"
#include "esp_log.h"

/* Logging Configuration */
static const char *TAG = "SMARTWATCH";
#define WORKOUT_DELAY_MS 5000
#define NORMAL_MODE_DELAY_MS 10000

/* Global Variables */
static EventGroupHandle_t state_group;

/**
 * @brief Task to display heart rate data
 * 
 * @param pvParam FreeRTOS task parameter (unused)
 * 
 * @note This task blocks indefinitely waiting for HR updates
 */
static void display_task(void *pvParam) {
    QueueHandle_t hr_queue = get_hr_queue();
    int hr_value;

    while (1) {
        if (xQueueReceive(hr_queue, &hr_value, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Current HR: %d BPM", hr_value);
            
            // Add threshold warnings
            if (hr_value > 120) {
                ESP_LOGW(TAG, "High heart rate detected!");
            }
        }   
    }
}

/**
 * @brief Main application entry point
 */
void app_main(void) {
    ESP_LOGI(TAG, "Starting smartwatch simulator...");

    // Initialize event group
    if ((state_group = xEventGroupCreate()) == NULL) {
        ESP_LOGE(TAG, "Failed to create event group!");
        return;
    }

    // Initialize modules
    init_hr_sensor(state_group);

    // Create tasks
    if (xTaskCreate(display_task, "Display", 2048, NULL, 1, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create display task!");
    }

    // Demo sequence
    ESP_LOGI(TAG, "Simulating normal mode...");
    vTaskDelay(pdMS_TO_TICKS(WORKOUT_DELAY_MS));
    
    ESP_LOGW(TAG, "Activating workout mode...");
    xEventGroupSetBits(state_group, WORKOUT_BIT);
    
    vTaskDelay(pdMS_TO_TICKS(NORMAL_MODE_DELAY_MS));
    
    ESP_LOGW(TAG, "Returning to normal mode...");
    xEventGroupClearBits(state_group, WORKOUT_BIT);
}