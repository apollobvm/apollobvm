#include "panel.h"


Panel::Panel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr) :
  _disp_ptr(disp_ptr),
  _encoder_ptr(encoder_ptr),
  _em_button_ptr(em_button_ptr),
  _stop_button_ptr(stop_button_ptr),
  _vs_ptr(vs_ptr) {}

SplashPanel::SplashPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, String* text, int display_time, Panel** next_ptr) :
  Panel{disp_ptr, encoder_ptr, em_button_ptr, stop_button_ptr, vs_ptr},
  _next_d_ptr(next_ptr),
  _display_time(display_time),
  _text(text) {}

void SplashPanel::start() {

  // Dereference double pointer to panel.
  _next_ptr = *_next_d_ptr;

  // Clear display.
  _disp_ptr->clearDisplay();
  //_disp_ptr->print("hi");

  // Display each line.
  for (int i = 0; i < 4; i++) {
    _disp_ptr->setCursor(0, i);
    _disp_ptr->print(*(_text+i));
  }
}

Panel* SplashPanel::update() {

  // Wait for time and then pass next panel.
  delay(_display_time);
  return _next_ptr;
}

EditPanel::EditPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, String top_text, Panel** run_panel_ptr, Panel** stop_panel_ptr) :
  Panel{disp_ptr, encoder_ptr, em_button_ptr, stop_button_ptr, vs_ptr},
  _top_text(top_text),
  _run_panel_d_ptr(run_panel_ptr),
  _stop_panel_d_ptr(stop_panel_ptr) {
    // Build new encoder manager with 4 selections.
    _em_ptr = new EncoderManager(encoder_ptr, 4);
  }

