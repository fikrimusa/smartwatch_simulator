#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "heart_rate.h"
#include "esp_log.h"

static const char *DebugTAG = "Debug";

EventGroupHandle_t state_group;

void display_task(void *pvParam){
    QueueHandle_t hr_queue = get_hr_queue();
    int hr_value;

    while (1)
    {
        if (xQueueReceive(hr_queue, &hr_value, portMAX_DELAY) == pdTRUE)
        {
            ESP_LOGI(DebugTAG, "Current HR: %d BPM", hr_value);
        }   
    }
}


void app_main(void)
{
    //Create event group for state management
    state_group = xEventGroupCreate();

    //Init modules
    init_hr_sensor(state_group);

    //Create display task
    xTaskCreate(display_task, "Display", 2048, NULL, 1, NULL);

    // Simulate workout mode after 5s
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    xEventGroupSetBits(state_group, WORKOUT_BIT);
    ESP_LOGW(DebugTAG, "Workout mode activated!");
}
