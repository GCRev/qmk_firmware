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

static bool leds_disabled = true;

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

void default_led() {
    leds_disabled = false;
}

void disable_led() {
    leds_disabled = true;
}

void toggle_led() {
    if (leds_disabled) {
        default_led();
    } else {
        disable_led();
    }
}

// Brightness scale per LED: full at edges, fading toward center
// static const uint8_t led_scale[WS2812_LED_COUNT] = {255, 185, 115, 45, 45, 115, 185, 255};
static const uint8_t led_scale[WS2812_LED_COUNT] = {255, 255, 255, 255, 255, 255, 255, 255};

static uint8_t scale_channel(uint8_t value, uint8_t scale) {
    return (uint16_t)value * scale / 255;
}

bool led_step(void) {
    uint8_t hid_leds = usb_device_state_get_leds();
    // HID LED byte: bit 0 = num lock, bit 1 = caps lock
    bool caps = (hid_leds >> 1) & 1;
    bool num  = (hid_leds >> 0) & 1;

    uint8_t base_r = 0, base_g = 0, base_b = 0;
    if (!leds_disabled) {
        switch (get_highest_layer(layer_state)) {
        case _STENO:
            base_r = 150; base_g = 0;   base_b = 120; break;
        case _FUN:
            base_r = 140; base_g = 90;  base_b = 0;   break;
        case _SYM:
            base_r = 0;   base_g = 80;  base_b = 150; break;
        default:
            base_r = DEFAULT_LED_BRIGHTNESS;
            base_g = DEFAULT_LED_BRIGHTNESS;
            base_b = DEFAULT_LED_BRIGHTNESS;
            break;
        }
    }

    for (uint8_t i = 0; i < WS2812_LED_COUNT; i++) {
        uint8_t tr = base_r, tg = base_g, tb = base_b;

        if (!leds_disabled) {
            if (i == 0 && caps) { tr = 0; tg = 180; tb = 0; }
            if (i == 7 && num)  { tr = 0; tg = 180; tb = 0; }
        }

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

void suspend_power_down_kb(void) {
    static uint32_t previous = 0;
    if (timer_elapsed32(previous) > 25) {
        previous = timer_read32();
        led_step();
    }
    suspend_power_down_user();
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