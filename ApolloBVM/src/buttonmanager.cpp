#include "buttonmanager.h"


ButtonManager::ButtonManager(int pin, bool high_default):
  _last_state(false),
  _debounce_delay(100),
  _button_pin(pin),
  _high_default(high_default) {}


void ButtonManager::poll() {

  // Set the reading false by default
  bool reading = false;

  // Set the trigger variable to false by default.
  _trigger = false;

  // Set different logic for different default button states.
  if (_high_default) {
    if (digitalRead(_button_pin) == LOW){
      reading = true;
    }
  } else {
    if (digitalRead(_button_pin) == HIGH){
      reading = true;
    }
  }

  // If the switch changed, due to noise or pressing.
  if (reading != _last_state) {
    // Reset the debouncing timer.
    _last_debounce = millis();
  }

  // If the reading has been there longer than the debounce, we accept its state.
  if ((millis() - _last_debounce) > _debounce_delay) {

    // If the button state has changed.
    if (reading != _button_state) {
      _button_state = reading;

      // We allow the button to only be triggered once per press.
      if (_button_state) {
        _trigger = true;
      }
    }
  }

  // Save the previous reading for the next poll.
  _last_state = reading;

}

bool ButtonManager::getButtonState(){
  // Return the toggle state.
  return _trigger;
}


