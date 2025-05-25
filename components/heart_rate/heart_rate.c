#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "heart_rate.h"

// File-scope variables (static for encapsulation)
static QueueHandle_t hr_queue;
static EventGroupHandle_t state_group;

void hr_task(void *pvParam){
    while(1){
        int hr = 60 + (rand() % 40); // Base 60-100 BPM

        //Simulate workout impact
        if(xEventGroupGetBits(state_group) & WORKOUT_BIT){
            hr += 20 + (rand() % 40); // Base 80-140 BPM
        }

        xQueueSend(hr_queue, &hr, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void init_hr_sensor(EventGroupHandle_t ext_state_group){
    state_group = ext_state_group;
    hr_queue = xQueueCreate(5, sizeof(int));
    xTaskCreate( hr_task, "HR", 2048, NULL, 2, NULL);
}

QueueHandle_t get_hr_queue(){
    return hr_queue;
}