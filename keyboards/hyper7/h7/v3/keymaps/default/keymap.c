/* #include QMK_KEYBOARD_H */
#include "../../v3.h"

// Brevity defines
#define FT KC_TRNS

// modifier masks used for determining modifier actuation within macros
#define MODS_SHIFT_MASK   (MOD_BIT(KC_LSFT)|MOD_BIT(KC_RSFT))

// Clear mods, perform action, restore mods
#define CLEAN_MODS(action) {       \
        uint8_t mods = get_mods(); \
        clear_mods();              \
        action;                    \
        set_mods(mods);            \
    }

static uint8_t _mods = 0;

static inline void push_mods(void) {
    _mods = get_mods();
    clear_mods();
}

static inline void pop_mods(void) {
    set_mods(_mods);
}

static inline bool shift_pressed(void) {
    return get_mods() & MODS_SHIFT_MASK;
}

// UNICODEMAP_ENABLE
enum unicode_names {
    AT_SYMBOL,
    GBP,
    JPY,
    EURO,
    PLUS_MINUS,
    QUOTATION_MARK,
    TILDE_OPERATOR,
    SINGLE_RIGHT_POINTING_ANGLE_QUOTATION_MARK,
    SINGLE_LEFT_POINTING_ANGLE_QUOTATION_MARK,
    BACKTICK,
    DQUOTE_OPEN,
    DQUOTE_CLOSE,
    PRIME,
    PIPE
};

const uint32_t PROGMEM unicode_map[] = {
    // Unicode char definitions
    [AT_SYMBOL] = 0x0040,
    [GBP] = 0x00A3,
    [JPY] = 0x00A5,
    [EURO] = 0x20AC,
    [PLUS_MINUS] = 0x00B1,
    [QUOTATION_MARK] = 0x0022,
    [TILDE_OPERATOR] = 0x223C,
    [SINGLE_RIGHT_POINTING_ANGLE_QUOTATION_MARK] = 0x203A,
    [SINGLE_LEFT_POINTING_ANGLE_QUOTATION_MARK] = 0x2039,
    [BACKTICK] = 0x2018,
    [DQUOTE_OPEN] = 0x201C,
    [DQUOTE_CLOSE] = 0x201D,
    [PRIME] = 0x2019,
    [PIPE] = 0x2502
};

/*
 * Layer definitions.
 */
#define _BASE 0   // Base layer
#define _APL 1    // APL symbols
#define _GREEK 2  // Greek symbols
#define _FN 3     // FN & media keys

enum custom_keycodes {
    // daughter board R1
    H_HELP = SAFE_RANGE,
    H_MACRO,
    H_TERMINAL,
    H_QUOTE,
    H_OVERSTRIKE,
    H_CLEAR_INPUT,
    H_CLEAR_SCREEN,
    H_HOLD_OUTPUT,
    H_STOP_INPUT,
    H_ABORT,
    H_BREAK,
    H_RESUME,
    H_CALL,

    // daughter board R2
    H_LOCAL,
    H_NETWORK,
    H_SYSTEM,
    H_REFRESH,
    H_BUFFER,
    H_SQUARE,
    H_CIRCLE,
    H_TRIANGLE,
    H_DIAMOND,
    H_REPEAT,
    H_TRANSMIT,
    H_STATUS,
    H_SUSPEND,

    // daughter board r3
    H_CLOSE,
    H_OPEN,
    H_COMPLETE,

    // main board r1
    H_FIND,
    H_WRITE,
    H_DOUBLE_QUOTE__PLUS_MINUS,
    /* H_COLON__TILDE, */
    H_L_BRACE__L_CHEVRON,
    H_R_BRACE__R_CHEVRON,

    // main board r2
    H_MARK,
    H_UNDO,
    H_LEFT_PAREN__LEFT_BRACKET,
    H_RIGHT_PAREN__RIGHT_BRACKET,

    // main board r3
    H_SELECT,
    H_DEBUG,
    H_SEMI_COLON__BACK_TICK,
    /* H_TICK__QUOTE, */
    H_LINE,
    H_PAGE,

    // main board r4
    H_TTY,
    H_LOCK, // lock machine? a little close to other used keys on a single keypress?

    // main board r5
    H_EOF,
    H_7BIT,
    H_CIRCLE_SM,

    // cam's custom stuff.
    H_HAND_RIGHT,
    H_HAND_LEFT,
    H_HAND_UP,
    H_HAND_DOWN,
    H_ROMAN_NUMERAL_I,
    H_ROMAN_NUMERAL_II,
    H_ROMAN_NUMERAL_III,
    H_ROMAN_NUMERAL_IV,
    H_PASTE,
    H_CUT,

    // APL layer
    H_APL_LOGICAL_AND,
    H_APL_LOGICAL_OR,
    H_APL_INTERSECT,
    H_APL_UNION,
    H_APL_SUBSET_OF,
    H_APL_SUPERSET_OF,
    H_APL_FORALL,
    H_APL_LEMNISCATE,
    H_APL_THERE_EXISTS,
    H_APL_PARTIAL_DIFFERENTIAL,
    H_APL_UP_TACK,
    H_APL_DOWN_TACK,
    H_APL_RIGHT_TACK,
    H_APL_LEFT_TACK,
    H_APL_UPWARDS_ARROW,
    H_APL_DOWNARDS_ARROW,
    H_APL_LEFTWARDS_ARROW,
    H_APL_RIGHTWARDS_ARROW,
    H_APL_LEFTRIGHT_ARROW,
    H_APL_LEFT_FLOOR,
    H_APL_LEFT_CEILING,
    H_APL_NOT_EQUAL_TO,
    H_APL_ASYMPTOTICALLY_EQUAL_TO,
    H_APL_NOT_ASYMPTOTICALLY_EQUAL_TO,
    H_APL_LESS_THAN_OR_EQUAL,
    H_APL_GREATER_THAN_OR_EQUAL,

