/**
 * @file heap_tracker.h
 * @brief Memory monitoring for Smartwatch
 * 
 * @author Fikri
 * 
 * @copyright Copyright (c) 2025 Fikri
 * 
 * @note Key Features:
 * - Memory leak detection capabilities
 * 
 * @attention Enable CONFIG_HEAP_TRACING in menuconfig for full diagnostics
 */
#pragma once

#include "esp_heap_caps.h"
#include "esp_system.h"
#include "esp_log.h"
#include <stdio.h>

/**
 * @brief Prints comprehensive memory statistics
 * 
 * Displays current memory status including:
 * - Free internal memory
 * - Largest contiguous block size
 * - Minimum ever free heap (low watermark)
 * - SPIRAM status (if configured)
 * 
 * @note Logs output via ESP_LOGI with "MEM" tag
 * @example print_memory_stats(); // Logs: [MEM] Internal Free: 12345 bytes
 */
void print_memory_stats(void);