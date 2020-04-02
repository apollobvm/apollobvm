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
  _encoder_ptr->write(COUNTS_PER_STEP * _selection);
  _pos_ptr = new long(COUNTS_PER_STEP * _selection);
}

void EncoderManager::poll() {
  long new_pos = _encoder_ptr->read();
  if (new_pos != *_pos_ptr) {

    if (new_pos < 0){
      _encoder_ptr->write(0);
      new_pos = 0;
    } else if (new_pos > _num_options * COUNTS_PER_STEP) {
      _encoder_ptr->write(_num_options * COUNTS_PER_STEP);
      new_pos = _num_options * COUNTS_PER_STEP;
    }
    *_pos_ptr = new_pos;

    for (int i = 0; i < _num_options; i++) {
      if (i * COUNTS_PER_STEP <= new_pos && new_pos <= (i+1) * COUNTS_PER_STEP) {
        _selection = i;
        break;
      }
    }

  }// Variables will change:
}

int EncoderManager::getSelection() {
  return _selection;
}

void EncoderManager::setSelection(int selection) {
  _selection = selection;
  _encoder_ptr->write(COUNTS_PER_STEP * _selection);
  _pos_ptr = new long(COUNTS_PER_STEP * _selection);
}

void EncoderManager::setNumOptions(int num_options) {
  _num_options = num_options;
}

void EncoderManager::close() {
  delete _encoder_ptr;
}