    // Symbol Layer
    // [main board] row 0
    H_SYMBOL_CONTOUR_INTEGRAL,
    H_SYMBOL_COPTIC_LC_DEI,
    H_SYMBOL_DOUBLE_DAGGER,
    H_SYMBOL_NABLA,
    H_SYMBOL_CENT,
    H_SYMBOL_DEGREE,
    H_SYMBOL_APL_QUAD,
    H_SYMBOL_DIVISION,
    H_SYMBOL_MULTIPLICATION,
    H_SYMBOL_PILCROW,
    H_SYMBOL_LARGE_CIRCLE,
    H_SYMBOL_HORIZONTAL_BAR,
    H_SYMBOL_APPROXIMATELY_EQUAL_TO,
    H_SYMBOL_DOUBLE_VERTICAL_LINE,
    H_SYMBOL_SQUARE_IMAGE_OF,
    H_SYMBOL_SQUARE_ORIGINAL_OF,

    // [main board] row 1
    H_SYMBOL_GREEK_THETA,
    H_SYMBOL_GREEK_OMEGA,
    H_SYMBOL_GREEK_EPSILON,
    H_SYMBOL_GREEK_RHO,
    H_SYMBOL_GREEK_TAU,
    H_SYMBOL_GREEK_PSI,
    H_SYMBOL_GREEK_UPSILON,
    H_SYMBOL_GREEK_IOTA,
    H_SYMBOL_GREEK_OMICRON,
    H_SYMBOL_GREEK_PI,
    H_SYMBOL_MATHEMATICAL_LEFT_WHITE_SQUARE_BRACKET,
    H_SYMBOL_MATHEMATICAL_RIGHT_WHITE_SQUARE_BRACKET,

    // [main board] row 2
    H_SYMBOL_GREEK_ALPHA,
    H_SYMBOL_GREEK_SIGMA,
    H_SYMBOL_GREEK_DELTA,
    H_SYMBOL_GREEK_PHI,
    H_SYMBOL_GREEK_GAMMA,
    H_SYMBOL_GREEK_ETA,
    H_SYMBOL_GREEK_YOT,
    H_SYMBOL_GREEK_KAPPA,
    H_SYMBOL_GREEK_LAMDA,
    H_SYMBOL_TWO_DOT_LEADER,
    H_SYMBOL_BULLET,

