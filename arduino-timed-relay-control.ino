//
// Spot welder relay control.
//

#include "ssd1306.h"

const int PIN_RELAY = A3;
const int PIN_BTN_ENABLE = 3;
const int PIN_BTN_SET = 5;

const int BTN_ENABLE_PRESSED_STATE = LOW;
const int BTN_SET_PRESSED_STATE = LOW;

const int BTN_ENABLE_DEBOUNCE_MS = 20;
const int BTN_SET_DEBOUNCE_MS = 20;

bool is_relay_active = false;
bool is_set_active = false;

int set_step = 10;
int relay_active_time = 60;

int last_btn_enable_state = !BTN_ENABLE_PRESSED_STATE;
int last_btn_set_state = !BTN_SET_PRESSED_STATE;
unsigned long last_btn_enable_press_ms = 0;
unsigned long last_btn_set_press_ms = 0;

unsigned long ms = 0;

void display(const char *str) {
  ssd1306_clearScreen();
  ssd1306_printFixed(0, 0, str, STYLE_NORMAL);
}

void display_relay_active_time() {
  display((String("Relay time: ") + String(relay_active_time) + String(" ms")).c_str());
}

void enable_relay() {
    ssd1306_invertMode();
    display("");

    digitalWrite(PIN_RELAY, HIGH);
    delay(relay_active_time);
    digitalWrite(PIN_RELAY, LOW);
    ssd1306_normalMode();
    display_relay_active_time();
}

int debounce_btn(const int pin, const unsigned long debounce_ms, const int *pressed_state, int *prev_state, unsigned long *prev_state_ms) {
  int state = digitalRead(pin);
  if (state == *prev_state) {
    if (ms - *prev_state_ms > debounce_ms && (pressed_state == NULL || state == *pressed_state)) {
      return state;
    }
  }
  else {
    *prev_state = state;
    *prev_state_ms = ms;
  }
  return -1;
}

void setup() {
  Serial.begin(115200);
  Serial.println("SETUP");
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BTN_ENABLE, INPUT_PULLUP);
  pinMode(PIN_BTN_SET, INPUT_PULLUP);

  ssd1306_128x32_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_flipHorizontal(1);
  ssd1306_flipVertical(1);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  display_relay_active_time();
}

void loop() {
  ms = millis();

  int btn_state = debounce_btn(PIN_BTN_ENABLE, BTN_ENABLE_DEBOUNCE_MS, NULL, &last_btn_enable_state, &last_btn_enable_press_ms);
  if (btn_state == BTN_ENABLE_PRESSED_STATE) {
    if (!is_relay_active) {
      is_relay_active = true;
      enable_relay();
    }
  }
  else {
    is_relay_active = false;
  }
  
  btn_state = debounce_btn(PIN_BTN_SET, BTN_SET_DEBOUNCE_MS, &BTN_SET_PRESSED_STATE, &last_btn_set_state, &last_btn_set_press_ms);
  if (btn_state != -1) {
    if (!is_set_active) {
      is_set_active = true;
      if (is_relay_active) {
        relay_active_time -= set_step;
        if (relay_active_time < 0) {
          relay_active_time = 0;
        }
      }
      else {
        relay_active_time += set_step;
      }
      display_relay_active_time();
    }
  }
  else {
    is_set_active = false;
  }
}