void EditPanel::start() {

  // Dereference double pointer to panels.
  _run_panel_ptr = *_run_panel_d_ptr;
  if (_stop_panel_d_ptr != 0) {
    _stop_panel_ptr = *_stop_panel_d_ptr;
  } else {
    _stop_panel_ptr = 0;
  }

  // Start the selection encoder.
  _em_ptr->start();

  // Clear display.
  _disp_ptr->clearDisplay();

  // Write cursor.
  _disp_ptr->setCursor(0,0);
  _disp_ptr->print(">");

  // Write first line.
  _disp_ptr->setCursor(1, 0);
  _disp_ptr->print(_top_text);

  // Write second line and add default tidal volume value.
  _disp_ptr->setCursor(1, 1);
  _disp_ptr->print(_tv_text + _vs_ptr->tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _disp_ptr->setCursor(1, 2);
  _disp_ptr->print(_rr_text + _disp_ptr->zeroPad(_vs_ptr->respiration_rate) + _rr_units);
  
  // Write fourth line and add default i:e ratio.
  _disp_ptr->setCursor(1, 3);
  _disp_ptr->print(_i_e_text + _vs_ptr->inhale + ':' + _vs_ptr->exhale);

  // Mark that the user hasn't made a change.
  _made_change = false;
  
}

Panel* EditPanel::update() {

  // Update encoder manager for new positions.
   _em_ptr->poll();

  // Check if we have a non-zero stop panel pointer and return if button pushed.
  if (_stop_panel_ptr != 0 && _stop_button_ptr->getButtonState()) {
    return _stop_panel_ptr; 
  }

  // If we are not in edit mode and the button pushed, edit current selection.
  if (!_edit && _em_button_ptr->getButtonState()) {

    // Selected confirm settings, exit and run device.
    if (_row == 0) {

      if (_made_change) {
        // Reset time when endering edit panel.
        _vs_ptr->hours = 0;
        _vs_ptr->minute = 0;
        _vs_ptr->seconds = 0;
      }

      return _run_panel_ptr;
    }

    // Enable edit mode.
    _edit = true;

    // Set cursor to flash.
    _disp_ptr->blinkingOn();

    // Set encoder manager to give selections based on selected quantity.
    int num_selections;
    int starting_selection;

    switch (_row) {
      // Set selections for tidal volume.
      case 1:
        num_selections = (_max_tidal_volume - _min_tidal_volume) / _delta_tidal_volume + 1;
        starting_selection = (_vs_ptr->tidal_volume - _min_tidal_volume) / _delta_tidal_volume;
        break;
      // Set selections for respiration rate.
      case 2:
        num_selections = (_max_respiration_rate - _min_respiration_rate) / _delta_respiration_rate + 1;
        starting_selection = (_vs_ptr->respiration_rate - _min_respiration_rate) / _delta_respiration_rate;
        break;
      // Set selections for i:e ratio.
      case 3:
        num_selections = (_max_exhale - _min_exhale) / _delta_exhale + 1;
        starting_selection = (_vs_ptr->exhale - _min_exhale) / _delta_exhale;
        break;
    }

    // Update settings to encoder manager.
    _em_ptr->setNumOptions(num_selections);
    _em_ptr->setSelection(starting_selection);
    _old_selection = starting_selection;

    // Move cursor to over the arrow for the row.
    _disp_ptr->setCursor(0, _row);
    
  
  // If we are in edit mode and the button was not pushed, encoder movement changes value.
  } else if (_edit && !_em_button_ptr->getButtonState()) {

    if (_em_ptr->getSelection() != _old_selection){

      // Mark that the user made a change.
      _made_change = true;

      switch (_row) {
        // Edit tidal volume.
        case 1:
          // Calculate new tidal volume amount.
          _vs_ptr->tidal_volume = _min_tidal_volume + _em_ptr->getSelection() * _delta_tidal_volume;

          // Write to the display.
          _disp_ptr->setCursor(1 + _tv_text_length, 1);
          _disp_ptr->print(_vs_ptr->tidal_volume);
          break;
          
        // Edit respiration rate to new value.
        case 2:
          // Calculate new respitory rate.
          _vs_ptr->respiration_rate = _min_respiration_rate + _em_ptr->getSelection() * _delta_respiration_rate;
          
          // Write to the display.
          _disp_ptr->setCursor(1 + _rr_text_length, 2);
          _disp_ptr->print(_disp_ptr->zeroPad(_vs_ptr->respiration_rate));
          break;

        // Edit i:e ratio.
        case 3:
          // Calculate new i:e ratio.
          _vs_ptr->exhale = _min_exhale + _em_ptr->getSelection() * _delta_exhale;

          // Write to the display.
          _disp_ptr->setCursor(1 + _i_e_text_length, 3);
          _disp_ptr->print(_vs_ptr->exhale);
          break;
      }

      // Set cursor back to row so blinking continues.
      _disp_ptr->setCursor(0,_row);

      // Set old selection to the selection from this cycle.
      _old_selection = _em_ptr->getSelection();
    }

  // If we are in edit mode and the button was pushed, exit edit mode.
  } else if (_edit && _em_button_ptr->getButtonState()) {
    
    // Disable edit mode.
    _edit = false;

    // Stop cursor blinking.
    _disp_ptr->blinkingOff();

    // Set encoder manager back to 4 selections and to the row.
    _em_ptr->setNumOptions(4);
    _em_ptr->setSelection(_row);

  // If we are not in edit mode and the button was not pushed, move cursoe.
  } else if (!_edit && !_em_button_ptr->getButtonState()) {
   
    // Check if we've moved the cursor.
    if (_em_ptr->getSelection() != _row) {
      // Remove old cursor on the display.
      _disp_ptr->setCursor(0, _row);
      _disp_ptr->remove();

      // Write the new cursor on the display.
      _disp_ptr->setCursor(0,_em_ptr->getSelection());
      _disp_ptr->print(">");

      // Set the old row to the current row.
      _row = _em_ptr->getSelection();
    }
  }

  return 0;
}

RunningPanel::RunningPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, Panel** apply_panel_ptr, Panel** stop_panel_ptr) :
  Panel{disp_ptr, encoder_ptr, em_button_ptr, stop_button_ptr, vs_ptr},
  _apply_panel_d_ptr(apply_panel_ptr), 
  _stop_panel_d_ptr(stop_panel_ptr) {}

String RunningPanel::formatTime() {
  return _disp_ptr->zeroPad(_vs_ptr->hours) + ":" + _disp_ptr->zeroPad(_vs_ptr->minute) + ":" + _disp_ptr->zeroPad(_vs_ptr->seconds);
}

