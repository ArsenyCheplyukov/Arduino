#include <Thread.h>
#include <math.h>
#include <ServoDriverSmooth.h>

const double pi                      = 3.1415926535897932;
const double lever_1                 = 100;
const double lever_2                 = 83;
const double lever_3                 = 170;
const double step_length             = 10;
const double rotate_step             = 15;
const double acceleration            = 0.25;
const int max_angle                  = 135;
const int number_of_servos           = 6;
const int servo_delay                = 125;
const int bluetooth_delay            = 10;
const int servo_pins[]               = {4, 2, 1, 3, 6, 5};
const int servo_speed[]              = {15, 20, 30, 40, 40, 30};
const int start_angles[]             = {120, 90, 90, 90, 0, 0};

// create array of connected to driver servos
ServoDriverSmooth servo[number_of_servos];

// create array of threads to regulate servos
Thread ServoThread[]                 = {Thread(),
                                        Thread(),
                                        Thread(),
                                        Thread(),
                                        Thread(),
                                        Thread()
                                       };

Thread BluetoothThread               = Thread();

struct angles
{
  double alpha;
  double beta;
  double delta;
  double gamma;
  double epsilon;
  double zeta;
};

struct axis
{
  double x;
  double y;
  double z;
};

angles current_angles           = {120, 90, 90, 90, 0, 0};
axis current_axis               = {165, 175, 0};
angles warning_axis             = { -1, -1, -1, -1, -1, -1};

// Translate angle from radians to angles
double RadToAng(double radian) {
  return radian * 180 / pi;
}

double AngToRad(double angle) {
  return angle * pi / 180;
}

angles calculateAnglesParallelUpperLever(axis& axis_to_calculate)
{
  angles angles_to_calculate = {0, 0, 0, 0, 0, 0};
  double lower_base = sqrt(pow(axis_to_calculate.x, 2) + pow(axis_to_calculate.y, 2) + pow(axis_to_calculate.z, 2));
  double height = sqrt(pow(lever_1, 2) - pow(((pow((lower_base - lever_2), 2) + pow(lever_1, 2) - pow(lever_3, 2)) / (2 * (lower_base - lever_2))), 2));
  double diagonal_1 = sqrt(pow(lower_base, 2) + pow(lever_3, 2) - (2 * lower_base * sqrt(pow(lever_3, 2) - pow(height, 2))));
  double diagonal_2 = sqrt(pow(lower_base, 2) + pow(lever_1, 2) - (2 * lower_base * sqrt(pow(lever_1, 2) - pow(height, 2))));
  double alpha = RadToAng(acos((pow(lever_1, 2) + pow(lower_base, 2) - pow(diagonal_2, 2)) / (2 * lever_1 * lower_base)));
  double upper_part_of_alpha = RadToAng(atan2(axis_to_calculate.z, sqrt(pow(axis_to_calculate.x, 2) + pow(axis_to_calculate.y, 2))));
  double beta = RadToAng(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(diagonal_1, 2)) / (2 * lever_1 * lever_2)));
  double delta = RadToAng(acos((pow(lever_2, 2) + pow(lever_3, 2) - pow(diagonal_2, 2)) / (2 * lever_2 * lever_3)));
  angles_to_calculate.alpha = alpha + upper_part_of_alpha;
  angles_to_calculate.beta = 180 - beta;
  angles_to_calculate.delta = delta - 45;
  return angles_to_calculate;
}

angles calculateAnglesParallelFirstLever(axis& axis_to_calculate)
{
  angles angles_to_calculate = {0, 0, 0, 0, 0, 0};
  double lower_base = sqrt(pow(axis_to_calculate.x, 2) + pow(axis_to_calculate.y, 2) + pow(axis_to_calculate.z, 2));
  double height = sqrt(pow(lower_base, 2) - pow(((pow((lever_3 - lever_1), 2) + pow(lower_base, 2) - pow(lever_2, 2)) / (2 * (lever_3 - lever_1))), 2));
  double diagonal_1 = sqrt(pow(lever_3, 2) + pow(lever_2, 2) - (2 * lever_3 * sqrt(pow(lever_2, 2) - pow(height, 2))));
  double diagonal_2 = sqrt(pow(lever_3, 2) + pow(lower_base, 2) - (2 * lever_3 * sqrt(pow(lower_base, 2) - pow(height, 2))));
  double alpha = RadToAng(acos((pow(lower_base, 2) + pow(lever_1, 2) - pow(diagonal_1, 2)) / (2 * lower_base * lever_1)));
  double upper_part_of_alpha = RadToAng(atan2(axis_to_calculate.z, sqrt(pow(axis_to_calculate.x, 2) + pow(axis_to_calculate.y, 2))));
  double beta = RadToAng(acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(diagonal_2, 2)) / (2 * lever_1 * lever_2)));
  double delta = RadToAng(acos((pow(lever_2, 2) + pow(lever_3, 2) - pow(diagonal_1, 2)) / (2 * lever_2 * lever_3)));
  angles_to_calculate.alpha = alpha + upper_part_of_alpha;
  angles_to_calculate.beta = 180 - beta;
  angles_to_calculate.delta = delta - 45;
  return angles_to_calculate;
}

