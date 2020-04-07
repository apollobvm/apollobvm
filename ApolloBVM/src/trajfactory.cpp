#include "Arduino.h"
#include "trajfactory.h"

TrajFactory::TrajFactory(int respitory_rate, float i_e_ratio, float hold_time, int delta_t):
  _respitory_rate(respitory_rate),
  _i_e_ratio(i_e_ratio),
  _t_hold(hold_time),
  _delta_t(delta_t){}

TrajFactory::TrajFactory(){}

Trajectory* TrajFactory::buildSetpoint(int setpoint){
  
  // Calculate total cycle time based on respitory rate.
  _t_tot = 60.0/_respitory_rate;

  // Calculate breath cycle times.
  _t_out = (_t_tot - _t_hold) / (_i_e_ratio + 1);
  _t_in = _i_e_ratio * _t_out;

  // Calculate kinematics for curve generation.
  float x_h = (_x_s + _x_0)/2.0;
  float v_in = 2*((_x_s - _x_0)/_t_in);
  float a_in = (v_in * v_in)/(_x_s-_x_0);
  float v_out = 2*((_x_s - _x_0)/_t_out);
  float a_out = (v_out * v_out)/(_x_s-_x_0);

  // Calculate the length of the new trajectory.
  _length = int (_t_tot * (1000.0 / _delta_t));

  // If there's a trajectory saved, delete it.
  if (_traj_pos != 0) {
    delete [] _traj_pos; 
  }

  // Create new array for trajectory.
  _traj_pos = new int[_length];

  // Calculate the timing of keypoints in the trajectory.
  int in_concave = int (1000*(_t_in/2.0));
  int in_convex = int (1000*_t_in);
  int hold_end = int (1000*(_t_in + _t_hold));
  int out_convex  = int (1000*(_t_in + _t_hold + _t_out/2.0));

  // Generate trajectory.
  for (int i = 0; i < _length; i++) {
    // Find trajectory time in ms.
    int t = _delta_t * i;
    
    // Calculate in stroke concave portion.
    if (t < in_concave) {
      *(_traj_pos + i) = _x_0 + a_in*(t/1000.0)*(t/1000.0) / 2;

    // Calculate in stroke convex portion.
    } else if (t >= in_concave && t < in_convex) {
      float cur_t = (t - in_concave)/1000.0;
      *(_traj_pos + i) = x_h + v_in*cur_t - a_in*cur_t*cur_t/2;

    // Calculate hold position.
    } else if (t >= in_convex && t < hold_end) {
        *(_traj_pos + i) = _x_s;

    // Calculate out stroke convex portion.
    } else if (t >= hold_end && t < out_convex) {
      float cur_t = (t - hold_end)/1000.0;
      *(_traj_pos + i) = _x_s - a_out*cur_t*cur_t / 2;
    
    // Calculate out stroke concave portion.
    } else {
      float cur_t = (t - out_convex)/1000.0;
      *(_traj_pos + i) = x_h - v_out*cur_t + (a_out*cur_t*cur_t)/2;
      //Serial.println(*(_traj_pos +i));
    }
  }

  // Create and return trajectory.
  return new Trajectory(_delta_t, _traj_pos, _length);
}

Trajectory* TrajFactory::build(int respitory_rate, float i_e_ratio, int setpoint, float hold_time, int delta_t){

  // Update internal parameters.
  _respitory_rate = respitory_rate;
  _i_e_ratio = i_e_ratio;
  _x_s = setpoint;
  _t_hold = hold_time;
  _delta_t = delta_t;

  return buildSetpoint(setpoint);
}

// Getters.
float TrajFactory::getTotalTime(){
  return _t_tot;
}

float TrajFactory::getInhaleTime(){
  return _t_in;
}

float TrajFactory::getExhaleTime(){
  return _t_out;
}

int TrajFactory::getDeltaTime(){
  return _length;
}

int TrajFactory::getLength(){
  return _length;
}
