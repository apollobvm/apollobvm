#ifndef nhd_0420d3z_h
#define nhd_0420d3z_h

#include "Arduino.h"
#include "SoftwareSerial.h"

using byte = unsigned char;

class NhdDisplay {

  public:
    NhdDisplay(byte pin);

    // Methods for beggining serial communication.
    void begin(int baud);

    // Methods for printing.
    void print(String st);
    void print(int val);
    void println(String st);
    
    // Methods for display adjustments.
    void setBacklight(byte brightness);
    void clearDisplay();
    void setCursor(byte cursor_position, byte line);

    // Methods for underline.
    void underlineOn();
    void underlineOff();

    // Methods for blinking.
    void blinkingOn();
    void blinkingOff();

    // Methods for single character shifting.
    void moveCursorRight();
    void moveCursorLeft();

    // Methods for deleting characters.
    void backspace();
    void remove();

    // Helper function to pad integers to 2 digits.
    String zeroPad(int in);

  private:

    // Pointer to serial interface.
    SoftwareSerial* _serial_ptr;

    // Current row and column of the cursor.
    byte _row;
    byte _col;



};


#endif
