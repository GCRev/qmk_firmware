#pragma once

#include <hal.h>

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

void default_led(void);