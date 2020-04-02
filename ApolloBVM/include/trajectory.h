#ifndef trajectory_h
#define trajectory_h

#include "Arduino.h"

// Single byte integer alias
//using byte = unsigned char;
//using int =  unsigned int;

class Trajectory {
  public:
    Trajectory(int delta_t, int* positions, int length);

    // Methods
    int nextStep();

    // Getters
    int getLength();
    int getCurrentStep();
    int getDeltaTime();

  private:
    int _cur_step;
    int _length;
    int* _traj_pos;
    int _delta_t;

};
#endif