// calculating angles for servos to being parallel second lever to main diagonal
angles calculateAngles(axis& calculating_axis)
{
  angles calculated_angles;
  calculated_angles = calculateAnglesParallelUpperLever(calculating_axis);
  if ((isnan(calculated_angles.beta)) || (isnan(calculated_angles.delta)) || (isnan(calculated_angles.alpha)) || (calculated_angles.beta < (180 - max_angle)) || (calculated_angles.delta < (180 - max_angle))) {
    calculated_angles = calculateAnglesParallelFirstLever(calculating_axis);
    if ((isnan(calculated_angles.beta)) || (isnan(calculated_angles.delta)) || (isnan(calculated_angles.alpha)) || (calculated_angles.beta < (180 - max_angle)) || (calculated_angles.delta < (180 - max_angle))) {
      angles calculated_angles = warning_axis;
    } else {
      current_angles.alpha = calculated_angles.alpha;
      current_angles.beta = calculated_angles.alpha;
      current_angles.delta = calculated_angles.alpha;
    }
  } else {
    current_angles.alpha = calculated_angles.alpha;
    current_angles.beta = calculated_angles.alpha;
    current_angles.delta = calculated_angles.alpha;
  }
  double lower_base_rotation_angle = RadToAng(atan2(calculating_axis.y, calculating_axis.x));
  calculated_angles.zeta = lower_base_rotation_angle;
  return calculated_angles;
}

void serialRead()
{
  if (Serial.available()) {
    char parcel = Serial.read();
    bool must = false;
    axis possible_axis = current_axis;
    angles possible_angles;
    switch (parcel) {
      case '1':
        possible_axis.x += step_length;
        must = true;
        break;
      case '2':
        possible_axis.x -= step_length;
        must = true;
        break;
      case '3':
        possible_axis.y += step_length;
        must = true;
        break;
      case '4':
        possible_axis.y -= step_length;
        must = true;
        break;
      case '5':
        possible_axis.z += step_length;
        must = true;
        break;
      case '6':
        possible_axis.z -= step_length;
        must = true;
        break;
      case '7':
        if (current_angles.epsilon + rotate_step <= max_angle) {
          current_angles.epsilon += rotate_step;
        }
        break;
      case '8':
        if (current_angles.epsilon >= rotate_step) {
          current_angles.epsilon -= rotate_step;
        }
        break;
      case '9':
        if (current_angles.gamma + rotate_step <= max_angle) {
          current_angles.gamma += rotate_step;
        }
        break;
      case 'a':
        if (current_angles.gamma >= rotate_step) {
          current_angles.gamma -= rotate_step;
        }
        break;
    }
    if (must) {
      possible_angles = calculateAngles(possible_axis);
      if (possible_angles.alpha != warning_axis.alpha) {
        current_axis = possible_axis;
        current_angles = possible_angles;
      }
    }
  }
}

void servoMoving1()
{
  if (current_angles.alpha <= max_angle) {
    servo[0].setTargetDeg(current_angles.alpha);
    servo[0].tick();
  }
}

void servoMoving2()
{
  if (current_angles.beta <= max_angle) {
    servo[1].setTargetDeg(current_angles.beta);
    servo[1].tick();
  }
}

void servoMoving3()
{
  if (current_angles.delta <= max_angle) {
    servo[2].setTargetDeg(current_angles.delta);
    servo[2].tick();
  }
}

void servoMoving4()
{
  if (current_angles.gamma <= max_angle) {
    servo[3].setTargetDeg(current_angles.gamma);
    servo[3].tick();
  }
}

void servoMoving5()
{
  if (current_angles.epsilon <= max_angle) {
    servo[4].setTargetDeg(current_angles.epsilon);
    servo[4].tick();
  }
}

void servoMoving6()
{
  if (current_angles.zeta <= max_angle) {
    servo[5].setTargetDeg(current_angles.zeta);
    servo[5].tick();
  }
}

void setup()
{
  Serial.begin(9600);
  for (int counter_pins = 0; counter_pins < number_of_servos; counter_pins++) {
    servo[counter_pins].attach(servo_pins[counter_pins], start_angles[counter_pins]);
    servo[counter_pins].smoothStart();
    servo[counter_pins].setAutoDetach(false);
    // max speed
    servo[counter_pins].setSpeed(servo_speed[counter_pins]);
    // acceleration
    servo[counter_pins].setAccel(acceleration);
    // activating servo threads
    switch (counter_pins) {
      case 0:
        ServoThread[counter_pins].onRun(servoMoving1);
        break;
      case 1:
        ServoThread[counter_pins].onRun(servoMoving2);
        break;
      case 2:
        ServoThread[counter_pins].onRun(servoMoving3);
        break;
      case 3:
        ServoThread[counter_pins].onRun(servoMoving4);
        break;
      case 4:
        ServoThread[counter_pins].onRun(servoMoving5);
        break;
      case 5:
        ServoThread[counter_pins].onRun(servoMoving6);
        break;
    }
    ServoThread[counter_pins].setInterval(servo_delay);
  }
  // activating bluetooth and calculating threads
  BluetoothThread.onRun(serialRead);
  BluetoothThread.setInterval(bluetooth_delay);
  Serial.write('1');
}

void loop()
{
  // servo threads
  for (int counter_pins = 0; counter_pins < number_of_servos; counter_pins++) {
    if (ServoThread[counter_pins].shouldRun()) {
      ServoThread[counter_pins].run();
    }
  }
  // thread to read data on bluetooth module
  if (BluetoothThread.shouldRun()) {
    BluetoothThread.run();
  }
}