    // [main board] row 3
    H_SYMBOL_GREEK_ZETA,
    H_SYMBOL_GREEK_XI,
    H_SYMBOL_GREEK_CHI,
    H_SYMBOL_GREEK_FINAL_SIGMA,
    H_SYMBOL_GREEK_BETA,
    H_SYMBOL_GREEK_NU,
    H_SYMBOL_GREEK_MU,
    H_SYMBOL_MUCH_LESS_THAN,
    H_SYMBOL_MUCH_GREATER_THAN,
    H_SYMBOL_INTEGRAL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* _BASE: Base Layer(Default)
     * ,-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
     * |   Help    |   Macro   |#| Terminal  |   Quote   | OverStrike| ClearInput|ClearScreen|HoldOutput | StopOutput|   Abort   |   Break   |  Resume   |#|   Call    |   RESET   |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |   Local   |  Network  |#|  System   |  Refresh  |   Buffer  |   Square  |   Circle  | Triangle  |  Diamond  |   Repeat  |  Transmit |  Status   |#|  Suspend  |  CapsLock |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F1  | F2  |Close|Open |#|   Esc     |  ?  |  !  |  @  |  £  |  €  |  ¥  |  '  |  “  |  ”  |  '  |  _  |  <  |  >  |  |  |  {  |  }  | Complete  |#|  ^  |  %  |  #  |  $  |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |###############################################################################################################################################################################|
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F3  | F4  |Find |Write|#|Compose | "/± | :/~ |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |  |  | {/‹  | }/›  | Undo   |#|  ~  |  /  |  *  |  -  |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F5  | F6  |Mark |Undo |#|Paste|  Cut   |Tab     |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  (  |  )  | Bspace | Clear  |Begin|#|  7  |  8  |  9  |  +  |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F7  | F8  |Selec|Debug|#|Function|  Mode  |  Top   |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  | Return |  Line  |  Page  |#|  4  |  5  |  6  |  &  |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F9  | F10 | TTY |LOCK |#|Begin| End | Symbol | Shift  |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  | Shift  | Symbol |  Up | End |#|  1  |  2  |  3  |  =  |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * | F11 | F12 |Home | EOF |#|Prev |Next |7bit | Alt | Hyper  | Spr |  Ctrl  | Space |TRNS |  Del   |  Ctrl  | Spr  | Hyper  |Meta |Left |Down |Right|#| Del |  0  |  .  | Run |
     * `-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------'
     *  Incomplete mappings
     *  r1: H_MACRO, H_TERMINAL, H_OVERSTRIKE, H_HOLD_OUTPUT, H_STOP_INPUT, H_CALL
     *  r2: H_LOCAL, H_NETWORK, H_SYSTEM, H_REFRESH, H_BUFFER, H_SQUARE, H_CIRCLE, H_TRIANGLE, H_DIAMOND, H_REPEAT, H_TRANSMIT, H_STATUS
     *  r3: H_COMPLETE
     *
     *  r1: H_FIND, H_WRITE
     *  r2: H_MARK
     *  r3: H_SELECT, H_DEBUG, H_LINE
     *  r4: H_TTY
     *  r2: H_7BIT, H_CIRCLE_SM
     */
    [_BASE] = LAYOUT_h7v3_183
    (
     H_HELP,            H_MACRO,           H_TERMINAL,           H_QUOTE,              H_OVERSTRIKE,                                             H_CLEAR_INPUT,             H_CLEAR_SCREEN,                        H_HOLD_OUTPUT,                           H_STOP_INPUT,           H_ABORT,           H_BREAK,              H_RESUME,            H_CALL,              KC_NLCK,           //14
     H_LOCAL,           H_NETWORK,         H_SYSTEM,             H_REFRESH,            H_BUFFER,                                                 H_SQUARE,                  H_CIRCLE,                              H_TRIANGLE,                              H_DIAMOND,              H_REPEAT,          H_TRANSMIT,           H_STATUS,            H_SUSPEND,           KC_CLCK,           //14
     KC_F1,   KC_F2,    H_CLOSE,  H_OPEN,  KC_ESC,               KC_QUES,    KC_EXLM,  H_LEFT_PAREN__LEFT_BRACKET, H_RIGHT_PAREN__RIGHT_BRACKET, H_HAND_LEFT, H_HAND_RIGHT, H_ROMAN_NUMERAL_I, H_ROMAN_NUMERAL_II, H_ROMAN_NUMERAL_III, H_ROMAN_NUMERAL_IV, KC_UNDS,       KC_LABK, KC_RABK,  X(PIPE), KC_LCBR,    KC_RCBR,  H_COMPLETE,          KC_CIRC,   KC_PERC,  KC_HASH,  KC_DLR,  //26

     KC_F3,   KC_F4,    H_FIND,   H_WRITE, KC_LEAD,  H_DOUBLE_QUOTE__PLUS_MINUS, KC_GRAVE,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,    KC_6,     KC_7,    KC_8,     KC_9,    KC_0,     KC_MINS,  KC_EQL,  KC_BACKSLASH, H_L_BRACE__L_CHEVRON,  H_R_BRACE__R_CHEVRON,  H_UNDO,    KC_TILD,   KC_SLSH,  KC_PAST,  KC_PMNS, //27
     KC_F5,   KC_F6,    H_MARK,   H_UNDO,  H_PASTE,  H_CUT ,   KC_TAB ,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,    KC_Y,     KC_U,    KC_I,     KC_O,    KC_P,     KC_LBRC,  KC_RBRC, KC_BSPC, KC_CLEAR, KC_HOME,                                                                 KC_P7,     KC_P8,    KC_P9,    KC_PPLS, //26
     KC_F7,   KC_F8,    KC_SELECT,H_DEBUG, MO(_FN),  KC_NO,       MO(_APL), KC_A,     KC_S,     KC_D,     KC_F,     KC_G,    KC_H,     KC_J,    KC_K,     KC_L,    KC_SCOLON,  KC_QUOT,  KC_ENT , H_LINE, H_PAGE,                                                           KC_P4,     KC_P5,    KC_P6,    KC_AMPR, //25
     KC_F9,   KC_F10,   H_TTY,    KC_LOCK, KC_HOME,  KC_END ,  MO(_GREEK), KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,    KC_B,     KC_N,    KC_M,     KC_COMM, KC_DOT,   KC_SLSH,  KC_RSFT, MO(_GREEK),KC_UP,    KC_END,                                                              KC_P1,     KC_P2,    KC_P3,    KC_EQL,  //26
     KC_F11,  KC_F12,   KC_HOME,  H_EOF,   H_HAND_UP,H_HAND_DOWN, H_7BIT,  KC_RCTL,  KC_LALT,  KC_LGUI,  KC_LCTL,  KC_SPC,  H_CIRCLE_SM,  KC_SPC,  KC_LCTL,  KC_APP, KC_RCTL,  KC_RALT,  KC_LEFT, KC_DOWN, KC_RGHT,                                                                        KC_DEL,    KC_P0,    KC_PDOT,  KC_ENTER //25
     ),
    /* _APL: Cadet Symbol Layer (Default)
     * ,-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |           |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |           |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|           |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |           |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |###############################################################################################################################################################################|
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |        |        |  ⋀  |  ⋁  |  ⋂  |  ⋃  |  ⊂  |  ⊃  |  ∀  |  ∞  |  ∎  |  ∂  |     |     |        |        |     |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |        |        |  ⊥  |  ⊤  |  ⊢  |  ⊣  |  ↑  |  ↓  |  ←  |  →  |  ↔  |  ;  |  '  |        |        |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |     |        |        |  ⌊  |  ⌈  |  ≠  |  ≃  |  ≡  |  ≤  |  ≥  |     |     |     |        |        |     |     |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |     |     |     |        |     |        |        |     |        |        |     |        |     |     |     |     |#|     |     |     |     |
     * `-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_APL] = LAYOUT_h7v3_183(
                             FT,               FT,                 FT,                 FT,                 FT,                 FT,                FT,                FT,                 FT,                FT,               FT,                 FT,                  FT,                FT,
                             FT,               FT,                 FT,                 FT,                 FT,                 FT,                FT,                FT,                 FT,                FT,               FT,                 FT,                  FT,                FT,
                             FT,      FT,      FT,       FT,       FT,                 FT,       FT,       FT,       FT,       FT,       FT,      FT,       FT,      FT,       FT,       FT,      FT,       FT,      FT,      FT,       FT,       FT,                  FT,      FT,       FT,      FT,

                             FT, FT, FT, FT, FT, FT, FT, FT,                FT,               FT,                 FT,                 FT,                            FT,                                FT,                       FT,                          FT,                    FT,                         FT, FT, FT, FT, FT, FT, FT, FT, FT, FT,
                             FT, FT, FT, FT, FT, FT, FT, H_APL_LOGICAL_AND, H_APL_LOGICAL_OR, H_APL_INTERSECT,    H_APL_UNION,        H_APL_SUBSET_OF,               H_APL_SUPERSET_OF,                 H_APL_FORALL,             H_APL_LEMNISCATE,            H_APL_THERE_EXISTS,    H_APL_PARTIAL_DIFFERENTIAL, FT, FT, FT, FT, FT, FT, FT, FT, FT,
                             FT, FT, FT, FT, FT, FT, FT, H_APL_UP_TACK,     H_APL_DOWN_TACK,  H_APL_RIGHT_TACK,   H_APL_LEFT_TACK,    H_APL_UPWARDS_ARROW,           H_APL_DOWNARDS_ARROW,              H_APL_LEFTWARDS_ARROW,    H_APL_RIGHTWARDS_ARROW,      H_APL_LEFTRIGHT_ARROW, FT,                         FT, FT, FT, FT, FT, FT, FT, FT,
                             FT, FT, FT, FT, FT, FT, FT, FT,                H_APL_LEFT_FLOOR, H_APL_LEFT_CEILING, H_APL_NOT_EQUAL_TO, H_APL_ASYMPTOTICALLY_EQUAL_TO, H_APL_NOT_ASYMPTOTICALLY_EQUAL_TO, H_APL_LESS_THAN_OR_EQUAL, H_APL_GREATER_THAN_OR_EQUAL, FT,                    FT,                         FT, FT, FT, FT, FT, FT, FT, FT, FT,
                             FT, FT, FT, FT, FT, FT, FT, FT,                FT,               FT,                 FT,                 FT,                            FT,                                FT,                       FT,                          FT,                    FT,                         FT, FT, FT, FT, FT, FT, FT, FT
                             ),

    /* _GREEK: lower/upper case greek (needs shift modifier application for upper case chars) from codepage U0370.pdf \
     * ,-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |           |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |           |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|           |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |           |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |###############################################################################################################################################################################|
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |  ∮  |     |  ϯ  |  ‡  |  ∇  |  ¢  |  °  |  ⎕  |  ÷  |  ×  |  ¶  |  ◯  |  ―  |  ≈  |  ‖  |  ⊏  |  ⊐  |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |        |        | θ/Θ | ω/Ω | ε/Ε | ρ/Ρ | τ/Τ | ψ/Ψ | υ/Υ | ι/Ι | ο/Ο | π/Π |  ⟦  |  ⟧  |        |        |      |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |        |        | α/Α | σ/Σ | δ/Δ | φ/Φ | γ/Γ | η/Η  | ϳ/Ϳ | κ/Κ | λ/Λ |  ‥  |  •  |        |        |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |     |         | SHIFT | ζ/Ζ  | ξ/Ξ | χ/Χ | ς/Σ | β/Β | ν/Ν | μ/Μ |  ≪  |  ≫  |  ∫  |        |        |     |     |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |     |     |     |        |     |        |        |     |        |        |     |        |     |     |     |     |#|     |     |     |     |
     * `-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_GREEK] = LAYOUT_h7v3_183(
                               FT,               FT,                 FT,                 FT,                 FT,                 FT,                FT,                FT,                 FT,                FT,               FT,                 FT,                  FT,                FT,
                               FT,               FT,                 FT,                 FT,                 FT,                 FT,                FT,                FT,                 FT,                FT,               FT,                 FT,                  FT,                FT,
                               FT,      FT,      FT,       FT,       FT,                 FT,       FT,       FT,       FT,       FT,       FT,      FT,       FT,      FT,       FT,       FT,       FT,      FT,      FT,      FT,       FT,       FT,                  FT,      FT,       FT,      FT,

                               FT, FT, FT, FT, FT, H_SYMBOL_CONTOUR_INTEGRAL, FT, H_SYMBOL_COPTIC_LC_DEI, H_SYMBOL_DOUBLE_DAGGER, H_SYMBOL_NABLA,         H_SYMBOL_CENT,      H_SYMBOL_DEGREE,            H_SYMBOL_APL_QUAD,   H_SYMBOL_DIVISION,      H_SYMBOL_MULTIPLICATION, H_SYMBOL_PILCROW,        H_SYMBOL_LARGE_CIRCLE,      H_SYMBOL_HORIZONTAL_BAR,                         H_SYMBOL_APPROXIMATELY_EQUAL_TO,                  H_SYMBOL_DOUBLE_VERTICAL_LINE, H_SYMBOL_SQUARE_IMAGE_OF, H_SYMBOL_SQUARE_ORIGINAL_OF, FT, FT, FT, FT, FT,
                               FT, FT, FT, FT, FT, FT,                        FT, H_SYMBOL_GREEK_THETA,   H_SYMBOL_GREEK_OMEGA,   H_SYMBOL_GREEK_EPSILON, H_SYMBOL_GREEK_RHO, H_SYMBOL_GREEK_TAU,         H_SYMBOL_GREEK_PSI,  H_SYMBOL_GREEK_UPSILON, H_SYMBOL_GREEK_IOTA,     H_SYMBOL_GREEK_OMICRON,  H_SYMBOL_GREEK_PI,          H_SYMBOL_MATHEMATICAL_LEFT_WHITE_SQUARE_BRACKET, H_SYMBOL_MATHEMATICAL_RIGHT_WHITE_SQUARE_BRACKET, FT,                            FT,                       FT,                          FT, FT, FT, FT,
                               FT, FT, FT, FT, FT, FT,                        FT, H_SYMBOL_GREEK_ALPHA,   H_SYMBOL_GREEK_SIGMA,   H_SYMBOL_GREEK_DELTA,   H_SYMBOL_GREEK_PHI, H_SYMBOL_GREEK_GAMMA,       H_SYMBOL_GREEK_ETA,  H_SYMBOL_GREEK_YOT,     H_SYMBOL_GREEK_KAPPA,    H_SYMBOL_GREEK_LAMDA,    H_SYMBOL_TWO_DOT_LEADER,    H_SYMBOL_BULLET,                                 FT,                                               FT,                            FT,                       FT,                          FT, FT, FT,
                               FT, FT, FT, FT, FT, FT,                        FT, FT,                     H_SYMBOL_GREEK_ZETA,    H_SYMBOL_GREEK_XI,      H_SYMBOL_GREEK_CHI, H_SYMBOL_GREEK_FINAL_SIGMA, H_SYMBOL_GREEK_BETA, H_SYMBOL_GREEK_NU,      H_SYMBOL_GREEK_MU,       H_SYMBOL_MUCH_LESS_THAN, H_SYMBOL_MUCH_GREATER_THAN, H_SYMBOL_INTEGRAL,                               FT,                                               FT,                            FT,                       FT,                          FT, FT, FT, FT,
                               FT, FT, FT, FT, FT, FT,                        FT, FT,                     FT,                     FT,                     FT,                 FT,                         FT,                  FT,                     FT,                      FT,                      FT,                         FT,                                              FT,                                               FT,                            FT,                       FT,                          FT, FT, FT
                               ),
    /* _FN: Function layer, media & LED mods
     * ,-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |   Reset   |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |           |           |#|           |           |           |           |           |           |           |           |           |           |#|           |           |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|           |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |           |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |###############################################################################################################################################################################|
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |        |        |     |UC_M_WI|     |     |     |     |     |     |     |     |     |     |        |        |      |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|        |UC_MOD|        |     |     |     |     |     |     |     |     |UC_M_LN|     |     |        |        |        |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |H_LOCK|#|     |     |         |       |     |     |UC_M_WC|     |     |     |UC_M_MA|     |     |     |        |        |     |     |#|     |     |     |     |
     * |-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
     * |     |     |     |     |#|     |     |     |     |        |     |        |        |     |        |        |     |        |     |     |     |     |#|     |     |     |     |
     * `-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------'
     */
    [_FN] = LAYOUT_h7v3_183(
                            FT,               FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,               FT,                 FT,                  FT,                RESET,
                            FT,               FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,                 FT,               FT,                 FT,                  FT,                FT,
                            FT,      FT,      FT,       FT,       FT,                 FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,      FT,      FT,       FT,       FT,                  FT,      FT,       FT,      FT,

                            FT,      FT,      FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,      FT,      FT,       FT,       FT,                  FT,      FT,       FT,      FT,
                            FT,      FT,      FT,       FT,       FT,       FT,       FT,       FT,       UC_M_WI,  FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,      FT,      FT,       FT,                            FT,      FT,       FT,      FT,
                            FT,      FT,      FT,       FT,       FT,       UC_MOD,   FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       UC_M_LN,  FT,       FT,       FT,      FT,      FT,                                      FT,      FT,       FT,      FT,
                            FT,      FT,      FT,       H_LOCK,   FT,       FT,       FT,       FT,       FT,       FT,       UC_M_WC,  FT,       FT,       FT,       UC_M_MA,  FT,       FT,       FT,       FT,      FT,      FT,       FT,                            FT,      FT,       FT,      FT,
                            FT,      FT,      FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,       FT,      FT,      FT,                                      FT,      FT,       FT,      FT
                            )
};

