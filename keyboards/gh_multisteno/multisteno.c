#include "quantum.h"
#include "multisteno.h"
#include "timer.h"
#include <hal.h>
#include <usb_device_state.h>


static uint16_t rgb_current[3] = {
   0,
   0,
   0 
};

static uint16_t rgb_target[3] = {
   200,
   150,
   200 
};

// static struct usb_device_state usb_device_state;

void keyboard_post_init_kb(void){
    // set the usb en pin (PB14) to 1
    palSetLineMode(B14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLine(B14);
    
    // configure pins to support PWM output
    palSetLineMode(A8, PAL_MODE_ALTERNATE_PUSHPULL);

    palSetLineMode(A0, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A1, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A2, PAL_MODE_ALTERNATE_PUSHPULL);

    pwmStart(&PWMD1, &TIM1_PWM_CONFIG);
    pwmStart(&PWMD2, &TIM2_PWM_CONFIG);

    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));

    // pwmDisableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000));
    
    default_led();

    keyboard_post_init_user();
}

void set_r(uint16_t r) {
    rgb_target[0] = 10000 < r ? 10000 : r;
}
void set_g(uint16_t g) {
    rgb_target[1] = 10000 < g ? 10000 : g;
}
void set_b(uint16_t b) {
    rgb_target[2] = 10000 < b ? 10000 : b;
}

void default_led() {
    set_r(200);
    set_g(150);
    set_b(200);
    pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 200));
    pwmEnableChannel(&PWMD2, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 150));
    pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 200));
}

void disable_led() {
    set_r(0);
    set_g(0);
    set_b(0);
    pwmDisableChannel(&PWMD2, 1);
    pwmDisableChannel(&PWMD2, 2);
    pwmDisableChannel(&PWMD2, 0);
}

// this runs every tick, so use this over led_upate_*
void housekeeping_task_kb(void) {
    static uint32_t previous = 0;
    if (timer_elapsed32(previous) > 16) { // Throttle to 60fps
        previous = timer_read32();
        for(uint8_t i = 0; i < 3; i++) {
            int16_t diff = rgb_target[i] - rgb_current[i];
            diff = diff * 0.6;
            rgb_current[i] = rgb_target[i] - diff;
            PWMD2.tim->CCR[rgb_channel[i]] = PWM_PERCENTAGE_TO_WIDTH(&PWMD2, rgb_current[i]);
        }
    }
}

void notify_usb_device_state_change_kb(struct usb_device_state usb_device_state) {
    notify_usb_device_state_change_user(usb_device_state);
    
    uint16_t status_width = 0;

    switch(usb_device_state.configure_state) {
    case USB_DEVICE_STATE_CONFIGURED:
        status_width = 10000;
        default_led();
        break;
    case USB_DEVICE_STATE_INIT:
    case USB_DEVICE_STATE_SUSPEND:
        status_width = 3000;
        disable_led();
        break;
    default:
        status_width = 0;
        disable_led();
        break;
    }

    PWMD1.tim->CCR[0] = PWM_PERCENTAGE_TO_WIDTH(&PWMD1, status_width);
}