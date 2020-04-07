#ifndef trajfactory_h
#define trajfactory_h

#include "Arduino.h"
#include "trajectory.h"

class TrajFactory {
  public:
    // Init traj with parameters.
    TrajFactory(int respitory_rate, float i_e_ratio, float hold_time, int delta_t);

    // Init.
    TrajFactory();

    // TrajFactory rebuild methods.
    Trajectory* build(int respitory_rate, float i_e_ratio, int setpoint, float hold_time, int delta_t);
    Trajectory* buildSetpoint(int setpoint);
    
    // Getters
    float getTotalTime();
    float getInhaleTime();
    float getExhaleTime();
    int getDeltaTime();
    int getLength();

  private:

    // Length of trajectory to generate.
    int _length;

    // Trajectory array pointer, set initially to 0 to know to delete.
    int* _traj_pos = 0;

    // Ventilation settings.
    int _respitory_rate;
    float _i_e_ratio;

    // Variables to hold positions of trajectory.
    int _x_s;
    int _x_0;

    // Variables to hold timing keypoints in trajectory
    float _t_in;
    float _t_out;
    float _t_hold;
    float _t_tot;
    int _delta_t;
};
#endif
