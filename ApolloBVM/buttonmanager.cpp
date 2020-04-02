#include "buttonmanager.h"



ButtonManager::ButtonManager(int pin, bool high_default):
  _last_state(false),
  _debounce_delay(100),
  _button_pin(pin),
  _high_default(high_default) {}


void ButtonManager::poll() {

  bool reading = false;

  // Set the trigger variable to false by default.
  _trigger = false;

  if (_high_default) {
    if (digitalRead(_button_pin) == LOW){
      reading = true;
    }
  } else {
    if (digitalRead(_button_pin) == HIGH){
      reading = true;
    }
  }


  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != _last_state) {
    // reset the debouncing timer
    _last_debounce = millis();
  }

  if ((millis() - _last_debounce) > _debounce_delay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != _button_state) {
      _button_state = reading;

      if (_button_state) {
        _trigger = true;
      }
    }

  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  _last_state = reading;

}

bool ButtonManager::getButtonState(){
  //return _button_state;
  return _trigger;
}


