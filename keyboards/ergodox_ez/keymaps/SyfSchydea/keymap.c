#include QMK_KEYBOARD_H
#include "version.h"
#include "keymap_uk.h"

enum layer {
	BASE,  // default layer
	STNO,  // Steno Layer
	GAME,  // Game Layer

	CTRL,  // Ctrl overlay, mostly qwerty
	SYMB,  // symbols
	NAVI,  // Navigation
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

void td_base_spec_fin(tap_dance_state_t *state, void *user_data) {
	if (state->pressed) {
		// Begin hold case
		layer_on(SPEC);
	}
}

void td_base_spec_release(tap_dance_state_t *state, void *user_data) {
	if (!state->finished) {
		// Fire tap case
		layer_move(BASE);
		state->finished = true;
	}
}

void td_base_spec_reset(tap_dance_state_t *state, void *user_data) {
	// End hold case
	layer_off(SPEC);
}

tap_dance_action_t tap_dance_actions[] = {
	// TO(BASE) on tap, MO(SPEC) on hold
	[TD_BASE_SPEC] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(
			NULL, // on each tap
			td_base_spec_release,
			td_base_spec_fin,
			td_base_spec_reset),
};

// Strafe-tapping
// Keep track of if the physical A/D keys are held
bool key_a_held = false;
bool key_d_held = false;

#define set_code_state(kc, down) ((down)? register_code : unregister_code)(kc)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	// Do strafe-tap pairs
	if (IS_LAYER_ON(GAME)) {
		switch (keycode) {
			case UK_A:
				key_a_held = record->event.pressed;
				if (key_d_held) {
					set_code_state(UK_D, !record->event.pressed);
				}
				break;

			case UK_D:
				key_d_held = record->event.pressed;
				if (key_a_held) {
					set_code_state(UK_A, !record->event.pressed);
				}
				break;
		}
	}

	return true;
}

#define BASE_SPEC TD(TD_BASE_SPEC)
#define SYF_LCTL  LM(CTRL, MOD_LCTL)

#define LCG(kc) LCTL(LGUI(kc))

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap BASE: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  ESC   |      |      |      |      |      | LEFT |           | RIGHT|      |      |      |      |      |Special |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |  Q   |  W   |  F   |  P   |  B   |      |           |      |  J   |  L   |  U   |  Y   |  ;:  | Enter  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |Symbols |  A   |  R   |  S   |  T   |  G   |------|           |------|  M   |  N   |  E   |  I   |  O   |   -_   |
 * |--------+------+------+------+------+------| Win  |           | Win  |------+------+------+------+------+--------|
 * | Shift  |  Z   |  X   |  C   |  D   |  V   |      |           |      |  K   |  H   |  ,<  |  .>  |  '@  |   /?   |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | CTRL |  \|  |      | Alt  | Nav  |                                       | Left | Down |  Up  |Right |Media |
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
  KC_TAB,   UK_Q,    UK_W,  UK_F,    UK_P,   UK_B,  KC_NO,                KC_NO,   UK_J,    UK_L,    UK_U,    UK_Y,    UK_SCLN,  KC_ENT,
  MO(SYMB), UK_A,    UK_R,  UK_S,    UK_T,   UK_G,                                 UK_M,    UK_N,    UK_E,    UK_I,    UK_O,     UK_MINS,
  KC_LSFT,  UK_Z,    UK_X,  UK_C,    UK_D,   UK_V,  KC_LWIN,              KC_RWIN, UK_K,    UK_H,    UK_COMM, UK_DOT,  UK_QUOT,  UK_SLSH,
  SYF_LCTL, UK_BSLS, KC_NO, KC_LALT, MO(NAVI),                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, MO(MDIA),
                                       LCTL(KC_INS),  LSFT(KC_INS),     KC_NO,   KC_NO,
                                                      KC_APP,           KC_NO,
                                     KC_SPC, KC_BSPC, KC_DEL,           KC_RCTL, KC_RSFT, MO(SYMB)
),

