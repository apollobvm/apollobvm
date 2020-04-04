#ifndef trajfactory_h
#define trajfactory_h

#include "Arduino.h"
#include "trajectory.h"

class TrajFactory {
  public:
    // Init traj with parameters.
    TrajFactory(int respitory_rate, float i_e_ratio, float hold_time, int delta_t);

    // Init
    TrajFactory();

    // TrajFactory rebuild functions
    Trajectory* build(int respitory_rate, float i_e_ratio, int setpoint, float hold_time, int delta_t);
    Trajectory* buildSetpoint(int setpoint);
    
    // Getters
    float getTotalTime();
    float getInhaleTime();
    float getExhaleTime();
    int getDeltaTime();
    int getLength();

  private:
    int _length;
    // Set trajectory array pointer to 0 to know to delete.
    int* _traj_pos = 0;

    int _respitory_rate;
    float _i_e_ratio;

    int _x_s;
    int _x_0;

    float _t_in;
    float _t_out;
    float _t_hold;
    float _t_tot;
    int _delta_t;




    // True value is increasing
    bool _direction;

    int calcTotalTime();

};
#endif
