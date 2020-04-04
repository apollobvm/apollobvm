#include "Arduino.h"
#include "nhd_0420d3z.h"


NhdDisplay::NhdDisplay(byte pin):
  _row(0),
  _col(0) {
  _serial_ptr = new SoftwareSerial(2, pin);
}

void NhdDisplay::begin(int baud) {
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
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x53);  // send the backlight command
  _serial_ptr->write(brightness);  // send the brightness value
}

void NhdDisplay::clearDisplay() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x51);  // send the clear screen command
}

void NhdDisplay::setCursor(byte cursor_position, byte line) {
  byte to_write = cursor_position;

  // Change the write value based on the line.
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
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x45);  // send the set cursor command
  _serial_ptr->write(to_write);  // send the cursor position
}

void NhdDisplay::underlineOn() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x47);  // send the clear screen command
}

void NhdDisplay::underlineOff() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x48);  // send the clear screen command
}

void NhdDisplay::blinkingOn() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x4B);  // send the clear screen command
}

void NhdDisplay::blinkingOff() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x4C);  // send the clear screen command
}

void NhdDisplay::moveCursorRight() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x4A);  // send the clear screen command
}

void NhdDisplay::moveCursorLeft() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x49);  // send the clear screen command
}

void NhdDisplay::backspace() {
  _serial_ptr->write(0xFE);  // send the special command
  _serial_ptr->write(0x4E);  // send the clear screen command
}

void NhdDisplay::remove() {
  moveCursorRight();
  backspace();
}

String NhdDisplay::zeroPad(int in) {
  char digit[2];
  sprintf(digit, "%02d", in);
  return String(digit);
}
