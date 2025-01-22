// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>
#include "multisteno.h"
#include QMK_KEYBOARD_H

enum Layer {
    _BASE,
    _STENO,
    _FUN,
    _SYM
};

enum custom_keycodes {
    STN_ESC = SAFE_RANGE,
    TO_STN,
    MO_FUN
};

// left-handed space combo
const uint16_t PROGMEM lh_space_combo[] = { MO_FUN, KC_LSFT, COMBO_END };

// left-handed sym access (for numpad with left hand)
const uint16_t PROGMEM lh_sym_combo[] =   { TO_STN, KC_Z,    COMBO_END };
combo_t key_combos[] = {
    COMBO(lh_space_combo, KC_SPC),
    COMBO(lh_sym_combo,   TG(_SYM))
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* _BASE
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ESC│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │BKS│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │TAB│ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ \ │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │CTR│ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │STN│ Z │ X │ C │ V │ B │ N │ M │ , │ . │ ? │ENT│
     * └───┴───┴───┼───┼───┼───┼───┼───┼───┼───┴───┴───┘
     *             │ALT│FUN│SFT│SPC│SYM│WIN│
     *             └───┴───┴───┴───┴───┴───┘
     */
    [_BASE] = LAYOUT(
        KC_ESC,     KC_1,    KC_2,     KC_3,    KC_4,     KC_5,    KC_6,   KC_7,     KC_8,    KC_9,   KC_0,    KC_BSPC,
        KC_TAB,     KC_Q,    KC_W,     KC_E,    KC_R,     KC_T,    KC_Y,   KC_U,     KC_I,    KC_O,   KC_P,    KC_BSLS,
        KC_LCTL,    KC_A,    KC_S,     KC_D,    KC_F,     KC_G,    KC_H,   KC_J,     KC_K,    KC_L,   KC_SCLN, KC_QUOT,
        TO_STN,     KC_Z,    KC_X,     KC_C,    KC_V,     KC_B,    KC_N,   KC_M,     KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
                                       KC_LALT, MO_FUN,   KC_LSFT, KC_SPC, MO(_SYM), KC_LWIN
    ),

    /* _STENO
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ESC│   │   │   │   │   │   │   │   │   │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │ S │ T │ P │ H │ * │ * │ F │ P │ L │ T │ D │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │ S │ K │ W │ R │ * │ * │ R │ B │ G │ S │ Z │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │QWE│   │   │   │   │   │   │   │   │   │   │   │
     * └───┴───┴───┼───┼───┼───┼───┼───┼───┼───┴───┴───┘
     *             │ # │ A │ O │ E │ U │ # │
     *             └───┴───┴───┴───┴───┴───┘
     */
    [_STENO] = LAYOUT(
        STN_ESC,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX,   STN_S1,  STN_TL,  STN_PL,  STN_HL,  STN_ST1, STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR,  STN_DR,
        XXXXXXX,   STN_S2,  STN_KL,  STN_WL,  STN_RL,  STN_ST2, STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR,  STN_ZR,
        TO(_BASE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  
                                     STN_N1,  STN_A,   STN_O,   STN_E,   STN_U,   STN_N2
    ),

    /* _FUN
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │ F1│ F2│ F3│ F4│ F5│ F6│ F7│ F8│ F9│F10│F11│F12│
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │ UP│   │   │   │   │DEL│INS│   │PRT│   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │LFT│DWN│RGT│   │   │HOM│PGD│PGU│END│   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │DEL│CAP│   │BRK│   │   │   │   │   │   │
     * └───┴───┴───┼───┼───┼───┼───┼───┼───┼───┴───┴───┘
     *             │   │   │   │   │   │   │
     *             └───┴───┴───┴───┴───┴───┘
     */
    [_FUN] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, _______, KC_UP,   _______, _______, _______, _______, KC_DEL,  KC_INS,  _______, KC_PSCR, _______,
        _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,
        _______, _______, KC_DEL,  KC_CAPS, _______, KC_BRK,  _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______, _______, _______
    ),

    /* _SYM
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
     * │TLD│   │NLK│ / │ * │ - │   │   │ ( │ ) │   │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │GRV│   │ 7 │ 8 │ 9 │ + │   │ _ │ { │ } │ = │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │ 4 │ 5 │ 6 │ + │   │ - │ [ │ ] │ + │   │
     * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     * │   │   │ 1 │ 2 │ 3 │ENT│   │   │ < │ > │   │   │
     * └───┴───┴───┼───┼───┼───┼───┼───┼───┼───┴───┴───┘
     *             │ 0 │ . │   │   │   │   │
     *             └───┴───┴───┴───┴───┴───┘
     */
    [_SYM] = LAYOUT(
        KC_TILD, _______, KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS, _______, _______, KC_LPRN, KC_RPRN, _______, _______,
        KC_GRV,  _______, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, _______, KC_UNDS, KC_LCBR, KC_RCBR, KC_EQL,  _______,
        _______, _______, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, _______, KC_MINS, KC_LBRC, KC_RBRC, KC_PLUS, _______,
        _______, _______, KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______, _______, KC_LT,   KC_GT,   _______, _______,
                                   KC_P0,   KC_PDOT, _______, _______, _______, _______
    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
    switch(get_highest_layer(state)) {
    case _STENO:
        // poorple
        set_r(2500);
        set_g(0);
        set_b(4000);
        break;
    case _FUN:
        // yellow 
        set_r(4000);
        set_g(1200);
        set_b(0);
        break;
    case _SYM:
        // cyan
        set_r(0);
        set_g(1200);
        set_b(4000);
        break;
    default:
        set_r(200);
        set_g(150);
        set_b(200);
        break;
    }
    
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch(keycode) {
    case STN_ESC:
        // run immediately only press
        if (record->event.pressed) {
            tap_code(KC_ESC);
            layer_move(_BASE);
        }
        return false;
    case MO_FUN:
        if (record->event.pressed) {
            layer_on(_FUN);
        } else {
            layer_off(_FUN);
        }
        return false;
    case TO_STN:
        if (record->event.pressed) {
            layer_move(_STENO);
        } else {
            // do nothing
        }
        return false;
    default:
        return true;
    }
}