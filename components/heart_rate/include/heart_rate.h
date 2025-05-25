#ifndef HEART_RATE_H
#define HEART_RATE_H

#include "freertos/event_groups.h"

void init_hr_sensor(EventGroupHandle_t state_group);
QueueHandle_t get_hr_queue();

//Event group bits
#define WORKOUT_BIT (1 << 0) // Bit 0

#endif