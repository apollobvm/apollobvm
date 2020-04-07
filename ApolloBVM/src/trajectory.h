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
    
    // Current step in the trajectory.
    int _cur_step;

    // Length of the trajectory.
    int _length;

    // Pointer to array of positions.
    int* _traj_pos;

    // Delta time between positions.
    int _delta_t;
};
#endif
