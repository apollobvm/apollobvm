#include "encodermanager.h"

EncoderManager::EncoderManager(Encoder* encoder_ptr, int num_options) :
  _encoder_ptr(encoder_ptr),
  _selection(0),
  _num_options(num_options) {}

EncoderManager::EncoderManager(Encoder* encoder_ptr, int num_options, int starting) :
  _encoder_ptr(encoder_ptr),
  _selection(starting),
  _num_options(num_options) {}

void EncoderManager::start() {

  // Set the encoder to the current selection.
  _encoder_ptr->write(COUNTS_PER_STEP * _selection);

  // Create a new long variable to hold position.
  _pos_ptr = new long(COUNTS_PER_STEP * _selection);
}

void EncoderManager::poll() {

  // Read the current position.
  long new_pos = _encoder_ptr->read();

  // If the positions have changed, evaluate the new selection.
  if (new_pos != *_pos_ptr) {

    // Keep values in the positive range.
    if (new_pos < 0){

      // Write to position 0 if we're too low.
      _encoder_ptr->write(0);
      new_pos = 0;

    // Keep values below the max for the number of selections we have.
    } else if (new_pos > _num_options * COUNTS_PER_STEP) {
      _encoder_ptr->write(_num_options * COUNTS_PER_STEP);
      new_pos = _num_options * COUNTS_PER_STEP;
    }

    // Set position to new position.
    *_pos_ptr = new_pos;

    // Find which selection our position is in.
    for (int i = 0; i < _num_options; i++) {
      if (i * COUNTS_PER_STEP <= new_pos && new_pos <= (i+1) * COUNTS_PER_STEP) {
        _selection = i;
        break;
      }
    }
  }
}

int EncoderManager::getSelection() {
  return _selection;
}

void EncoderManager::setSelection(int selection) {
  // Set new selection.
  _selection = selection;

  // Write the encoder to the new position.
  _encoder_ptr->write(COUNTS_PER_STEP * _selection);

  // Delete and make new long with new position.
  delete _pos_ptr;
  _pos_ptr = new long(COUNTS_PER_STEP * _selection);
}

void EncoderManager::setNumOptions(int num_options) {
  _num_options = num_options;
}

void EncoderManager::close() {
  delete _pos_ptr;
}
