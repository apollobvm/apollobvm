#ifndef ventsettings_h
#define ventsettings_h

typedef struct  {

  // Current slave mode.
  char mode;

  // Ventilation settings.
  int tidal_volume;
  int respiration_rate;
  int inhale;
  int exhale;
  int hold_seconds;
  int hold_decimals;

  // Time between trajectory points.
  int delta_time;

  // Current ventilation time.
  int hours;
  int minute;
  int seconds;

  // Boolean to indicate to send settings.
  bool send;
} VentSettings;
#endif
