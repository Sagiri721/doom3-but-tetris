/**
 * @file        time.c
 * @brief       Time utils
 */

#include "utils.h"

#include <time.h>

uint32_t now_ms() {
    // Use clock monotonic since this'll be used to measure network round trips
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}