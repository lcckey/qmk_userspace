// Copyright 2022 Manna Harbour
// https://github.com/manna-harbour/miryoku

// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

#include QMK_KEYBOARD_H

#include "manna-harbour_miryoku.h"


// Additional Features double tap guard

enum {
    U_TD_BOOT,
#define MIRYOKU_X(LAYER, STRING) U_TD_U_##LAYER,
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};

void u_td_fn_boot(tap_dance_state_t *state, void *user_data) {
  if (state->count == 2) {
    reset_keyboard();
  }
}

#define MIRYOKU_X(LAYER, STRING) \
void u_td_fn_U_##LAYER(tap_dance_state_t *state, void *user_data) { \
  if (state->count == 2) { \
    default_layer_set((layer_state_t)1 << U_##LAYER); \
  } \
}
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X

#if !defined (VIAL_ENABLE)
tap_dance_action_t tap_dance_actions[] = {
    [U_TD_BOOT] = ACTION_TAP_DANCE_FN(u_td_fn_boot),
#define MIRYOKU_X(LAYER, STRING) [U_TD_U_##LAYER] = ACTION_TAP_DANCE_FN(u_td_fn_U_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};
#endif


// keymap

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
#define MIRYOKU_X(LAYER, STRING) [U_##LAYER] = U_MACRO_VA_ARGS(MIRYOKU_LAYERMAPPING_##LAYER, MIRYOKU_LAYER_##LAYER),
MIRYOKU_LAYER_LIST
#undef MIRYOKU_X
};


// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t *key_overrides[] = {
    &capsword_key_override
};


// thumb combos

#if defined (MIRYOKU_KLUDGE_THUMBCOMBOS)
const uint16_t PROGMEM thumbcombos_base_right[] = {LT(U_SYM, KC_ENT), LT(U_NUM, KC_BSPC), COMBO_END};
const uint16_t PROGMEM thumbcombos_base_left[] = {LT(U_NAV, KC_SPC), LT(U_MOUSE, KC_TAB), COMBO_END};
const uint16_t PROGMEM thumbcombos_nav[] = {KC_ENT, KC_BSPC, COMBO_END};
const uint16_t PROGMEM thumbcombos_mouse[] = {KC_BTN2, KC_BTN1, COMBO_END};
const uint16_t PROGMEM thumbcombos_media[] = {KC_MSTP, KC_MPLY, COMBO_END};
const uint16_t PROGMEM thumbcombos_num[] = {KC_0, KC_MINS, COMBO_END};
  #if defined (MIRYOKU_LAYERS_FLIP)
const uint16_t PROGMEM thumbcombos_sym[] = {KC_UNDS, KC_LPRN, COMBO_END};
  #else
const uint16_t PROGMEM thumbcombos_sym[] = {KC_RPRN, KC_UNDS, COMBO_END};
  #endif
const uint16_t PROGMEM thumbcombos_fun[] = {KC_SPC, KC_TAB, COMBO_END};
combo_t key_combos[] = {
  COMBO(thumbcombos_base_right, LT(U_FUN, KC_DEL)),
  COMBO(thumbcombos_base_left, LT(U_MEDIA, KC_ESC)),
  COMBO(thumbcombos_nav, KC_DEL),
  COMBO(thumbcombos_mouse, KC_BTN3),
  COMBO(thumbcombos_media, KC_MUTE),
  COMBO(thumbcombos_num, KC_DOT),
  #if defined (MIRYOKU_LAYERS_FLIP)
  COMBO(thumbcombos_sym, KC_RPRN),
  #else
  COMBO(thumbcombos_sym, KC_LPRN),
  #endif
  COMBO(thumbcombos_fun, KC_APP)
};
#endif

#ifdef RGB_MATRIX_ENABLE
# ifdef LAYOUT_split_3x5_3_ex2
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t LED_LIST[] = {8,7,0,20,27,28};
    switch (get_highest_layer(layer_state|default_layer_state)) {
    case U_EXTRA:
        rgb_matrix_set_color(38, RGB_BLUE); break;
    case U_TAP:
        rgb_matrix_set_color(18, RGB_GREEN); break;
    case U_EXTRATAP:
        rgb_matrix_set_color(18, RGB_BLUE);
        rgb_matrix_set_color(38, RGB_BLUE); break;
    case U_MEDIA:
        if (get_highest_layer(default_layer_state) != 4) {
            LED_LIST[0] = 6;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_PURPLE);
        };
        break;
    case U_NAV:
        if (get_highest_layer(default_layer_state) != 5) {
            LED_LIST[1] = 6;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_CYAN);
        };
        break;
    case U_MOUSE:
        if (get_highest_layer(default_layer_state) != 6) {
            LED_LIST[2] = 6;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_YELLOW);
        };
        break;
    case U_SYM:
        if (get_highest_layer(default_layer_state) != 7) {
            LED_LIST[3] = 26;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_GREEN);
        };
        break;
    case U_NUM:
        if (get_highest_layer(default_layer_state) != 8) {
            LED_LIST[4] = 26;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_BLUE);
        };
        break;
    case U_FUN:
        if (get_highest_layer(default_layer_state) != 9) {
            LED_LIST[5] = 26;
        }
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_RED);
        };
        break;
    case U_BUTTON:
        for (uint8_t i = 0; i < sizeof(LED_LIST); i++) {
            rgb_matrix_set_color(LED_LIST[i], RGB_ORANGE);
        };
        break;
    default: // for any other layer, set indicators to off
        rgb_matrix_set_color(18, RGB_BLACK);
        rgb_matrix_set_color(19, RGB_BLACK);
        rgb_matrix_set_color(38, RGB_BLACK);
        rgb_matrix_set_color(39, RGB_BLACK); break;
    }
    if (get_highest_layer(default_layer_state|layer_state) > 3) {
        switch(get_highest_layer(default_layer_state)) {
        case 0:
            rgb_matrix_set_color(4, RGB_GREEN); break;
            rgb_matrix_set_color(23, RGB_GREEN); break;
        case 1:
            rgb_matrix_set_color(12, RGB_GREEN); break;
            rgb_matrix_set_color(31, RGB_GREEN); break;
        case 2:
            rgb_matrix_set_color(11, RGB_BLUE); break;
            rgb_matrix_set_color(24, RGB_BLUE); break;
        case 3:
            rgb_matrix_set_color(17, RGB_BLUE); break;
            rgb_matrix_set_color(32, RGB_BLUE); break;
        }
    }
    return false;
}
# endif
#endif
