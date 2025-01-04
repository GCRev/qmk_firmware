#include "quantum.h"
#include "multisteno.h"
#include <hal.h>

void keyboard_post_init_kb(void){
    // set the usb en pin (PB14) to 1
    palSetLineMode(B14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLine(B14);
    
    palSetLineMode(A8, PAL_MODE_ALTERNATE_PUSHPULL);

    palSetLineMode(A0, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A1, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A2, PAL_MODE_ALTERNATE_PUSHPULL);

    pwmStart(&PWMD1, &TIM1_PWM_CONFIG);
    pwmStart(&PWMD2, &TIM2_PWM_CONFIG);

    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));
        
    pwmDisableChannel(&PWMD2, 0);
    pwmDisableChannel(&PWMD2, 1);
    pwmDisableChannel(&PWMD2, 2);

    // pwmDisableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000));
    
    default_led();

    keyboard_post_init_user();
}

void default_led() {
    pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 100));
    pwmEnableChannel(&PWMD2, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 150));
    pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 0));
}