void RunningPanel::start() {
 
  // Dereference double pointer to panels.
  _apply_panel_ptr = *_apply_panel_d_ptr;  
  _stop_panel_ptr = *_stop_panel_d_ptr;


  // Change mode to load new settings.
  _vs_ptr->mode = 'L';
  _vs_ptr->send = true;

  // Clear display.
  _disp_ptr->clearDisplay();

  // Write first line.
  _disp_ptr->setCursor(1, 0);
  _disp_ptr->print(_top_text + formatTime());

  // Write second line and add default tidal volume value.
  _disp_ptr->setCursor(1, 1);
  _disp_ptr->print(_tv_text + _vs_ptr->tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _disp_ptr->setCursor(1, 2);
  _disp_ptr->print(_rr_text + _disp_ptr->zeroPad(_vs_ptr->respiration_rate) + _rr_units);
  
  // Write fourth line and add default i:e ratio.
  _disp_ptr->setCursor(1, 3);
  _disp_ptr->print(_i_e_text + _vs_ptr->inhale + ':' + _vs_ptr->exhale);
}

Panel* RunningPanel::update() {
  
  // Check if stop button pushed and return stop panel if pushed.
  if (_stop_button_ptr->getButtonState()) {
    return _stop_panel_ptr; 
  }

  // Check if encoder button pushed and return apply panel if pushed.
  if (_em_button_ptr->getButtonState()) {
    return _apply_panel_ptr;
  }

  // Update and display time if a second has passed.
  if (!(millis() % 1000)) 
  {
    // Increase time.
    _vs_ptr->seconds++;  // increase seconds
    if (_vs_ptr->seconds == 60)  // If it's been a minute
    {
      _vs_ptr->seconds = 0;  // start over seconds
      _vs_ptr->minute++;  // Increase minutes
      if (_vs_ptr->minute == 60)  // If it's been an hour
      {
        _vs_ptr->minute = 0;  // start over minutes
        _vs_ptr->hours++;  // increase hours

        // Assuming device will not be run for more than 99 hours.
      }
    }

    // Update time on display.
    // Add 1 to the text length to accoud for empty column for cursor.
    _disp_ptr->setCursor(_text_length_to_time + 1, 0);
    _disp_ptr->print(formatTime());
  }
  return 0;
}

// TODO add constructor etc
PausePanel::PausePanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, Panel** apply_panel_ptr, Panel** run_panel_ptr) :
  Panel{disp_ptr, encoder_ptr, em_button_ptr, stop_button_ptr, vs_ptr},
  _apply_panel_d_ptr(apply_panel_ptr), 
  _run_panel_d_ptr(run_panel_ptr) {
    // Build new encoder manager with 2 selections.
    _em_ptr = new EncoderManager(encoder_ptr, 2);
}

void PausePanel::start() {
  
  // Dereference double pointer to panels.
  _apply_panel_ptr = *_apply_panel_d_ptr;  
  _run_panel_ptr = *_run_panel_d_ptr;

  // Start a new encoder manager.
  _em_ptr->start();
  _em_ptr->setSelection(0);

  // Change mode to stop operation.
  _vs_ptr->mode = 'X';
  _vs_ptr->send = true;

  // Reset the selection.
  _selection = 0;

  // Clear display.
  _disp_ptr->clearDisplay();

  // Write first line.
  _disp_ptr->setCursor(1, 0);
  _disp_ptr->print(_top_before_time); 
  _disp_ptr->print(_disp_ptr->zeroPad(_vs_ptr->hours) + ":" + _disp_ptr->zeroPad(_vs_ptr->minute) + ":" + _disp_ptr->zeroPad(_vs_ptr->seconds));
  _disp_ptr->print(_top_after_time);

  // Write second line and add default tidal volume value.
  _disp_ptr->setCursor(1, 1);
  _disp_ptr->print(_tv_text + _vs_ptr->tidal_volume + _tv_units);

  // Write third line and add default respiration rate value.
  _disp_ptr->setCursor(1, 2);
  _disp_ptr->print(_rr_text + _vs_ptr->respiration_rate + _rr_units);
  
  // Write fourth line and add default i:e ratio.
  _disp_ptr->setCursor(1, 3);
  _disp_ptr->print(_i_e_text + _vs_ptr->inhale + ':' + _vs_ptr->exhale);

  // Set cursor over R in Run.
  _disp_ptr->setCursor(_text_length_to_run, 0);
  _disp_ptr->blinkingOn();

}

Panel* PausePanel::update() {
  
  // Poll encoder for updates.
  _em_ptr->poll();

  // If encoder button pushed over run, exit and start running.
  if (_selection == 0 && _em_button_ptr->getButtonState()) {
    _disp_ptr->blinkingOff();
    return _run_panel_ptr;
  } else if (_selection == 1 && _em_button_ptr->getButtonState()) {
    _disp_ptr->blinkingOff();
    return _apply_panel_ptr;
  }

  
  if (_em_ptr->getSelection() != _selection) {

    switch (_em_ptr->getSelection()) {
      // Cursor moved to run.
      case 0:
        _disp_ptr->setCursor(_text_length_to_run, 0);
        break;
      // Cursor moved to edit.
      case 1:
        _disp_ptr->setCursor(_text_length_to_edit, 0);
        break;
    }
    
    // Set the current selection to the old one.
    _selection = _em_ptr->getSelection();
  }
  return 0;
}
