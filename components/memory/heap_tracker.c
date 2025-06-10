#include "esp_heap_caps.h"
#include "heap_tracker.h"
#include "esp_system.h"
#include "esp_log.h"

void print_memory_stats() {
    ESP_LOGI("MEM", "---------------------");
    ESP_LOGI("MEM", "Internal Free: %6d bytes", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    ESP_LOGI("MEM", "Largest Block: %6d bytes", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
    ESP_LOGI("MEM", "Min Ever Free: %6lu bytes", esp_get_minimum_free_heap_size());
    ESP_LOGI("MEM", "---------------------");
    
    #if CONFIG_SPIRAM
    ESP_LOGI("MEM", "SPIRAM Free: %6d bytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    #endif
}