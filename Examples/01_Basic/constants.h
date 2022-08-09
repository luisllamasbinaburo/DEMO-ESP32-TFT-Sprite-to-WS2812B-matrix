#pragma once

const uint8_t LED_MATRIX_HEIGHT = 16;
const uint8_t LED_MATRIX_WIDTH = 16;
const uint16_t LED_MATRIX_NUM_LEDS = LED_MATRIX_WIDTH * LED_MATRIX_HEIGHT;

#define COLOR_ORDER GRB
#define CHIPSET WS2812

#define DATA_PIN 26

#define BRIGHTNESS 10