// activate leader functionality
LEADER_EXTERNS();

void matrix_scan_keymap(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        // slash :: find
        SEQ_ONE_KEY(KC_SLASH) {
            SEND_STRING(SS_LCTRL(SS_TAP(X_F)));
        }

        // Copy word
        SEQ_TWO_KEYS(KC_W, KC_C) {
            SEND_STRING(SS_LCTRL(SS_TAP(X_LEFT))SS_LSFT(SS_LCTRL(SS_TAP(X_RIGHT)))SS_LCTRL(SS_TAP(X_C)));
        }
        // Copy line
        SEQ_TWO_KEYS(KC_L, KC_C) {
            SEND_STRING(SS_TAP(X_HOME)SS_LSFT(SS_TAP(X_END))SS_LCTRL(SS_TAP(X_C)));
        }
        // Copy all
        SEQ_TWO_KEYS(KC_A, KC_C) {
            SEND_STRING(SS_LCTRL(SS_TAP(X_A)SS_TAP(X_C)));
        }
    }

}

#define CTRL_X SS_LCTL(SS_TAP(X_X))
#define META_X SS_LALT(SS_TAP(X_X))
#define RET    SS_TAP(X_ENTER)

static void send_emacs_unicode(const char *str) {
    push_mods();
    send_string(CTRL_X SS_TAP(X_8) RET);
    send_string(str);
    send_string(RET);
    pop_mods();
}

