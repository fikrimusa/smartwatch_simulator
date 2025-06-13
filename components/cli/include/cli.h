/**
 * @file cli.h
 * @brief Smartwatch Command Line Interface Implementation
 *
 * Provides interactive command processing for smartwatch functions including:
 * - Step counter control and monitoring
 * - System configuration
 * - Real-time clock operations
 * - Debugging utilities
 *
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 *
 * @note Features:
 * - UART-based command input
 * - Real-time command processing
 * - Integrated with step counter and RTC modules
 * - Thread-safe operation using FreeRTOS primitives
 *
 * @warning Requires UART driver initialization prior to use
 * @attention Set correct UART pins in menuconfig
 */
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "step_counter.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>
#include "datetime.h"

/**
 * @brief CLI task that handles user input
 * @param pvParameters Task parameters (unused)
 */
void cli_task(void *pvParameters);

/**
 * @brief Initialize the CLI interface
 */
void init_cli(void);

/**
 * @brief Process a CLI command
 * @param cmd The command string to process
 */
void process_command(char *cmd);