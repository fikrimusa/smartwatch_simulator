/**
 * @file datetime.c
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
#include "datetime.h"

static const char* TAG = "DATETIME";
static smartwatch_time_t current_time = {0};

void datetime_init() {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Load from NVS
    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READWRITE, &handle) == ESP_OK) {
        size_t required_size = sizeof(current_time.timeinfo);
        if (nvs_get_blob(handle, "time", &current_time.timeinfo, &required_size) == ESP_OK) {
            current_time.valid = true;
            ESP_LOGI(TAG, "Loaded time from NVS");
        }
        nvs_close(handle);
    }

    // Fallback to current time
    if (!current_time.valid) {
        time_t now;
        time(&now);
        localtime_r(&now, &current_time.timeinfo);
        current_time.valid = true;
        ESP_LOGW(TAG, "Using system time as fallback");
    }
}

smartwatch_time_t get_current_time() {
    // Simulate clock ticking (called every second)
    time_t temp = mktime(&current_time.timeinfo);
    temp++;
    localtime_r(&temp, &current_time.timeinfo);
    
    return current_time;
}

void set_time(struct tm new_time) {
    current_time.timeinfo = new_time;
    current_time.valid = true;
    
    // Persist to NVS
    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_blob(handle, "time", &current_time.timeinfo, sizeof(current_time.timeinfo));
        nvs_commit(handle);
        nvs_close(handle);
    }
}