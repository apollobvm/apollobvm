#ifndef ventsettings_h
#define ventsettings_h

typedef struct  {

  char mode;

  int tidal_volume;
  int respiration_rate;
  int inhale;
  int exhale;
  int hold_seconds;
  int hold_decimals;

  int delta_time;

  int hours;
  int minute;
  int seconds;

  bool send;
} VentSettings;
#endif
