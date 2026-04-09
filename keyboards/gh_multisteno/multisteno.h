#pragma once

#include <hal.h>
#include <usb_device_state.h>

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

static const PWMConfig TIM2_PWM_CONFIG = {
    .frequency = 72000000U,
    .period = 0xFFFF,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
    }
};

static const uint8_t rgb_channel[3] = {
    1,
    2,
    0
};

#ifndef BRIGHTNESS
#define BRIGHTNESS 1.0
#endif

void set_r(uint16_t r);
void set_g(uint16_t g);
void set_b(uint16_t b);

void default_led(void);

void disable_led(void);

bool led_step(void);