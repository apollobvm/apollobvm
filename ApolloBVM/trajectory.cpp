#include "Arduino.h"
#include "trajectory.h"

Trajectory::Trajectory(int delta_t, int* positions, int length):
  _delta_t(delta_t),
  _cur_step(0),
  _length(length){

    _traj_pos = positions;
  }

int Trajectory::nextStep() {

  // Find current position and duration
  int next =  *(_traj_pos + _cur_step);

  // Move the counter forward to the next step
  _cur_step++;

  if(_cur_step >= _length) {
    _cur_step = 0;
  }

  return next;
}

int Trajectory::getLength() {
  return _length;
}

int Trajectory::getCurrentStep() {
  return _cur_step;
}

int Trajectory::getDeltaTime() {
  return _delta_t;
}