static void send_emacs_unicode_shift(const char *unshifted_str, const char *shifted_str) {
    if (shift_pressed()) {
        send_emacs_unicode(shifted_str);
    } else {
        send_emacs_unicode(unshifted_str);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            // [daughter board] row 1 POS key macros
        case H_HELP:
            send_string(SS_TAP(X_F1));
            return false;

        case H_MACRO:
            send_string("[MACRO] key");
            return false;

        case H_TERMINAL:
            send_string(META_X "vterm" RET);
            return false;

        case H_QUOTE:
            // wraps selected (editable) text in quotation marks
            send_string(CTRL_X SS_TAP(X_QUOTE) SS_LCTL(SS_TAP(X_V)) SS_TAP(X_QUOTE));
            return false;

        case H_OVERSTRIKE:
            // toggles insert/overstrike
            send_string(SS_TAP(X_INSERT));
            return false;

        case H_CLEAR_INPUT:
            // terminal: clears from the cursor to the beginning of the current command
            send_string(SS_LCTL(SS_TAP(X_U)));
            return false;

        case H_CLEAR_SCREEN:
            send_string(SS_LCTL(SS_TAP(X_L)));
            // ^ linux/osx shell specific, for windows command replace with
            // send_string(SS_TAP(X_C)SS_TAP(X_L)SS_TAP(X_S)SS_TAP(KC_ENTER));
            return false;

        case H_HOLD_OUTPUT:
            send_string(SS_TAP(X_SCROLLLOCK));
            return false;

        case H_STOP_INPUT:
            send_string(SS_LCTL(SS_TAP(X_G)));
            return false;

        case H_ABORT:
            send_string(SS_LCTL(SS_TAP(X_G)));
            return false;

        case H_BREAK:
            send_string(SS_TAP(X_PAUSE));
            return false;

        case H_RESUME:
            send_string(SS_TAP(X_PAUSE));
            // C-u C-SPC (jump to mark)
            /* send_string(SS_LCTL(SS_TAP(X_U)SS_TAP(X_SPACE))); */
            return false;

        case H_CALL:
            // M-x
            send_string(META_X);
            return false;

            // [daughter board] row 2 POS key macros
        case H_LOCAL:
            // M-: eval-expression
            send_string(SS_LALT(SS_LSFT(SS_TAP(X_SCOLON))));
            return false;

        case H_NETWORK:
            send_string("[NETWORK] key");
            return false;

        case H_SYSTEM:
            // M-! shell-command
            send_string(SS_LALT(SS_LSFT(SS_TAP(X_1))));
            return false;

        case H_REFRESH:
            send_string(SS_LCTL(SS_TAP(X_R)));
            return false;

        case H_BUFFER:
            // C-x C-b helm-buffers-list
            send_string(SS_LCTL(SS_TAP(X_X) SS_TAP(X_B)));
            return false;

        case H_SQUARE:
            send_string("SQUARE.");
            return false;

        case H_CIRCLE:
            send_string(SS_LALT(SS_TAP(X_X)));
            return false;

        case H_TRIANGLE:
            if (shift_pressed()) {
                // C-u C-@ (C-s-2) to jump to mark from global mark ring
                push_mods();
                send_string(SS_LCTL(SS_TAP(X_U) SS_LSFT(SS_TAP(X_2))));
                pop_mods();
            } else {
                // C-u C-SPC jump to mark from local mark ring
                send_string(SS_LCTL(SS_TAP(X_U) SS_TAP(X_SPACE)));
            }
            return false;

        case H_DIAMOND:
            send_string("[DIAMOND] key");
            return false;

        case H_REPEAT:
            send_string(SS_LCTL(SS_TAP(X_X))SS_TAP(X_Z));
            return false;

        case H_TRANSMIT:
            send_string("[TRANSMIT] key");
            return false;

        case H_STATUS:
            send_string("[STATUS] key");
            return false;

        case H_SUSPEND:
            send_string(SS_LCTL(SS_TAP(X_Z)));
            return false;

            // [daughter board] row 3
        case H_CLOSE:
            send_string(SS_LCTL(SS_TAP(X_X))SS_TAP(X_K));
            return false;

        case H_OPEN:
            // Shift-open = recent files
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_LCTL(SS_TAP(X_X)SS_TAP(X_R))));
            }
            // Ctrl-open: open file in project
            else if (get_mods() & MOD_BIT(KC_LCTRL)) {
                CLEAN_MODS(send_string(SS_TAP(X_F5)));
            }
            // (regular) open: find-file
            else {
                send_string(SS_LCTL(SS_TAP(X_X)SS_TAP(X_F)));
            }
            return false;

        case H_COMPLETE:
            // tab tab: bash command line completion
            // send_string(SS_TAP(X_TAB)SS_TAP(X_TAB));
            send_string("[COMPLETE] key");
            return false;

            // [main board] row 1
        case H_FIND:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_LCTL(SS_TAP(X_R))));
            } else {
                send_string(SS_LCTL(SS_TAP(X_S)));
            }
            return false;

        case H_WRITE:
            send_string(SS_LCTL(SS_TAP(X_X)SS_TAP(X_S)));
            return false;

        case H_DOUBLE_QUOTE__PLUS_MINUS:
            if (shift_pressed()) {
                push_mods();
                send_string(SS_LCTL(SS_TAP(X_X))SS_TAP(X_8)SS_TAP(X_KP_PLUS));
                pop_mods();
            } else {
                send_string("\"");
            }
            return false;

        case H_L_BRACE__L_CHEVRON:
            if (shift_pressed()) {
                push_mods();
                send_string("<");
                pop_mods();
            } else {
                send_string("{");
            }
            return false;

        case H_R_BRACE__R_CHEVRON:
            if (shift_pressed()) {
                push_mods();
                send_string(">");
                pop_mods();
            } else {
                send_string("}");
            }
            return false;

            // [main board] row 2
        case H_MARK:
            send_string(SS_LCTL(SS_TAP(X_SPC)));
            return false;

        case H_UNDO:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_LALT(SS_LSFT(SS_TAP(X_MINUS)))));
            } else {
                send_string(SS_LCTL(SS_LSFT(SS_TAP(X_MINUS))));
            }
            return false;

        case H_LEFT_PAREN__LEFT_BRACKET:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_TAP(X_LBRACKET)));
            } else {
                send_string("(");
            }
            return false;

        case H_RIGHT_PAREN__RIGHT_BRACKET:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_TAP(X_RBRACKET)));
            } else {
                send_string(")");
            }
            return false;

            // [main board] row 3
        case H_SELECT:
            send_string("[SELECT] key");
            return false;

        case H_DEBUG:
            send_string("[DEBUG] key");
            return false;

        case H_SEMI_COLON__BACK_TICK:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(
                           send_string("`");
                           );
            } else {
                send_string(";");
            }
            return false;

        /* case H_TICK__QUOTE: */
        /*     if (get_mods() & MODS_SHIFT_MASK) { */
        /*         // host configured for uk layout, for ansi, switch to X_QUOT */
        /*         send_string(SS_TAP(X_2)); */
        /*     } else { */
        /*         send_string("'"); */
        /*     } */
        /*     return false; */

        case H_LINE:
            // send_string(SS_LALT(SS_TAP(X_G)));
            return false;

        case H_PAGE:
            if (get_mods() & MODS_SHIFT_MASK) {
                CLEAN_MODS(send_string(SS_TAP(X_PGUP)));
            } else {
                send_string(SS_TAP(X_PGDN));
            }
            return false;

            // [main board] row 4
        case H_TTY:
            send_string("[TTY] key");
            return false;

        case H_LOCK:
            send_string("[LOCK] key");
            return false;

            // [main board] row 5
        case H_EOF:
            send_string(SS_LCTL(SS_TAP(X_END)));
            return false;

        case H_7BIT:
            send_string(SS_LCTL(SS_TAP(X_ENTER)));
            return false;

        case H_CIRCLE_SM:
            send_string(SS_LALT(SS_TAP(X_X)));
            return false;

        case H_HAND_LEFT:
            send_string(SS_LCTL(SS_LALT(SS_TAP(X_F))));
            return false;

        case H_HAND_RIGHT:
            send_string(SS_LCTL(SS_LALT(SS_TAP(X_B))));
            return false;

        case H_HAND_UP:
            send_string(SS_LCTL(SS_LALT(SS_TAP(X_U))));
            return false;

        case H_HAND_DOWN:
            send_string(SS_LCTL(SS_LALT(SS_TAP(X_D))));
            return false;

        case H_ROMAN_NUMERAL_I:
            send_string("I");
            return false;

        case H_ROMAN_NUMERAL_II:
            send_string("2");
            return false;

        case H_ROMAN_NUMERAL_III:
            send_string("III");
            return false;

        case H_ROMAN_NUMERAL_IV:
            send_string("IV");
            return false;

        case H_PASTE:
            send_string(SS_LCTL(SS_TAP(X_Y)));
            return false;

        case H_CUT:
            if (shift_pressed()) {
                CLEAN_MODS(send_string(SS_LALT(SS_TAP(X_W))));
            } else {
                send_string(SS_LCTL(SS_TAP(X_W)));
            }
            return false;

        // APL layer

        case H_APL_LOGICAL_AND:                 { send_emacs_unicode("22C0"); return false; }
        case H_APL_LOGICAL_OR:                  { send_emacs_unicode("22C1"); return false; }
        case H_APL_INTERSECT:                   { send_emacs_unicode("22C2"); return false; }
        case H_APL_UNION:                       { send_emacs_unicode("22C3"); return false; }
        case H_APL_SUBSET_OF:                   { send_emacs_unicode("2282"); return false; }
        case H_APL_SUPERSET_OF:                 { send_emacs_unicode("2283"); return false; }
        case H_APL_FORALL:                      { send_emacs_unicode("2200"); return false; }
        case H_APL_LEMNISCATE:                  { send_emacs_unicode("221E"); return false; }
        case H_APL_THERE_EXISTS:                { send_emacs_unicode("2203"); return false; }
        case H_APL_PARTIAL_DIFFERENTIAL:        { send_emacs_unicode("2202"); return false; }
        case H_APL_UP_TACK:                     { send_emacs_unicode("22A5"); return false; }
        case H_APL_DOWN_TACK:                   { send_emacs_unicode("22A4"); return false; }
        case H_APL_RIGHT_TACK:                  { send_emacs_unicode("22A2"); return false; }
        case H_APL_LEFT_TACK:                   { send_emacs_unicode("22A3"); return false; }
        case H_APL_UPWARDS_ARROW:               { send_emacs_unicode("2191"); return false; }
        case H_APL_DOWNARDS_ARROW:              { send_emacs_unicode("2193"); return false; }
        case H_APL_LEFTWARDS_ARROW:             { send_emacs_unicode("2190"); return false; }
        case H_APL_RIGHTWARDS_ARROW:            { send_emacs_unicode("2192"); return false; }
        case H_APL_LEFTRIGHT_ARROW:             { send_emacs_unicode("2194"); return false; }
        case H_APL_LEFT_FLOOR:                  { send_emacs_unicode("230A"); return false; }
        case H_APL_LEFT_CEILING:                { send_emacs_unicode("2308"); return false; }
        case H_APL_NOT_EQUAL_TO:                { send_emacs_unicode("2260"); return false; }
        case H_APL_ASYMPTOTICALLY_EQUAL_TO:     { send_emacs_unicode("2243"); return false; }
        case H_APL_NOT_ASYMPTOTICALLY_EQUAL_TO: { send_emacs_unicode("2261"); return false; }
        case H_APL_LESS_THAN_OR_EQUAL:          { send_emacs_unicode("2264"); return false; }
        case H_APL_GREATER_THAN_OR_EQUAL:       { send_emacs_unicode("2265"); return false; }

        // Symbol Layer
        // [main board] row 0
        case H_SYMBOL_CONTOUR_INTEGRAL:       { send_emacs_unicode("222E"); return false; }
        case H_SYMBOL_COPTIC_LC_DEI:          { send_emacs_unicode("03EF"); return false; }
        case H_SYMBOL_DOUBLE_DAGGER:          { send_emacs_unicode("2021"); return false; }
        case H_SYMBOL_NABLA:                  { send_emacs_unicode("2207"); return false; }
        case H_SYMBOL_CENT:                   { send_emacs_unicode("00A2"); return false; }
        case H_SYMBOL_DEGREE:                 { send_emacs_unicode("00B0"); return false; }
        case H_SYMBOL_APL_QUAD:               { send_emacs_unicode("2395"); return false; }
        case H_SYMBOL_DIVISION:               { send_emacs_unicode("00F7"); return false; }
        case H_SYMBOL_MULTIPLICATION:         { send_emacs_unicode("00D7"); return false; }
        case H_SYMBOL_PILCROW:                { send_emacs_unicode("00B6"); return false; }
        case H_SYMBOL_LARGE_CIRCLE:           { send_emacs_unicode("25EF"); return false; }
        case H_SYMBOL_HORIZONTAL_BAR:         { send_emacs_unicode("2015"); return false; }
        case H_SYMBOL_APPROXIMATELY_EQUAL_TO: { send_emacs_unicode("2248"); return false; }
        case H_SYMBOL_DOUBLE_VERTICAL_LINE:   { send_emacs_unicode("2016"); return false; }
        case H_SYMBOL_SQUARE_IMAGE_OF:        { send_emacs_unicode("228F"); return false; }
        case H_SYMBOL_SQUARE_ORIGINAL_OF:     { send_emacs_unicode("2290"); return false; }

        // [main board] row 1
        case H_SYMBOL_GREEK_THETA:                             { send_emacs_unicode_shift("03B8", "0398"); return false; }
        case H_SYMBOL_GREEK_OMEGA:                             { send_emacs_unicode_shift("03C9", "03A9"); return false; }
        case H_SYMBOL_GREEK_EPSILON:                           { send_emacs_unicode_shift("03B5", "0395"); return false; }
        case H_SYMBOL_GREEK_RHO:                               { send_emacs_unicode_shift("03C1", "03A1"); return false; }
        case H_SYMBOL_GREEK_TAU:                               { send_emacs_unicode_shift("03C4", "03A4"); return false; }
        case H_SYMBOL_GREEK_PSI:                               { send_emacs_unicode_shift("03C8", "03A8"); return false; }
        case H_SYMBOL_GREEK_UPSILON:                           { send_emacs_unicode_shift("03C5", "03A5"); return false; }
        case H_SYMBOL_GREEK_IOTA:                              { send_emacs_unicode_shift("03B9", "0399"); return false; }
        case H_SYMBOL_GREEK_OMICRON:                           { send_emacs_unicode_shift("03BF", "039F"); return false; }
        case H_SYMBOL_GREEK_PI:                                { send_emacs_unicode_shift("03C0", "03A0"); return false; }
        case H_SYMBOL_MATHEMATICAL_LEFT_WHITE_SQUARE_BRACKET:  { send_emacs_unicode("27E6"); return false; }
        case H_SYMBOL_MATHEMATICAL_RIGHT_WHITE_SQUARE_BRACKET: { send_emacs_unicode("27E7"); return false; }

        // [main board] row 2
        case H_SYMBOL_GREEK_ALPHA:    { send_emacs_unicode_shift("03B1", "0391"); return false; }
        case H_SYMBOL_GREEK_SIGMA:    { send_emacs_unicode_shift("03C3", "03A3"); return false; }
        case H_SYMBOL_GREEK_DELTA:    { send_emacs_unicode_shift("03B4", "0394"); return false; }
        case H_SYMBOL_GREEK_PHI:      { send_emacs_unicode_shift("03C6", "03A6"); return false; }
        case H_SYMBOL_GREEK_GAMMA:    { send_emacs_unicode_shift("03B3", "0393"); return false; }
        case H_SYMBOL_GREEK_ETA:      { send_emacs_unicode_shift("03B7", "0397"); return false; }
        case H_SYMBOL_GREEK_YOT:      { send_emacs_unicode_shift("03F3", "037F"); return false; }
        case H_SYMBOL_GREEK_KAPPA:    { send_emacs_unicode_shift("03BA", "039A"); return false; }
        case H_SYMBOL_GREEK_LAMDA:    { send_emacs_unicode_shift("03BB", "039B"); return false; }
        case H_SYMBOL_TWO_DOT_LEADER: { send_emacs_unicode("2025"); return false; }
        case H_SYMBOL_BULLET:         { send_emacs_unicode("2022"); return false; }

        // [main board] row 3
        case H_SYMBOL_GREEK_ZETA:           { send_emacs_unicode_shift("03B6", "0396"); return false; }
        case H_SYMBOL_GREEK_XI:             { send_emacs_unicode_shift("03BE", "039E"); return false; }
        case H_SYMBOL_GREEK_CHI:            { send_emacs_unicode_shift("03C7", "03A7"); return false; }
        case H_SYMBOL_GREEK_FINAL_SIGMA:    { send_emacs_unicode("03C2"); return false; }
        case H_SYMBOL_GREEK_BETA:           { send_emacs_unicode_shift("03B2", "0392"); return false; }
        case H_SYMBOL_GREEK_NU:             { send_emacs_unicode_shift("03BD", "039D"); return false; }
        case H_SYMBOL_GREEK_MU:             { send_emacs_unicode_shift("03BC", "039C"); return false; }
        case H_SYMBOL_MUCH_LESS_THAN:       { send_emacs_unicode("226A"); return false; }
        case H_SYMBOL_MUCH_GREATER_THAN:    { send_emacs_unicode("226B"); return false; }
        case H_SYMBOL_INTEGRAL:             { send_emacs_unicode("222B"); return false; }

        }
    }

    return true;
}

void led_set_keymap(uint8_t usb_led) {
    // stub
}
