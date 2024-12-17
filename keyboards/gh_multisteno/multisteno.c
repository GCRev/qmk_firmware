#include "quantum.h"
#include "multisteno.h"
#include <hal.h>

void keyboard_post_init_kb(void){
    // set the usb en pin (PB12) to 1
    palSetLineMode(B12, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLine(B12);

    palSetLineMode(A8, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A9, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A10, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(B8, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(B9, PAL_MODE_ALTERNATE_PUSHPULL);

    pwmStart(&PWMD1, &TIM1_PWM_CONFIG);
    pwmStart(&PWMD4, &TIM4_PWM_CONFIG);

    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));
        
    pwmDisableChannel(&PWMD1, 1);
    pwmDisableChannel(&PWMD1, 2);

    pwmDisableChannel(&PWMD4, 2);
    pwmDisableChannel(&PWMD4, 3);

    // pwmDisableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000));

    keyboard_post_init_user();
}