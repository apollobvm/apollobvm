#include "Arduino.h"
#include "nhd_0420d3z.h"


NhdDisplay::NhdDisplay(byte pin):
  _row(0),
  _col(0) {
  _serial_ptr = new SoftwareSerial(2, pin);
}

void NhdDisplay::begin(int baud) {
  // Start Serial connection with default baud.
  _serial_ptr->begin(9600);
}

void NhdDisplay::print(String st) {
  _serial_ptr->print(st);
}

void NhdDisplay::print(int val) {
  _serial_ptr->print(String(val));
}

void NhdDisplay::println(String st) {
  _serial_ptr->println(st);
}

void NhdDisplay::setBacklight(byte brightness) {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command for setting brightness.
  _serial_ptr->write(0x53);
  // Send brightness byte.
  _serial_ptr->write(brightness);
}

void NhdDisplay::clearDisplay() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to clear the screen.
  _serial_ptr->write(0x51);
}

void NhdDisplay::setCursor(byte cursor_position, byte line) {

  byte to_write = cursor_position;

  // Change the write value based on the line.
  // This is needed as the display write line 1 to 3 to 2 to 4.
  switch (line) {
    case 1:
      to_write += 0x40;
      break;
    case 2:
      to_write += 0x14;
      break;
    case 3:
      to_write += 0x54;
      break;
  }
  
  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor.
  _serial_ptr->write(0x45);
  // Send byte for cursor position.
  _serial_ptr->write(to_write);
}

void NhdDisplay::underlineOn() {
  
  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline on.
  _serial_ptr->write(0x47);
}

void NhdDisplay::underlineOff() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn underline off.
  _serial_ptr->write(0x48);
}

void NhdDisplay::blinkingOn() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn blinking on.
  _serial_ptr->write(0x4B);
}

void NhdDisplay::blinkingOff() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to turn blinking off.
  _serial_ptr->write(0x4C);
}

void NhdDisplay::moveCursorRight() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor right by one.
  _serial_ptr->write(0x4A);
}

void NhdDisplay::moveCursorLeft() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move the cursor left by one.
  _serial_ptr->write(0x49);
}

void NhdDisplay::backspace() {

  // Send special command byte.
  _serial_ptr->write(0xFE);
  // Send command to move cursor left and remove.
  _serial_ptr->write(0x4E);
}

void NhdDisplay::remove() {
 
  // Together these delete the current character.
  moveCursorRight();
  backspace();
}

String NhdDisplay::zeroPad(int in) {
  // This helper function ensures integers have 2 decimal places.
  
  // Make a temp char array to hold the 2 digits.
  char digit[2];

  // Pad with a 0 if integer is 1 digit.
  sprintf(digit, "%02d", in);

  return String(digit);
}
