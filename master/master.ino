#include "panel.h"
#include "Wire.h"

#define SLAVE_ADDR 8

NhdDisplay display(3);
Encoder enc(5,6);
ButtonManager encoder_button(7, true);
ButtonManager stop_button(11, false);

// Default settings
VentSettings vs = {'X', 500, 12, 1, 3, 0, 30, 20, 0, 0, 0, false}; 

// String params
String* splash_text = new String[4];


String* warning_text = new String[4];
SplashPanel* splash_ptr;
SplashPanel* warning_ptr;
EditPanel* start_ptr;
EditPanel* apply_ptr;
RunningPanel* run_ptr;
PausePanel* pause_ptr;

Panel* cur_panel;

void setup()
{

  // Join i2c bus.
  Wire.begin();

  // Start serial debug connection.
  Serial.begin(9600);

  // Start display.
  display.begin(9600);
  display.clearDisplay();

  // Init slash text.
  splash_text[0] = "";
  splash_text[1] = "       Apollo";
  splash_text[2] = "        BVM";
  splash_text[3] = "";

  //Init warning text.
  warning_text[0] = "      WARNING: ";
  warning_text[1] = "      USE ADULT ";
  warning_text[2] = "        SIZED";
  warning_text[3] = "         BVM ";

  // Init panels.
  start_ptr = new EditPanel(&display, &enc, &encoder_button, &stop_button, &vs, "Confirm & Run?", &run_ptr, 0);
  warning_ptr = new SplashPanel(&display, &enc, &encoder_button, &stop_button, &vs, warning_text, 2000, &start_ptr);
  splash_ptr = new SplashPanel(&display, &enc, &encoder_button, &stop_button, &vs, splash_text, 2000, &warning_ptr);
  apply_ptr = new EditPanel(&display, &enc, &encoder_button, &stop_button, &vs, "Apply Changes?", &run_ptr, &pause_ptr);
  run_ptr = new RunningPanel(&display, &enc, &encoder_button, &stop_button, &vs, &apply_ptr, &pause_ptr);
  pause_ptr = new PausePanel(&display, &enc, &encoder_button, &stop_button, &vs, &start_ptr, &run_ptr);

  // Delay just cause.
  delay(100);

  // Set current panel to splash.
  cur_panel = splash_ptr;
  cur_panel->start();

}

void loop()
{
  // Poll button status.
  encoder_button.poll();
  stop_button.poll();

  // Update current panel.
  Panel* new_panel = cur_panel->update();

  // If we get a new panel, start and switch to it.
  if (new_panel != 0) {
    cur_panel = new_panel;
    cur_panel->start();
  }

  // Transmit to the device if necessary.
  if (vs.send)
    transmit();


}

void transmit() {

  // Transmit to the slave device.
  Wire.beginTransmission(SLAVE_ADDR);

  // Write the mode first.
  Wire.write(vs.mode);

  // Send settings to controller unit, if we're loading.
  if (vs.mode == 'L') {
      // TODO Build lookup table based on real information.
      int setpoint = map(vs.tidal_volume, 300, 650, 800, 1850);
      Wire.write(byte(setpoint >> 8));
      Wire.write(byte(setpoint & 0x00FF));
      Wire.write(byte(vs.respiration_rate));
      Wire.write(byte(vs.inhale));
      Wire.write(byte(vs.exhale));
      Wire.write(byte(vs.hold_seconds));
      Wire.write(byte(vs.hold_decimals));
      Wire.write(byte(vs.delta_time));

      // Set the mode to on as device will start.
      vs.mode = 'O';
  }

  // Send i2c message.
  Wire.endTransmission();

  // Sent settings so disable send.
  vs.send = false;


}

