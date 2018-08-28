#include <stdarg.h>
#include <stdio.h>
#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "config.h"
#include "quantum.h"
#include "led.h"
#include "timer.h"
#include "print.h"

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM, // Initialize the keyboard
  VRSN, // Print the keyboard version
  RGB_SLD, // Toggle RGB backlighting
  KEYLOG, // Toggle keylogger

  // RGB Presets defined in process_record_user
  RGBPRESET_1,
  RGBPRESET_2,
  RGBPRESET_3,
  RGBPRESET_4,
  RGBPRESET_5

};

enum layers {
  BASE = 0,
  CHARNUM,
  ARRSUBL,
  MEDIA
};

#define ____ KC_TRNS

#ifdef KEYLOGGER_ENABLE
bool logging_enabled = false;
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // Base layer
  [BASE] = LAYOUT_ergodox(
      // left hand
      KC_ESC,            KC_1,           KC_2,       KC_3,        KC_4,       KC_5,  KC_CAPSLOCK,
      KC_TAB,            KC_Q,           KC_W,       KC_E,        KC_R,       KC_T,  ____,
      LT(2,KC_ESC),      KC_A,           KC_S,       KC_D,        KC_F,       KC_G,
      OSM(MOD_LSFT),     KC_Z,           KC_X,       KC_C,        KC_V,       KC_B,  KC_BSLS,
      OSM(MOD_LCTL),     ALT_T(KC_BSLS), ALT_CMD,    KC_LEFT,     KC_RIGHT,
                                                                              ____,  TT(MEDIA),
                                                                                          ____,
                                                        OSL(CHARNUM), GUI_T(KC_TAB),KC_DELETE,


      // right hand
      KC_ESC,  KC_6,       KC_7,       KC_8,       KC_9,     KC_0,            KC_BSPC,
      ____,    KC_Y,       KC_U,       KC_I,       KC_O,     KC_P,            KC_QUOT,
               KC_H,       KC_J,       KC_K,       KC_L,     LT(2,KC_SCOLON), RGUI_T(KC_ENTER),
      KC_MINS, KC_N,       KC_M,       KC_COMMA,   KC_DOT,   RCTL_T(KC_SLASH),OSM(MOD_RSFT),
                           KC_UP,      KC_DOWN,    KC_LBRC,  KC_RBRC,         TG(2),
      ____,    ____,
      ____,
      KC_BSPC,    GUI_T(KC_ENTER),   KC_SPACE),

  // Numpad, function keys, and shifted punctuation
  [CHARNUM] = LAYOUT_ergodox(
    // left hand
    KC_ESC, KC_F1,   KC_F2,   KC_F3,     KC_F4,    KC_F5,    ____,
    ____,   KC_ASTR, KC_SLASH,KC_LCBR,   KC_RCBR,  KC_PIPE,  ____,
    ____,   KC_PLUS, KC_EQL,  KC_LPRN,   KC_RPRN,  KC_GRV,
    ____,   KC_MINS, KC_UNDS, KC_LBRC,   KC_RBRC,  KC_TILD,  ____,
    ____,   ____,    ____,    ____,      ____,
                                                   ____,     ____,
                                                             ____,
                                         ____,     ____,     ____,

    // right hand
    ____,  KC_F6,   KC_F7,  KC_F8,  KC_F9,  KC_F10,    KC_F11,
    ____,  KC_AMPR, KC_7,   KC_8,   KC_9,   KC_ASTR,   KC_F12,
           KC_QUOT, KC_4,   KC_5,   KC_6,   KC_PLUS,   ____,
    ____,  KC_DQT,  KC_1,   KC_2,   KC_3,   KC_BSLS,   ____,
                    KC_EQL, KC_0,   ____,   KC_MINS,   ____,
    ____,  ____,
    ____,
    ____,  ____,    ____),

  // Arrow keys and sublime navigation shortcuts
  [ARRSUBL] = LAYOUT_ergodox(
    // left hand
    ____, ____,          ____,         ____,        ____,          ____,          ____,
    ____, ____,          ____,         LCMD(KC_UP), ____,          ____,          ____,
    ____, LCMD(KC_LEFT),LALT(KC_LEFT), LCMD(KC_D),  LALT(KC_RGHT), LCMD(KC_RGHT),
    ____, LCMD(KC_Z),   LCMD(KC_X),    LCMD(KC_C),  LCMD(KC_V),    ____,          ____,
    ____, ____,         ____,          ____,        ____,
                                                                   ____,          ____,
                                                                                  ____,
                                                    ____,          ____,          ____,
    // right hand
    ____, ____,       ____,    ____,     ____, ____, ____,
    ____, ____,       ____,   KC_UP,     ____, ____, ____,
          KC_PGUP, KC_LEFT, KC_DOWN, KC_RIGHT, ____, ____,
    ____, KC_PGDN,    ____,    ____,     ____, ____, ____,
                      ____,    ____,  KC_MUTE, ____, ____,
    ____, ____,
    ____,
    ____, ____, ____),

  // Media and RGB keys
  [MEDIA] = LAYOUT_ergodox(
    // left hand
    KEYLOG ,RGBPRESET_1,RGBPRESET_2,RGBPRESET_3,RGBPRESET_4,____, ____,
    ____,   ____,____,____,____,____, ____,
    ____,   ____,____,____,____,____,
    ____,   ____,____,____,____,____, ____,
    ____,   ____,____,____,____,
                                ____, ____,
                                      ____,
                           ____,____, ____,
    // right hand
    RGB_TOG, RGB_VAI, RGB_VAD, RGB_SAI, RGB_SAD, RGB_HUI, RGB_HUD,
    ____,    ____,    ____,    ____, ____,    ____, RGB_MOD,
             ____,    ____,    ____, KC_RIGHT,____, KC_MPLY,
    ____,    ____,    ____, KC_MPRV, KC_MNXT, ____, ____,
                   KC_VOLU, KC_VOLD, KC_MUTE, ____, ____,
    ____,    ____,
    ____,
    ____,    ____, KC_PGDN
  )

};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
      }
    return MACRO_NONE;
};

