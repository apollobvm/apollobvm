#ifndef encodermanager_h
#define encodermanager_h

#include "Arduino.h"
#include "Encoder.h"

class EncoderManager {

  public:
    EncoderManager(Encoder* encoder_ptr, int num_options);
    EncoderManager(Encoder* encoder_ptr, int num_options, int starting);

    void start();
    void poll();
    void close();

    int getSelection();
    bool getButtonState();

    void setNumOptions(int num_options);
    void setSelection(int selection);

  private:

    const byte COUNTS_PER_STEP = 4;

    Encoder* _encoder_ptr;
    long* _pos_ptr;
    int _selection;
    int _num_options;
    bool _button_state;
    int _button_pin;

};

#endif