/* Keymap STNO: Stenography Layer
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |Base/Spc|
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | (Tab)  |  #   |  #   |  #   |  #   |  #   |      |           |      |  #   |  #   |  #   |  #   |  #   |   #    |
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
  KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,  KC_NO,   KC_NO,      KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  BASE_SPEC,
  KC_TRNS, STN_N1, STN_N2, STN_N3,  STN_N4, STN_N5,  KC_NO,      KC_NO, STN_N6,  STN_N7,  STN_N8,  STN_N9,  STN_NA, STN_NB,
  KC_TRNS, STN_S1, STN_TL, STN_PL,  STN_HL, STN_ST1,                    STN_ST3, STN_FR,  STN_PR,  STN_LR,  STN_TR, STN_DR,
  KC_TRNS, STN_S2, STN_KL, STN_WL,  STN_RL, STN_ST2, KC_NO,      KC_NO, STN_ST4, STN_RR,  STN_BR,  STN_GR,  STN_SR, STN_ZR,
  KC_TRNS, KC_NO,  KC_NO,  KC_TRNS, KC_TRNS,                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO,
                                          KC_TRNS, KC_TRNS,        KC_NO, KC_NO,
                                                   KC_NO,          KC_NO,
                                   STN_A, STN_O,   KC_NO,          KC_NO, STN_E, STN_U
),

/* Keymap GAME: Gaming layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  ESC   |  1   |  2   |  3   |  4   |  5   |  6   |           |      |  6   |  7   |  8   |  9   |  0   |Special |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |  T   |  Q   |  W   |  E   |  R   |  7   |           |      |  Y   |  U   |  I   |  O   |  P   | Enter  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |   \    |  G   |  A   |  S   |  D   |  F   |------|           |------|  H   |  J   |  K   |  L   |  ;:  |   '@   |
 * |--------+------+------+------+------+------|  8   |           | Win  |------+------+------+------+------+--------|
 * | Shift  |  B   |  Z   |  X   |  C   |  V   |      |           |      |  N   |  M   |  ,<  |  .>  |  /?  |   -_   |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | CTRL |  9   |  0   | Alt  | Nav  |                                       | Left | Down |  Up  |Right |Media |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Copy |Paste |       |  -   |  =   |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 | Space|Backsp|------|       |------|Shift |Symbol|
 *                                 |      |ace   |      |       | CTRL |      |Layer |
 *                                 `--------------------'       `--------------------'
 */
[GAME] = LAYOUT_ergodox_pretty(
  // left hand
  KC_ESC,  UK_1, UK_2, UK_3,    UK_4,   UK_5, UK_6,        KC_NO,   UK_6,    UK_7,    UK_8,    UK_9,    UK_0,     BASE_SPEC,
  KC_TAB,  UK_T, UK_Q, UK_W,    UK_E,   UK_R, UK_7,        KC_NO,   UK_Y,    UK_U,    UK_I,    UK_O,    UK_P,     KC_ENT,
  UK_BSLS, UK_G, UK_A, UK_S,    UK_D,   UK_F,                       UK_H,    UK_J,    UK_K,    UK_L,    UK_SCLN,  UK_QUOT,
  KC_LSFT, UK_B, UK_Z, UK_X,    UK_C,   UK_V, UK_8,        KC_RWIN, UK_N,    UK_M,    UK_COMM, UK_DOT,  UK_SLSH,  UK_MINS,
  KC_LCTL, UK_9, UK_0, KC_LALT, KC_ENT,                                      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,  MO(MDIA),
                              LCTL(KC_INS), LSFT(KC_INS),    UK_MINS, UK_EQL,
                                            KC_NO,           KC_NO,
                      KC_SPC,      KC_BSPC, KC_NO,           KC_RCTL, KC_RSFT, MO(SYMB)
),

