/**
 * @file        input_keyboard.h
 * @brief       Keyboard input provider
 */
#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H
#include "../input.h"

struct sapp_event;

// Keyboard-specific functions
void handle_kb_input_event(const struct sapp_event* event);

void init_keyboard_provider(input_provider* provider);
void cleanup_keyboard_provider(input_provider* provider);

#endif