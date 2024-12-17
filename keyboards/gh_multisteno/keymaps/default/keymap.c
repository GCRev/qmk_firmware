// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>
#include QMK_KEYBOARD_H

enum Layer {
    _BASE,
    _STENO
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┐
     * │ 0 │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │
     * ├───┼───┼───┼───┼───┼───┼───┼───┤
     * │ 8 │ 9 │ A │ B │ C │ D │ E │ F │
     * ├───┼───┼───┼───┼───┼───┼───┼───┤
     * │ G │ H │ I │ J │ K │ L │ M │ N │
     * ├───┼───┼───┼───┼───┼───┼───┼───┤
     * │ O │ P │ Q │ R │ S │ T │ U │ V │
     * └───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [_BASE] = LAYOUT(
        KC_0, TO(_STENO), KC_2, KC_3, KC_4, KC_5, KC_6, KC_7,
        KC_8, KC_9, KC_A, KC_B, KC_C, KC_D, KC_E, KC_F,
        KC_G, KC_H, KC_I, KC_J, KC_K, KC_L, KC_M, KC_N,
        KC_O, KC_P, KC_Q, KC_R, KC_S, KC_T, KC_U, KC_V
    ),
    [_STENO] = LAYOUT(
        STN_S1, TO(_BASE), STN_TL, STN_PL, STN_HL, STN_ST1, STN_S2, STN_KL,
        STN_WL, STN_RL, STN_ST2, STN_NA, STN_A, STN_O, STN_E, STN_U,
        STN_NB, STN_FR, STN_PR, STN_LR, STN_TR, STN_DR, STN_RR, STN_BR,
        STN_GR, STN_SR, STN_ZR, _______, _______, _______, _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch(get_highest_layer(state)) {
    case _BASE:
        pwmDisableChannel(&PWMD1, 1);
        pwmDisableChannel(&PWMD1, 2);
        pwmDisableChannel(&PWMD4, 2);
        pwmDisableChannel(&PWMD4, 3);
        break;
    case _STENO:
        pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));
        pwmEnableChannel(&PWMD1, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000));
        pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 10000));
        pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 10000));
        break;
    break;
    }
    
    return state;
}