#ifndef nhd_0420d3z_h
#define nhd_0420d3z_h

#include "Arduino.h"
#include "SoftwareSerial.h"

using byte = unsigned char;

class NhdDisplay {

  public:
    NhdDisplay(byte pin);

    void begin(int baud);

    void print(String st);
    void print(int val);
    void println(String st);
    //void print(int num);
    
    void setBacklight(byte brightness);
    void clearDisplay();
    void setCursor(byte cursor_position, byte line);

    void underlineOn();
    void underlineOff();

    void blinkingOn();
    void blinkingOff();

    void moveCursorRight();
    void moveCursorLeft();

    void backspace();
    void remove();

    String zeroPad(int in);

  private:
    SoftwareSerial* _serial_ptr;
    byte _row;
    byte _col;



};


#endif
