#ifndef encodermanager_h
#define encodermanager_h

#include "Arduino.h"
#include "Encoder.h"

class EncoderManager {

  public:
    EncoderManager(Encoder* encoder_ptr, int num_options);
    EncoderManager(Encoder* encoder_ptr, int num_options, int starting);

    // Methods to manage state.
    void start();
    void poll();
    void close();

    // Getters.
    int getSelection();
    bool getButtonState();

    // Setters.
    void setNumOptions(int num_options);
    void setSelection(int selection);

  private:

    // Number of counts per discrete encoder step.
    const byte COUNTS_PER_STEP = 4;

    // Pointer to encoder interface for master.
    Encoder* _encoder_ptr;

    // Pointer to position, deleted on close.
    long* _pos_ptr;

    // Internal
    int _selection;
    int _num_options;

};

#endif