void matrix_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
  rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef KEYLOGGER_ENABLE
  if (logging_enabled){
    if (record->event.pressed) {
      uint8_t layer = biton32(layer_state);
      char *layerStr = NULL;

      switch (layer) {
        case BASE:
          layerStr = "BASE";
          break;
        case CHARNUM:
          layerStr = "CHARNUM";
          break;
        case ARRSUBL:
          layerStr = "ARRSUBL";
          break;
      }

      if (layerStr) {
        xprintf("KL: col=%02d, row=%02d, pressed=%d, layer=%s\n", record->event.key.col,
               record->event.key.row, record->event.pressed, layerStr);
      }
    }
  }

  #endif
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
      break;

    #ifdef KEYLOGGER_ENABLE
    case KEYLOG:
      if (record->event.pressed) {
        ergodox_led_all_on();
        wait_ms(100);
        ergodox_led_all_off();
        wait_ms(50);
        logging_enabled = !logging_enabled;
        if (logging_enabled) {
          print("Keylogger started\n");
          ergodox_led_all_on();
          wait_ms(100);
          ergodox_led_all_off();
        } else {
          print("Keylogger stopped\n");
        }
      }
      return false;
      break;
    #endif

    #ifdef RGBPRESET_ENABLE
    // Knight rider
    case RGBPRESET_1:
      rgblight_enable();
      rgblight_mode(21);
      return false;
      break;
    // Cycling rainbow
    case RGBPRESET_2:
      rgblight_enable();
      rgblight_mode(6);
      return false;
      break;
    // Swirling rainbow
    case RGBPRESET_3:
      rgblight_enable();
      rgblight_mode(9);
      return false;
      break;
    // Breathing
    case RGBPRESET_4:
      rgblight_enable();
      rgblight_mode(2);
      return false;
      break;
    #endif
  }
  return true;
}

uint32_t layer_state_set_user(uint32_t state) {

    uint8_t layer = biton32(state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      case 0:
        #ifdef RGBLIGHT_COLOR_LAYER_0
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
        #endif
        break;
      case 1:
        ergodox_right_led_1_on();
        #ifdef RGBLIGHT_COLOR_LAYER_1
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
        #endif
        break;
      case 2:
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_2
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
        #endif
        break;
      case 3:
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_3
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
        #endif
        break;
      case 4:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        #ifdef RGBLIGHT_COLOR_LAYER_4
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
        #endif
        break;
      case 5:
        ergodox_right_led_1_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_5
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
        #endif
        break;
      case 6:
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_6
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
        #endif
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        #ifdef RGBLIGHT_COLOR_LAYER_7
          rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
        #endif
        break;
      default:
        break;
    }
    return state;

};
