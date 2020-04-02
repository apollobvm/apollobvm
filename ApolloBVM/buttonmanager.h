#ifndef buttonmanager_h
#define buttonmanager_h

#include "Arduino.h"

class ButtonManager {

  public:
    ButtonManager(int pin, bool highDefault);

    void poll();
    bool getButtonState();

  private:
    // Button pin for this manager
    int _button_pin;
    // Sets whether the button is normally high or not.
    bool _high_default;
    // Current press status of the button.
    bool _button_state;
    // Last state the button was in.
    bool _last_state;
    // Trigger variable to output only one true per press.
    bool _trigger;

    unsigned long _last_debounce;
    unsigned long _debounce_delay;
};

#endif
