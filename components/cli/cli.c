/**
 * @file cli.c
 * @brief Smartwatch CLI implementation
 * 
 * @author Fikri
 * @version 0.1
 * @date 2025-06-04
 * 
 * @based_on ESP-IDF v5.5-dev-3618-g7f6e7f4506 
 *           (https://github.com/espressif/esp-idf/tree/7f6e7f4506)
 * @license MIT
 * 
 * @copyright Copyright (c) 2025 Fikri
 * 
 */

#include "cli.h"
#include "step_counter.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>
#include "datetime.h"

#define CLI_BUFFER_SIZE 128
static const char *TAG = "CLI";

void init_cli(void) {
    // Configure UART for basic CLI
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config);
    uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, CLI_BUFFER_SIZE * 2, 0, 0, NULL, 0);
    
    ESP_LOGI(TAG, "CLI initialized");
}

void process_command(char *cmd) {
    if (strcmp(cmd, "reset-steps") == 0) {
        reset_steps();
        printf("Steps reset to 0\n");
    } 
    else if (strcmp(cmd, "help") == 0) {
        printf("Available commands:\n");
        printf("reset-steps - Reset step counter\n");
        printf("help - Show this help message\n");
    }
    else if (strcmp(cmd, "time") == 0) {
    smartwatch_time_t t = get_current_time();
    printf("Current: %02d:%02d:%02d\n", 
           t.timeinfo.tm_hour,
           t.timeinfo.tm_min,
           t.timeinfo.tm_sec);
    }
    else if (strncmp(cmd, "settime ", 8) == 0) {
        struct tm new_time = {0};
        sscanf(cmd + 8, "%d:%d:%d", 
            &new_time.tm_hour,
            &new_time.tm_min,
            &new_time.tm_sec);
        set_time(new_time);
    }
    else {
        printf("Unknown command. Type 'help' for options\n");
    }
}

void cli_task(void *pvParameters) {
    uint8_t data[CLI_BUFFER_SIZE];
    int len = 0;
    
    printf("\nSmartwatch CLI Ready\n");
    printf("Type 'help' for command list\n> ");
    
    while(1) {
        // Read character from UART
        int bytes_read = uart_read_bytes(CONFIG_ESP_CONSOLE_UART_NUM, &data[len], 1, pdMS_TO_TICKS(10));
        
        if (bytes_read > 0) {
            // Handle Enter key
            if (data[len] == '\r' || data[len] == '\n') {
                if (len > 0) {
                    data[len] = '\0'; // Null-terminate
                    printf("\n");
                    process_command((char *)data);
                    len = 0;
                    printf("> ");
                }
            } 
            // Handle backspace
            else if (data[len] == '\b') {
                if (len > 0) {
                    len--;
                    printf("\b \b"); // Erase character
                }
            }
            // Normal character
            else {
                printf("%c", data[len]); // Echo character
                len = (len + 1) % CLI_BUFFER_SIZE;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}