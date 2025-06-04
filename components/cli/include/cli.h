/**
 * @file cli.h
 * @brief Smartwatch CLI implementation
 * 
 * @author Fikri
 * @version 0.1
 * @date 2025-06-04
 * 
 * @copyright Copyright (c) 2025 Fikri
 * @license MIT
 */

#ifndef CLI_H
#define CLI_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the CLI interface
 */
void init_cli(void);

/**
 * @brief Process a CLI command
 * @param cmd The command string to process
 */
void process_command(char *cmd);

/**
 * @brief CLI task that handles user input
 * @param pvParameters Task parameters (unused)
 */
void cli_task(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* CLI_H */