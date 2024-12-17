#pragma once

#include <hal.h>

static PWMConfig TIM1_PWM_CONFIG = {
    .frequency = 72000000U,
    .period = 0xFFFF,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL}
    }
};

static PWMConfig TIM4_PWM_CONFIG = {
    .frequency = 72000000U,
    .period = 0xFFFF,
    .channels = {
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}
    }
};