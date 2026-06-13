#include "quantum.h"
#include "multisteno.h"
#include "timer.h"
#include "wait.h"
#include "ws2812.h"
#include "config.h"
#include <hal.h>
#include <usb_device_state.h>

bool led_step(void);

static uint8_t rgb_current[3 * WS2812_LED_COUNT] = {0};
static uint8_t rgb_target[3 * WS2812_LED_COUNT] = {0};

// static struct usb_device_state usb_device_state;

void keyboard_post_init_kb(void){
    // set the usb en pin (PB14) to 1
    palSetLineMode(B14, PAL_MODE_OUTPUT_PUSHPULL);
    palSetLine(B14);
    
    // configure pins to support PWM output
    palSetLineMode(A8, PAL_MODE_ALTERNATE_PUSHPULL);

    /*
    palSetLineMode(A0, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A1, PAL_MODE_ALTERNATE_PUSHPULL);
    palSetLineMode(A2, PAL_MODE_ALTERNATE_PUSHPULL);
    */

    pwmStart(&PWMD1, &TIM1_PWM_CONFIG);
    // pwmStart(&PWMD2, &TIM2_PWM_CONFIG);

    pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));
    
    ws2812_init();
    // WS2812_PWM_DRIVER.tim->CCER |= (1U << 9);
    // WS2812_PWM_DRIVER.tim->CCMR2 |= (1U << 4);
    default_led();

    keyboard_post_init_user();
}

void set_r(uint8_t r) {
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        rgb_target[i * 3 + 0] = r;
    }
}

void set_g(uint8_t g) {
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        rgb_target[i * 3 + 1] = g;
    }
}

void set_b(uint8_t b) {
    for (int i = 0; i < WS2812_LED_COUNT; i++) {
        rgb_target[i * 3 + 2] = b;
    }
}

void set_r_at(int index, uint8_t r) {
    rgb_target[index * 3 + 0] = r;
}

void set_g_at(int index, uint8_t g) {
    rgb_target[index * 3 + 1] = g;
}

void set_b_at(int index, uint8_t b) {
    rgb_target[index * 3 + 2] = b;
}

void default_led() {
    memset(rgb_target, 100, sizeof(rgb_target));
    led_step();
}

void disable_led() {
    memset(rgb_target, 0, sizeof(rgb_target));
    led_step();
}

// Brightness scale per LED: full at edges, fading toward center
// static const uint8_t led_scale[WS2812_LED_COUNT] = {255, 185, 115, 45, 45, 115, 185, 255};
static const uint8_t led_scale[WS2812_LED_COUNT] = {255, 255, 255, 255, 255, 255, 255, 255};

static uint8_t scale_channel(uint8_t value, uint8_t scale) {
    return (uint16_t)value * scale / 255;
}

bool led_step(void) {
    uint8_t leds = usb_device_state_get_leds();
    // HID LED byte: bit 0 = num lock, bit 1 = caps lock
    bool caps = (leds >> 1) & 1;
    bool num  = (leds >> 0) & 1;

    for (uint8_t i = 0; i < WS2812_LED_COUNT; i++) {
        uint8_t tr = rgb_target[i * 3 + 0];
        uint8_t tg = rgb_target[i * 3 + 1];
        uint8_t tb = rgb_target[i * 3 + 2];

        if (i == 0 && caps) { tr = 0; tg = 180; tb = 0; }
        if (i == 7 && num)  { tr = 0; tg = 180; tb = 0; }

        uint8_t s = led_scale[i];
        tr = scale_channel(tr, s);
        tg = scale_channel(tg, s);
        tb = scale_channel(tb, s);

        rgb_current[i * 3 + 0] += (tr - rgb_current[i * 3 + 0]) >> 2;
        rgb_current[i * 3 + 1] += (tg - rgb_current[i * 3 + 1]) >> 2;
        rgb_current[i * 3 + 2] += (tb - rgb_current[i * 3 + 2]) >> 2;

        ws2812_set_color(i, rgb_current[i * 3 + 0], rgb_current[i * 3 + 1], rgb_current[i * 3 + 2]);
    }
    ws2812_flush();
    return true;
}

// this runs every tick, so use this over led_upate_*
void housekeeping_task_kb(void) {
    static uint32_t previous = 0;
    if (timer_elapsed32(previous) > 16) { // Throttle to 60fps
        previous = timer_read32();
        led_step();
    }
}

void notify_usb_device_state_change_kb(struct usb_device_state usb_device_state) {
    notify_usb_device_state_change_user(usb_device_state);
    
    uint16_t status_width = 0;
    static usb_configure_state_t last_state = USB_DEVICE_STATE_NO_INIT;

    if(last_state == usb_device_state.configure_state) {
        return;
    }

    last_state = usb_device_state.configure_state;

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