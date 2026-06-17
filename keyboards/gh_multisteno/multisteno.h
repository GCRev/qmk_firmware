#pragma once

#include <hal.h>
#include <usb_device_state.h>

enum Layer {
    _BASE,
    _STENO,
    _FUN,
    _SYM
};

static const PWMConfig TIM1_PWM_CONFIG = {
    .frequency = 72000000U,
    .period = 0xFFFF,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL}
    }
};

/*
static const PWMConfig TIM2_PWM_CONFIG = {
    .frequency = 72000000U,
    .period = 0xFFFF,
    .channels = {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_ACTIVE_LOW, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    }
};
*/

static const uint8_t rgb_channel[3] = {
    1,
    2,
    0
};

#define DEFAULT_LED_BRIGHTNESS 60

void default_led(void);

void disable_led(void);

void toggle_led(void);