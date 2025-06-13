/**
 * @file cli.c
 * @brief Smartwatch Command Line Interface Implementation
 *
 * Implements an interactive UART-based CLI for smartwatch control and monitoring.
 * Supports:
 * - Step counter management
 * - Time/date operations
 * - System information
 *
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 *
 * @note Hardware Requirements:
 * - UART0 (default) or configured UART port
 * - 115200 baud rate (configurable)
 *
 * @warning Ensure proper UART GPIO configuration in menuconfig
 */
#include "cli.h"

#define CLI_BUFFER_SIZE 128
static const char *TAG = "CLI";

void cli_task(void *pvParameters) {
    static uint8_t data[CLI_BUFFER_SIZE];
    int len = 0;
    
    // Stack monitoring initialization
    UBaseType_t watermark = uxTaskGetStackHighWaterMark(NULL);
    ESP_LOGI(TAG, "Initial stack: %u bytes", watermark);
    
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
        
        // Periodic stack check (every 100 iterations)
        static int check_counter = 0;
        if (++check_counter >= 100) {
            check_counter = 0;
            watermark = uxTaskGetStackHighWaterMark(NULL);
            if (watermark < 512) { // Warning threshold
                ESP_LOGW(TAG, "Low stack: %u bytes remaining", watermark);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void init_cli(void) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    const int rx_buffer_size = 256;
    const int tx_buffer_size = 0;
    
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config));
    
    // Install UART driver with valid buffer sizes
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 
                                      rx_buffer_size,
                                      tx_buffer_size,
                                      0,      // Queue size (0 to disable)
                                      NULL,   // Queue handle
                                      0));    // Interrupt flags
}

void process_command(char *cmd) {
    // Use strtok_r instead of strcmp for better memory handling
    char *saveptr;
    char *token = strtok_r(cmd, " ", &saveptr);
    
    if (token == NULL) return;
    
    if (strcmp(token, "reset-steps") == 0) {
        reset_steps();
        printf("Steps reset to 0\n");
    }
    else if (strcmp(token, "help") == 0) {
        printf("Available commands:\n");
        printf("reset-steps - Reset step counter\n");
        printf("help - Show this help message\n");
    }
    else if (strcmp(token, "time") == 0) {
        smartwatch_time_t t = get_current_time();
        printf("Current: %02d:%02d:%02d\n", 
               t.timeinfo.tm_hour,
               t.timeinfo.tm_min,
               t.timeinfo.tm_sec);
    }
    else if (strcmp(token, "settime") == 0) {
        token = strtok_r(NULL, " ", &saveptr);
        if (token) {
            struct tm new_time = {0};
            if (sscanf(token, "%d:%d:%d", 
                      &new_time.tm_hour,
                      &new_time.tm_min,
                      &new_time.tm_sec) == 3) {
                set_time(new_time);
            } else {
                printf("Invalid time format\n");
            }
        }
    }
    else {
        printf("Unknown command. Type 'help' for options\n");
    }
}