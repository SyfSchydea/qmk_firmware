#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_uk.h"

enum layers {
	BASE,  // default layer
	STNO,  // Steno Layer

	SYMB,  // symbols
	MDIA,  // media keys

	SPEC,  // Special functions and layer switches
};

typedef struct {
    uint16_t tap;
    uint16_t hold;
    bool     held;
} tap_dance_tap_hold_t;

enum custom_keycodes {
	VRSN = SAFE_RANGE,
};

enum tapdance_keys {
	TD_BASE_SPEC,
};

tap_dance_action_t tap_dance_actions[] = {
	[TD_BASE_SPEC] = {
		.fn = {
			NULL, // on each tap
			td_base_spec_fin,
			td_base_spec_reset,
			td_base_spec_release,
		},
		.user_data = (void*) &((tap_dance_tap_hold_t) { TO(BASE), MO(SPEC), false })
};

#define BASE_SPEC TD(TD_BASE_SPEC)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap BASE: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  ESC   |      |      |      |      |      | LEFT |           | RIGHT|      |      |      |      |      |Special |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |  Q   |  W   |  F   |  P   |  B   |      |           |  L1  |  J   |  L   |  U   |  Y   |  ;:  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Symbols |  A   |  R   |  S   |  T   |  G   |------|           |------|  M   |  N   |  E   |  I   |  O   |   -_   |
 * |--------+------+------+------+------+------| Win  |           | Win  |------+------+------+------+------+--------|
 * | Shift  |  Z   |  X   |  C   |  D   |  V   |      |           |      |  K   |  H   |  ,<  |  .>  |  '@  |   /?   |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | CTRL |  \|  |      | Alt  |Enter |                                       | Left | Down |  Up  |Right |Media |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-----------.
 *                                        | Copy |Paste |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | App  |       |      |      |      |
 *                                 | Space|Backsp|------|       |------|Shift |Symbol|
 *                                 |      |ace   | Del  |       | CTRL |      |Layer |
 *                                 `--------------------'       `--------------------'
 */
[BASE] = LAYOUT_ergodox_pretty(
  // left hand
  KC_ESC,   KC_NO,   KC_NO, KC_NO,   KC_NO,  KC_NO, KC_LEFT,              KC_RGHT, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    MO(SPEC),
  KC_TAB,   UK_Q,    UK_W,  UK_F,    UK_P,   UK_B,  KC_NO,                KC_NO,   UK_J,    UK_L,    UK_U,    UK_Y,    UK_SCLN,  KC_NO,
  MO(SYMB), UK_A,    UK_R,  UK_S,    UK_T,   UK_G,                                 UK_M,    UK_N,    UK_E,    UK_I,    UK_O,     UK_MINS,
  KC_LSFT,  UK_Z,    UK_X,  UK_C,    UK_D,   UK_V,  KC_LWIN,              KC_RWIN, UK_K,    UK_H,    UK_COMM, UK_DOT,  UK_QUOT,  UK_SLSH,
  KC_LCTL,  UK_BSLS, KC_NO, KC_LALT, KC_ENT,                                       KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, MO(MDIA),
                                       LCTL(KC_INS),  LSFT(KC_INS),     KC_NO,   KC_NO,
                                                      KC_APP,           KC_NO,
                                     KC_SPC, KC_BSPC, KC_DEL,           KC_RCTL, KC_RSFT, MO(SYMB)
),

/* Keymap STNO: Stenography Layer
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |(Play)|           |      |      |      |      |      |      |Base/Spc|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |  #   |  #   |  #   |  #   |  #   |      |           |      |  #   |  #   |  #   |  #   |  #   |   #    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | (Symbl)|  S   |  T   |  P   |  H   |  *   |------|           |------|  *   |  F   |  P   |  L   |  T   |   D    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | (Shift)|  S   |  K   |  W   |  R   |  *   |      |           |      |  *   |  R   |  B   |  G   |  S   |   Z    |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |(CTRL)|      |      |(Alt) |(Entr)|                                       |(Left)|(Down)| (Up) |(Rght)|      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |(Copy)|(Pste)|       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |  A   |  O   |------|       |------|  E   |  U   |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[STNO] = LAYOUT_ergodox_pretty(
  // left hand
  KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,  KC_NO,   KC_TRNS,    KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  BASE_SPEC,
  KC_NO,   STN_N1, STN_N2, STN_N3,  STN_N4, STN_N5,  KC_NO,      KC_NO, STN_N6,  STN_N7,  STN_N8,  STN_N9,  STN_NA, STN_NB,
  KC_TRNS, STN_S1, STN_TL, STN_PL,  STN_HL, STN_ST1,                    STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR, STN_DR,
  KC_TRNS, STN_S2, STN_KL, STN_WL,  STN_RL, STN_ST2, KC_NO,      KC_NO, STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR, STN_ZR,
  KC_TRNS, KC_NO,  KC_NO,  KC_TRNS, KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
                                          KC_TRNS, KC_TRNS,        KC_NO, KC_NO,
                                                   KC_NO,          KC_NO,
                                   STN_A, STN_O,   KC_NO,          KC_NO, STN_E, STN_U
),

/* Keymap 1: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * | Escape  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |  !   |  "   |  {   |  }   |  £   |      |           |      |  /   |  7&  |  8*  |  9(  |   *  |   %    |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |  &   |  ^   |  (   |  )   |  $   |------|           |------|  :   |  4$  |  5%  |  6^  |   +  |   -_   |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |  |   |  `   |  [   |  ]   |  #~  |      |           |      |  _   |  1!  |  2"  |  3£  |  0)  |   =+   |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |  ,<  |  .   |   =  |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[SYMB] = LAYOUT_ergodox_pretty(
  // left hand
  KC_ESC,  KC_F1,         KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_TRNS,
  KC_TRNS, UK_EXLM,       UK_DQUO, UK_LCBR, UK_RCBR, UK_PND,  KC_TRNS,     KC_TRNS, KC_PSLS, UK_7,    UK_8,    UK_9,    UK_ASTR, UK_PERC,
  KC_TRNS, UK_AMPR,       UK_CIRC, UK_LPRN, UK_RPRN, UK_DLR,                        UK_COLN, UK_4,    UK_5,    UK_6,    UK_PLUS, UK_MINS,
  KC_TRNS, LSFT(UK_BSLS), UK_GRV,  UK_LBRC, UK_RBRC, UK_HASH, KC_TRNS,     KC_TRNS, UK_UNDS, UK_1,    UK_2,    UK_3,    UK_0,    UK_EQL,
  KC_TRNS, KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_NO,   UK_COMM, KC_PDOT, KC_NO,   KC_TRNS,
                                                   KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS,
                                                            KC_TRNS,         KC_TRNS,
                                          KC_TRNS, KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      | MsUp |      |      |Wheel |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|  Up  |           |      |------+------+------+------+------+--------|
 * |        |      |MsLeft|MsDown|MsRght|      |------|           |------|      |LftClk|RgtClk|MidClk|      |        |
 * |--------+------+------+------+------+------|Wheel |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      | Down |           |      |      |      | Prev | Next |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       | Mute |VolDn |VolUp |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |Click4|Click5|       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 | Left |Right |------|       |------| Prev | Next |
 *                                 |Click |Click |MdlClk|       |      | Tab  | Tab  |
 *                                 `--------------------'       `--------------------'
 */
[MDIA] = LAYOUT_ergodox_pretty(
  // left hand
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_U, KC_TRNS, KC_TRNS, KC_WH_U,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_MS_L, KC_MS_D, KC_MS_R, KC_TRNS,                       KC_TRNS, KC_BTN1, KC_BTN2, KC_BTN3, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_WH_D,     KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MNXT, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_TRNS,

                                               KC_BTN4, KC_BTN5,     KC_TRNS, KC_TRNS,
                                                        KC_TRNS,     KC_TRNS,
                                      KC_BTN1, KC_BTN2, KC_BTN3,     KC_TRNS, LCTL(KC_PGUP), LCTL(KC_PGDN)
),

/* Keymap SPEC: Special functions and layer switches
 *  Left side is fully transparent                                 Right side is opaque apart from marked keys
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |RstKb |      |      |      |Steno | Base |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |NumLk |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |      | NKRO |Sleep |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |Print |Print |
 *                                 |      |      |------|       |------|Screen|Screen|
 *                                 |      |      |      |       |      |      |Window|
 *                                 `--------------------'       `--------------------'
 */
[SPEC] = LAYOUT_ergodox_pretty(
  // left hand
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     QK_BOOT, KC_NO, KC_NO, KC_NO, TO(STNO), TO(BASE), KC_NO,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,    KC_NUM,   KC_NO,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                       KC_NO, KC_NO, KC_NO, NK_ON,    KC_SLEP,  KC_NO,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO,    KC_NO,    KC_NO,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                       KC_NO, KC_NO, KC_NO,    KC_NO,    KC_NO,

                                               KC_TRNS, KC_TRNS,     KC_NO, KC_NO,
                                                        KC_TRNS,     KC_NO,
                                      KC_TRNS, KC_TRNS, KC_TRNS,     KC_NO, KC_PSCR, LALT(KC_PSCR)
),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case VRSN:
                SEND_STRING(QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
                return false;
        }
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    uint8_t layer = get_highest_layer(state);
    switch (layer) {
        case BASE:
#ifdef RGBLIGHT_COLOR_LAYER_0
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
            break;
        case STNO:
            ergodox_right_led_1_on();
#ifdef RGBLIGHT_COLOR_LAYER_1
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
            break;
        // case 2:
            // ergodox_right_led_2_on();
// #ifdef RGBLIGHT_COLOR_LAYER_2
            // rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
// #endif
            // break;
        // case 3:
            // ergodox_right_led_3_on();
// #ifdef RGBLIGHT_COLOR_LAYER_3
            // rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
// #endif
            // break;
        case SYMB:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
#ifdef RGBLIGHT_COLOR_LAYER_4
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
            break;
        case MDIA:
            ergodox_right_led_1_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_5
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
            break;
        // case 6:
            // ergodox_right_led_2_on();
            // ergodox_right_led_3_on();
// #ifdef RGBLIGHT_COLOR_LAYER_6
            // rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
// #endif
            // break;
        case SPEC:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
#ifdef RGBLIGHT_COLOR_LAYER_7
            rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
            break;
        default:
            break;
    }

    return state;
};