/* Keymap CTRL: CTRL overlay - mostly qwerty
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |         |  Q   |  W   |  E   |  R   |  T   |  P   |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |  A   |  S   |  D   |  F   |  G   |------|           |------|      |      |      |      |      |        |
 * |---------+------+------+------+------+------|  N   |           |      |------+------+------+------+------+--------|
 * |         |  Z   |  X   |  C   |  V   |  B   |      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
[CTRL] = LAYOUT_ergodox_pretty(
  // left hand
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, UK_Q,    UK_W,    UK_E,    UK_R,    UK_T,    UK_P,        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, UK_A,    UK_S,    UK_D,    UK_F,    UK_G,                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, UK_Z,    UK_X,    UK_C,    UK_V,    UK_B,    UK_N,        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                             KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS,
                                                      KC_TRNS,         KC_TRNS,
                                    KC_TRNS, KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS
),

/* Keymap SYMB: Symbol Layer
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

/* Keymap NAVI: Navigation Layer
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |         |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |CtlAltTab|PageUp| Home |  Up  | End  |  T   |  P   |           |      |      |      |      |      |      |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |         |PageDn| Left | Down |Right |  G   |------|           |------|      |      |      |      |      |        |
 * |---------+------+------+------+------+------|  N   |           |      |------+------+------+------+------+--------|
 * |         |  Z   |  X   |DskTLt|DskTRt|  B   |      |           |      |      |      |      |      |      |        |
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |       |      |      |      |      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.       ,-------------.
 *                                         |      |      |       |      |      |
 *                                  ,------|------|------|       |------+------+------.
 *                                  |      |      |      |       |      |      |      |
 *                                  |      |      |------|       |------|      |      |
 *                                  |      |      |      |       |      |      |      |
 *                                  `--------------------'       `--------------------'
 */
[NAVI] = LAYOUT_ergodox_pretty(
  // left hand
  KC_NO,       KC_NO,   KC_NO,   KC_NO,        KC_NO,        KC_NO, KC_NO,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  LCA(KC_TAB), KC_PGUP, KC_HOME, KC_UP,        KC_END,       KC_NO, KC_NO,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_NO,       KC_PGDN, KC_LEFT, KC_DOWN,      KC_RGHT,      KC_NO,                    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_NO,       KC_NO,   KC_NO,   LCG(KC_LEFT), LCG(KC_RGHT), KC_NO, KC_NO,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_NO,       KC_NO,   KC_NO,   KC_NO,        KC_TRNS,                                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                         KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS,
                                                                  KC_TRNS,      KC_TRNS,
                                                KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS
),

/* Keymap MDIA: Media and mouse keys
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
 * |        |      |      |      |      |      |      |           |RstKb |      |      |      | Game |Steno |        |
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
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     QK_BOOT, KC_NO, KC_NO, KC_NO, TO(GAME), TO(STNO), KC_TRNS,
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

// Runs just one time when the keyboard initializes.
void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_COLOR_LAYER_0
    rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
};

#define LED_BLACK() do{ergodox_right_led_1_off();ergodox_right_led_2_off();ergodox_right_led_3_off();}while(0)
#define LED_RED()   do{ergodox_right_led_1_on(); ergodox_right_led_2_off();ergodox_right_led_3_off();}while(0)
#define LED_GREEN() do{ergodox_right_led_1_off();ergodox_right_led_2_on(); ergodox_right_led_3_off();}while(0)
#define LED_WHITE() do{ergodox_right_led_1_on(); ergodox_right_led_2_on(); ergodox_right_led_3_on(); }while(0)

// Runs whenever there is a layer state change.
layer_state_t layer_state_set_user(layer_state_t state) {
	ergodox_board_led_off();

	if (layer_state_cmp(state, SPEC)) {
		LED_WHITE();
	} else if (layer_state_cmp(state, GAME)) {
		LED_GREEN();
	} else if (layer_state_cmp(state, STNO)) {
		LED_RED();
	} else { // BASE
		LED_BLACK();
	}

	uint8_t layer = get_highest_layer(state);
	switch (layer) {
		case 0:
#ifdef RGBLIGHT_COLOR_LAYER_0
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_0);
#endif
			break;
		case 1:
#ifdef RGBLIGHT_COLOR_LAYER_1
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_1);
#endif
			break;
		case 2:
#ifdef RGBLIGHT_COLOR_LAYER_2
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_2);
#endif
			break;
		case 3:
#ifdef RGBLIGHT_COLOR_LAYER_3
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_3);
#endif
			break;
		case 4:
#ifdef RGBLIGHT_COLOR_LAYER_4
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_4);
#endif
			break;
		case 5:
#ifdef RGBLIGHT_COLOR_LAYER_5
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_5);
#endif
			break;
		case 6:
#ifdef RGBLIGHT_COLOR_LAYER_6
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_6);
#endif
			break;
		case 7:
#ifdef RGBLIGHT_COLOR_LAYER_7
			rgblight_setrgb(RGBLIGHT_COLOR_LAYER_7);
#endif
			break;
		default:
			break;
	}

	return state;
};
