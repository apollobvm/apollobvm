#include <Wire.h>
#include <Servo.h>
#include <trajfactory.h>

#define SLAVE_ADDR 8
#define SERVO_MIN 2490
#define SERVO_MAX 500

TrajFactory tf = TrajFactory();
Trajectory* traj_ptr = 0;
Trajectory* temp  = 0;

int rr;
float ie;
int setpoint;
float hold;
int delta_t;

Servo servo;

char state;

void setup() {
  Wire.begin(SLAVE_ADDR);                // join i2c bus with address #8
  Wire.onReceive(recieveTraj); // register event
  Serial.begin(9600);           // start serial for output

  // Attach servo
  servo.attach(9);
  state = 'X';
}

void loop() {

  /*Serial.println(state);*/
  switch (state) {
    // Device is on, continue following trajectory.
    case 'O': 
      moveTo(traj_ptr->nextStep(), traj_ptr->getDeltaTime());
      break;
    // Device has been instructed to shutdown.
    case 'X':
      stop();
      break;
    // Device has recieved a new trajectory, load and start.
    case 'L':

      // Stop motion.
      stop();

      // If assigned ptr, delete contents
      if (traj_ptr != 0) {
        delete traj_ptr;
      }

      // Build new trajectory
      traj_ptr = tf.build(rr, ie, setpoint, hold, delta_t);

      // Set device on.
      state = 'O';
      break;

  }

}

void moveTo(int pos, int delta_t){
  /*Serial.println(pos);*/
  servo.writeMicroseconds(SERVO_MIN-pos);
  delay(delta_t);
}

void stop(){
  
  // If we have already generated a traj, follow it till the end.
  if (traj_ptr != 0) {
    while (traj_ptr->getCurrentStep() != 0) {
      moveTo(traj_ptr->nextStep(), traj_ptr->getDeltaTime());
    }
  // If not, got to 0 position.
  } else {
    moveTo(SERVO_MAX, 100);
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void recieveTraj(int num_entries) {

  // Read the state from incoming transmission.
  char recieved_state = Wire.read();

  // Either change machine state or load new settings.
  switch (recieved_state) {
    case 'X':
      state = 'X';
      break;
    case 'L':
      state = 'L';

      Serial.println("Loading...");
      // Load new params
      byte high = Wire.read();
      byte low = Wire.read();
      setpoint = (high << 8) | low;
      Serial.println(setpoint);

      rr = Wire.read();
      Serial.println(rr);

      byte inhale = Wire.read();
      byte exhale = Wire.read();
      ie = float(inhale) / exhale;
      Serial.println(ie);

      byte hold_s = Wire.read();
      byte hold_dec = Wire.read();
      hold = hold_s + hold_dec/100.0;
      Serial.println(hold);

      delta_t = Wire.read();
      Serial.println(delta_t);


      break;
  }
}
