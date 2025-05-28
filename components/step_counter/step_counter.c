#include "step_counter.h"
#include "esp_log.h"

static const char *TAG = "STEP";
static uint32_t step_count = 0;
static EventGroupHandle_t state_group;

/**
 * @brief Simulates step detection using timing patterns
 */
static void step_simulation_task(void *pvParam) 
{
    const TickType_t normal_delay = pdMS_TO_TICKS(2000);   // 2s/step
    const TickType_t workout_delay = pdMS_TO_TICKS(500);    // 0.5s/step
    
    while(1) {
        // Detect movement state
        TickType_t delay = (xEventGroupGetBits(state_group) & WORKOUT_BIT) ? 
                          workout_delay : normal_delay;
        
        step_count++;
        
        // Every 10 steps, trigger alert
        if(step_count % 10 == 0) {
            xEventGroupSetBits(state_group, STEP_ALERT_BIT);
            ESP_LOGW(TAG, "Step milestone: %d", step_count);
        }
        
        vTaskDelay(delay);
    }
}

void init_step_counter(EventGroupHandle_t ext_state_group) 
{
    state_group = ext_state_group;
    xTaskCreate(
        step_simulation_task,
        "StepCounter",
        2048,
        NULL,
        2,
        NULL
    );
}

uint32_t get_steps() {
    return step_count;
}

void reset_steps() {
    step_count = 0;
    ESP_LOGI(TAG, "Step counter reset");
}