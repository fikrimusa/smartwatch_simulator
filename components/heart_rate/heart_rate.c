/**
 * @file heart_rate.c
 * @brief FreeRTOS-based heart rate sensor simulation for smartwatch
 * 
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 * 
 * @note This implementation simulates:
 *       - Resting heart rate (60-100 BPM)
 *       - Workout heart rate (80-140 BPM)
 */
#include "heart_rate.h"

static QueueHandle_t hr_queue;
static EventGroupHandle_t state_group;

/**
 * @brief Simulates heart rate detection and sends values to a queue
 *
 * This task generates simulated heart rate values in beats per minute (BPM):
 * - In normal state: generates random values between 60-100 BPM
 * - When WORKOUT_BIT is set in state_group: generates higher random values between 80-140 BPM
 * 
 * The generated heart rate value is sent to hr_queue every second (1000ms).
 * 
 * @param pvParam Task parameters (unused in this implementation)
 */
static void hr_simulation_task(void *pvParam){
    while(1){
        int hr = 60 + (rand() % 40); // Base 60-100 BPM

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
    xTaskCreate( hr_simulation_task, "HR", 2048, NULL, 2, NULL);
}

QueueHandle_t get_hr_queue(){
    return hr_queue;
}