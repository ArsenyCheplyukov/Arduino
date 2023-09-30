
/*
      ###          ###          ###                  |                                         | 6)    @
     #   # ################### #   #                 |                                         |        \
    #     ## 1 #   ###   # 6 ##     #                |           DELTA                         |  + (X)  \
   #       #####         #####       #               |              |                          |          \ ____________@  (Y) +
  ###      #                 #      ###              |             _|_______                   |           0
      ###  #                 #  ###                  |            ( @        }                 |           |
     #   # #####         ##### #   #                 |           /  O     O@-------- BETA      |           | (Z) +
    #     ## 2 #         # 5 ##     #                |          /    ____     (__________#     |           |
   #       #####         #####       #               |         |    /    \               #     |           @
  ###      #                 #      ###              |        /    |      \    O     ____#     |
      ###  #                 #  ###                  |       |    /        \___@____)    #     |
     #   # #####         ##### #   #                 |      /    |             |               |
   #      ## 3 #         # 4 ##     #                |     |    /              |               |
   #       ###################       #               |     (   )             ALPHA             |
  ###                               ###              |      000                                |
             ROBOT(HEXAPOD)                                   LEG(ANGLES MEANING)               COORDINATE AXIS(PASSES INTO FUNCTION)
*/

#include <ArduinoSTL.h>
#include <SD.h>             // необходимо включить SD библиотеку
#include <TMRpcm.h>         // также необходимо включить данную библиотеку...
#include <SPI.h>
#include <Vector.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Ultrasonic.h>
#include <Thread.h>
#include <ThreadController.h>

Ultrasonic ultrasonic(3, 2);        // Объявляем объект sensor, указывая номера arduino подключенные к выводам TRIG и ECHO датчика

TMRpcm music;   // создать объект для использования в данном скетче

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm_2 = Adafruit_PWMServoDriver(0x41);

ThreadController controll = ThreadController();
Thread readingThread = Thread();
Thread otherThread = Thread();

volatile char parcel;
const int SERVOMIN                   = 150; // This is the 'minimum' pulse length count (out of 4096)
const int SERVOMAX                   = 600; // This is the 'maximum' pulse length count (out of 4096)
const int USMIN                      = 600; // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
const int USMAX                      = 2400; // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
const int SERVO_FREQ                 = 50; // Analog servos run at ~50 Hz updates
const int common_points              = 6;
const int max_common_points          = 12;
const int steps_in_loop_num          = 4;
const int big_steps_in_loop_num      = 9;

const int legs_left [3][3]           = {{6, 9, 10}, {12, 11, 7}, {15, 14, 13}};
const int legs_right[3][3]           = {{14, 13, 11}, {12, 10, 15}, {9, 8, 7}};

const double speed_delay             = 2.5;
const double mid_speed               = 25;
const double max_speed               = 125;
const double delay_between_functions = 250;
const double lever_1                 = 0.053;
const double lever_2                 = 0.087;
const double lever                   = 0.045;
const double maximum_angle           = 157.5;
const double minimum_angle           = 37.5;
const double high_of_arc             = 0.015;
const double body_step               = 0.01;

const int minimal_distance          = 50;   // расстояние от которого начинает работать цикл объезда
const int servo_delay               = 250;  // задержка положения сервопривода
const int servo_port                = 8;
const int angle                     = 50;

// Измените это в соответствии с вашей платой расширения или модулем;
const int chipSelect            = 53;
const int letters_length        = 500;
const int delay_between_words   = 2000;
const int compile_delay         = 3000;

struct angles {
  double alpha;
  double beta;
  double delta;
};

struct axis {
  double x;
  double y;
  double z;
};

struct distance_data {                   // расстояния от боков и до препятствия напрямую
  int right;
  int front;
  int left;
};

const axis zero_axis     = {0, 0.06, 0.03};
const axis neutral_axis  = {0, 0.05, 0.05};
const axis forward_axis  = {0.015, 0.05, 0.05};
const axis backward_axis = { -0.015, 0.05, 0.05};
const axis max_axis      = {0, 0.07, 0.05};
const axis min_axis      = {0, 0.03, 0.05};

int pulseWidth(int angle) {
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, USMIN, USMAX);
  analog_value = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);
  return analog_value;
}

distance_data all_distance () {              // заполнение структуры расстояний
  distance_data data;
  delay(servo_delay);
  pwm.setPWM(servo_port, 0, pulseWidth(90 - angle));
  delay(servo_delay);
  data.right = ultrasonic.distanceRead(CM);
  Serial.print(data.left);
  delay(servo_delay);
  pwm.setPWM(servo_port, 0, pulseWidth(90));
  delay(servo_delay);
  data.front = ultrasonic.distanceRead(CM);
  Serial.print(data.left);
  delay(servo_delay);
  pwm.setPWM(servo_port, 0, pulseWidth(90 + angle));
  delay(servo_delay);
  data.left = ultrasonic.distanceRead(CM);
  Serial.print(data.left);
  delay(servo_delay);
  pwm.setPWM(servo_port, 0, pulseWidth(90));
  delay(servo_delay);
  return data;
}

void speaking(const std::vector<char> & v) {
  for (int i = 0; i < v.size(); i++) {
    switch (v.at(i)) {
        Serial.println(i);
      case 'a':
        music.play("a.wav");
        break;
      case 'A':
        music.play("a.wav");
        break;
      case 'b':
        music.play("b.wav");
        break;
      case 'B':
        music.play("b.wav");
        break;
      case 'c':
        music.play("c.wav");
        break;
      case 'C':
        music.play("c.wav");
        break;
      case 'd':
        music.play("d.wav");
        break;
      case 'D':
        music.play("d.wav");
        break;
      case 'e':
        music.play("e.wav");
        break;
      case 'E':
        music.play("e.wav");
        break;
      case 'f':
        music.play("f.wav");
        break;
      case 'F':
        music.play("f.wav");
        break;
      case 'g':
        music.play("g.wav");
        break;
      case 'G':
        music.play("g.wav");
        break;
      case 'h':
        music.play("h.wav");
        break;
      case 'H':
        music.play("h.wav");
        break;
      case 'i':
        music.play("i.wav");
        break;
      case 'I':
        music.play("i.wav");
        break;
      case 'j':
        music.play("j.wav");
        break;
      case 'J':
        music.play("j.wav");
        break;
      case 'k':
        music.play("k.wav");
        break;
      case 'K':
        music.play("k.wav");
        break;
      case 'l':
        music.play("l.wav");
        break;
      case 'L':
        music.play("l.wav");
        break;
      case 'm':
        music.play("m.wav");
        break;
      case 'M':
        music.play("m.wav");
        break;
      case 'n':
        music.play("n.wav");
        break;
      case 'N':
        music.play("n.wav");
        break;
      case 'o':
        music.play("o.wav");
        break;
      case 'O':
        music.play("o.wav");
        break;
      case 'P':
        music.play("p.wav");
        break;
      case 'p':
        music.play("p.wav");
        break;
      case 'q':
        music.play("q.wav");
        break;
      case 'Q':
        music.play("q.wav");
        break;
      case 'r':
        music.play("r.wav");
        break;
      case 'R':
        music.play("r.wav");
        break;
      case 's':
        music.play("s.wav");
        break;
      case 'S':
        music.play("s.wav");
        break;
      case 't':
        music.play("t.wav");
        break;
      case 'T':
        music.play("t.wav");
        break;
      case 'u':
        music.play("u.wav");
        break;
      case 'U':
        music.play("u.wav");
        break;
      case 'v':
        music.play("v.wav");
        break;
      case 'V':
        music.play("v.wav");
        break;
      case 'w':
        music.play("w.wav");
        break;
      case 'W':
        music.play("w.wav");
        break;
      case 'x':
        music.play("x.wav");
        break;
      case 'X':
        music.play("x.wav");
        break;
      case 'y':
        music.play("y.wav");
        break;
      case 'Y':
        music.play("y.wav");
        break;
      case 'z':
        music.play("z.wav");
        break;
      case 'Z':
        music.play("z.wav");
        break;
      case '_':
        delay(delay_between_words);
        break;
      default:
        break;
    }
    delay(letters_length);
  }
}

angles find_angles(const double x, const double y, const double z, bool state) {
  if (state == true) {
    angles ang;
    ang.alpha = 90 + (atan2(-x, y) * 180) / 3.14;
    double d = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    ang.beta = ((atan2(sqrt(pow(x, 2) + pow(y, 2)), z) * 180) / 3.14) + (((acos((pow(lever_1, 2) + pow(d, 2) - pow(lever_2, 2)) / (2 * lever_1 * d))) * 180) / 3.14);
    ang.delta = 180 - (((acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(d, 2)) / (2 * lever_1 * lever_2))) * 180) / 3.14);
    return ang;
  } else {
    angles ang;
    ang.alpha = 90 + (atan2(x, y) * 180) / 3.14;
    double d = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    ang.beta = 180 - ((atan2(sqrt(pow(x, 2) + pow(y, 2)), z) * 180) / 3.14) - (((acos((pow(lever_1, 2) + pow(d, 2) - pow(lever_2, 2)) / (2 * lever_1 * d))) * 180) / 3.14);
    ang.delta = (((acos((pow(lever_1, 2) + pow(lever_2, 2) - pow(d, 2)) / (2 * lever_1 * lever_2))) * 180) / 3.14);
    return ang;
  }
}

void move_leg(const int servo[3], const angles ang, bool state) {  // Если левая - то true, а если правая - то false
  if (state == true) {
    pwm.setPWM(servo[2], 0, pulseWidth(ang.delta));
    delay(speed_delay);
    pwm.setPWM(servo[1], 0, pulseWidth(ang.beta));
    delay(speed_delay);
    pwm.setPWM(servo[0], 0, pulseWidth(ang.alpha));
    delay(speed_delay);
  } else {
    pwm_2.setPWM(servo[2], 0, pulseWidth(ang.delta));
    delay(speed_delay);
    pwm_2.setPWM(servo[1], 0, pulseWidth(ang.beta));
    delay(speed_delay);
    pwm_2.setPWM(servo[0], 0, pulseWidth(ang.alpha));
    delay(speed_delay);
  }
}

void three_legs_full_control_lineral(const int servo_1[3], const axis strt_axis_1, const axis end_axis_1, const bool left_1,
                                     const int servo_2[3], const axis strt_axis_2, const axis end_axis_2, const bool left_2,
                                     const int servo_3[3], const axis strt_axis_3, const axis end_axis_3, const bool left_3) {
  double step_x_1 = (end_axis_1.x - strt_axis_1.x) / common_points;
  double step_y_1 = (end_axis_1.y - strt_axis_1.y) / common_points;
  double step_x_2 = (end_axis_2.x - strt_axis_2.x) / common_points;
  double step_y_2 = (end_axis_2.y - strt_axis_2.y) / common_points;
  double step_x_3 = (end_axis_3.x - strt_axis_3.x) / common_points;
  double step_y_3 = (end_axis_3.y - strt_axis_3.y) / common_points;
  for (int a = 0; a < common_points; a++) {
    double x_1 = step_x_1 * a + strt_axis_1.x;
    double y_1 = step_y_1 * a + strt_axis_1.y;
    double x_2 = step_x_2 * a + strt_axis_2.x;
    double y_2 = step_y_2 * a + strt_axis_2.y;
    double x_3 = step_x_3 * a + strt_axis_3.x;
    double y_3 = step_y_3 * a + strt_axis_3.y;
    angles ang_1 = find_angles(x_1, y_1, end_axis_1.z, left_1);
    angles ang_2 = find_angles(x_2, y_2, end_axis_2.z, left_2);
    angles ang_3 = find_angles(x_3, y_3, end_axis_3.z, left_3);
    move_leg(servo_1, ang_1, left_1);
    move_leg(servo_2, ang_2, left_2);
    move_leg(servo_3, ang_3, left_3);
  }
  delay(speed_delay);
}

void three_legs_full_control_round(const int servo_1[3], const axis strt_axis_1, const axis end_axis_1, const bool left_1,
                                   const int servo_2[3], const axis strt_axis_2, const axis end_axis_2, const bool left_2,
                                   const int servo_3[3], const axis strt_axis_3, const axis end_axis_3, const bool left_3) {
  double delta_high_x_1 = end_axis_1.x - strt_axis_1.x;
  double delta_high_y_1 = end_axis_1.y - strt_axis_1.y;
  double delta_high_x_2 = end_axis_2.x - strt_axis_2.x;
  double delta_high_y_2 = end_axis_2.y - strt_axis_2.y;
  double delta_high_x_3 = end_axis_3.x - strt_axis_3.x;
  double delta_high_y_3 = end_axis_3.y - strt_axis_3.y;
  for (int a = 0; a < common_points; a++) {
    double x_1 = delta_high_x_1 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_1.x;
    double y_1 = delta_high_y_1 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_1.y;
    double x_2 = delta_high_x_2 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_2.x;
    double y_2 = delta_high_y_2 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_2.y;
    double x_3 = delta_high_x_3 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_3.x;
    double y_3 = delta_high_y_3 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_3.y;
    double z = end_axis_3.z - (high_of_arc * cos(((a * (180 / common_points)) - 90) * 3.14 / 180));
    angles ang_1 = find_angles(x_1, y_1, z, left_1);
    angles ang_2 = find_angles(x_2, y_2, z, left_2);
    angles ang_3 = find_angles(x_3, y_3, z, left_3);
    move_leg(servo_1, ang_1, left_1);
    move_leg(servo_2, ang_2, left_2);
    move_leg(servo_3, ang_3, left_3);
  }
  delay(speed_delay);
}

void six_legs_full_control(const int servo_1[3], const axis strt_axis_1, const axis end_axis_1, const bool left_1,
                           const int servo_2[3], const axis strt_axis_2, const axis end_axis_2, const bool left_2,
                           const int servo_3[3], const axis strt_axis_3, const axis end_axis_3, const bool left_3,
                           const int servo_4[3], const axis strt_axis_4, const axis end_axis_4, const bool left_4,
                           const int servo_5[3], const axis strt_axis_5, const axis end_axis_5, const bool left_5,
                           const int servo_6[3], const axis strt_axis_6, const axis end_axis_6, const bool left_6) {
  double delta_high_x_1 = end_axis_1.x - strt_axis_1.x;
  double delta_high_y_1 = end_axis_1.y - strt_axis_1.y;
  double delta_high_x_2 = end_axis_2.x - strt_axis_2.x;
  double delta_high_y_2 = end_axis_2.y - strt_axis_2.y;
  double delta_high_x_3 = end_axis_3.x - strt_axis_3.x;
  double delta_high_y_3 = end_axis_3.y - strt_axis_3.y;
  double step_x_1 = (end_axis_4.x - strt_axis_4.x) / common_points;
  double step_y_1 = (end_axis_4.y - strt_axis_4.y) / common_points;
  double step_x_2 = (end_axis_5.x - strt_axis_5.x) / common_points;
  double step_y_2 = (end_axis_5.y - strt_axis_5.y) / common_points;
  double step_x_3 = (end_axis_6.x - strt_axis_6.x) / common_points;
  double step_y_3 = (end_axis_6.y - strt_axis_6.y) / common_points;
  for (int a = 0; a <= common_points; a++) {
    double x_1 = delta_high_x_1 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_1.x;
    double y_1 = delta_high_y_1 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_1.y;
    double x_2 = delta_high_x_2 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_2.x;
    double y_2 = delta_high_y_2 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_2.y;
    double x_3 = delta_high_x_3 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_3.x;
    double y_3 = delta_high_y_3 * sin((a * (90 / common_points)) * 3.14 / 180) + strt_axis_3.y;
    double z = end_axis_6.z - (high_of_arc * cos(((a * (180 / common_points)) - 90) * 3.14 / 180));
    double x_4 = step_x_1 * a + strt_axis_4.x;
    double y_4 = step_y_1 * a + strt_axis_4.y;
    double x_5 = step_x_2 * a + strt_axis_5.x;
    double y_5 = step_y_2 * a + strt_axis_5.y;
    double x_6 = step_x_3 * a + strt_axis_6.x;
    double y_6 = step_y_3 * a + strt_axis_6.y;
    angles ang_1 = find_angles(x_1, y_1, z, left_1);
    angles ang_2 = find_angles(x_2, y_2, z, left_2);
    angles ang_3 = find_angles(x_3, y_3, z, left_3);
    angles ang_4 = find_angles(x_4, y_4, end_axis_4.z, left_4);
    angles ang_5 = find_angles(x_5, y_5, end_axis_5.z, left_5);
    angles ang_6 = find_angles(x_6, y_6, end_axis_6.z, left_6);
    move_leg(servo_1, ang_1, left_1);
    move_leg(servo_2, ang_2, left_2);
    move_leg(servo_3, ang_3, left_3);
    move_leg(servo_4, ang_4, left_4);
    move_leg(servo_5, ang_5, left_5);
    move_leg(servo_6, ang_6, left_6);
  }
  delay(speed_delay);
}

void legs_hi() {
  angles ang_2 = {0, maximum_angle, minimum_angle};
  angles ang_1 = {30, maximum_angle, minimum_angle};
  angles start_angle = find_angles(neutral_axis.x, neutral_axis.y, neutral_axis.z, 1);
  for (int a = 0; a < steps_in_loop_num; a++) {
    move_leg(legs_left[0], ang_1, 1);
    delay(max_speed);
    move_leg(legs_left[0], ang_2, 1);
    delay(max_speed);
  }
  double step_alpha = (start_angle.alpha - ang_2.alpha) / max_common_points;
  double step_beta = (start_angle.beta - ang_2.beta) / max_common_points;
  double step_delta = (start_angle.delta - ang_2.delta) / max_common_points;
  for (int a = 0; a <= max_common_points; a++) {
    delay(speed_delay);
    angles common_angle;
    common_angle.alpha = a * step_alpha + ang_2.alpha;
    common_angle.beta = a * step_beta + ang_2.beta;
    common_angle.delta = a * step_delta + ang_2.delta;
    delay(speed_delay);
    move_leg(legs_left[0], common_angle, 1);
    delay(speed_delay);
  }
}

void vermaht() {
  angles ang_1 = {30, maximum_angle, minimum_angle};
  angles start_angle = find_angles(neutral_axis.x, neutral_axis.y, neutral_axis.z, 1);
  move_leg(legs_left[0], ang_1, 1);
  delay(speed_delay);
  music.play("vermaht.wav");
  int current_millis = millis();
  while ((millis() - current_millis) < 21000 ) {
    if (parcel == '%') {
      music.stopPlayback();
      goto NEXT;
    }
  }
NEXT:
  double step_alpha = (start_angle.alpha - ang_1.alpha) / max_common_points;
  double step_beta = (start_angle.beta - ang_1.beta) / max_common_points;
  double step_delta = (start_angle.delta - ang_1.delta) / max_common_points;
  for (int a = 0; a <= max_common_points; a++) {
    delay(speed_delay);
    angles common_angle;
    common_angle.alpha = a * step_alpha + ang_1.alpha;
    common_angle.beta = a * step_beta + ang_1.beta;
    common_angle.delta = a * step_delta + ang_1.delta;
    delay(speed_delay);
    move_leg(legs_left[0], common_angle, 1);
    delay(speed_delay);
  }
}

void legs_up() {
  for (int a = 0; a < 3; a++) {
    pwm_2.setPWM(legs_right[a][0], 0, pulseWidth(90));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][0], 0, pulseWidth(90));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][1], 0, pulseWidth(minimum_angle));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][1], 0, pulseWidth(maximum_angle));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][2], 0, pulseWidth(maximum_angle));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][2], 0, pulseWidth(minimum_angle));
    delay(mid_speed);
  }
}

void set_zero_axis() {
  angles ang_right = find_angles(zero_axis.x, zero_axis.y, zero_axis.z, 0);
  angles ang_left = find_angles(zero_axis.x, zero_axis.y, zero_axis.z, 1);
  for (int a = 0; a < 3; a++) {
    pwm.setPWM(legs_left[a][0], 0, pulseWidth(ang_left.alpha));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][0], 0, pulseWidth(ang_right.alpha));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][2], 0, pulseWidth(ang_left.delta));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][2], 0, pulseWidth(ang_right.delta));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][1], 0, pulseWidth(ang_left.beta));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][1], 0, pulseWidth(ang_right.beta));
    delay(mid_speed);
  }
}

void legs_90_degree() {
  for (int a = 0; a < 3; a++) {
    pwm_2.setPWM(legs_right[a][0], 0, pulseWidth(90));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][0], 0, pulseWidth(90));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][1], 0, pulseWidth(90));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][1], 0, pulseWidth(90));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[a][2], 0, pulseWidth(90));
    delay(mid_speed);
    pwm.setPWM(legs_left[a][2], 0, pulseWidth(90));
    delay(mid_speed);
  }
}

void start_working() {
  legs_up();
  delay(delay_between_functions);
  set_zero_axis();
  delay(delay_between_functions);
  angles ang_right = find_angles(neutral_axis.x, neutral_axis.y, neutral_axis.z, 0);
  angles ang_left = find_angles(neutral_axis.x, neutral_axis.y, neutral_axis.z, 1);
  for (int b = 0; b < 3; b++) {
    pwm.setPWM(legs_left[b][0], 0, pulseWidth(ang_left.alpha));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[b][0], 0, pulseWidth(ang_right.alpha));
    delay(mid_speed);
    pwm.setPWM(legs_left[b][1], 0, pulseWidth(ang_left.beta));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[b][1], 0, pulseWidth(ang_right.beta));
    delay(mid_speed);
    pwm.setPWM(legs_left[b][2], 0, pulseWidth(ang_left.delta));
    delay(mid_speed);
    pwm_2.setPWM(legs_right[b][2], 0, pulseWidth(ang_right.delta));
    delay(mid_speed);
  }
}

void moving_forward(const int num_of_loop) {
  three_legs_full_control_round(legs_left[0], neutral_axis, forward_axis, 1,
                                legs_left[2], neutral_axis, forward_axis, 1,
                                legs_right[1], neutral_axis, forward_axis, 0);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_right[0], neutral_axis, forward_axis, 0,
                          legs_right[2], neutral_axis, forward_axis, 0,
                          legs_left[1], neutral_axis, forward_axis, 1,
                          legs_left[0], forward_axis, neutral_axis, 1,
                          legs_left[2], forward_axis, neutral_axis, 1,
                          legs_right[1], forward_axis, neutral_axis, 0);
    six_legs_full_control(legs_left[0], neutral_axis, forward_axis, 1,
                          legs_left[2], neutral_axis, forward_axis, 1,
                          legs_right[1], neutral_axis, forward_axis, 0,
                          legs_right[0], forward_axis, neutral_axis, 0,
                          legs_right[2], forward_axis, neutral_axis, 0,
                          legs_left[1], forward_axis, neutral_axis, 1);
  }
  three_legs_full_control_lineral(legs_left[0], forward_axis, neutral_axis, 1,
                                  legs_left[2], forward_axis, neutral_axis, 1,
                                  legs_right[1], forward_axis, neutral_axis, 0);
}

void moving_backward(const int num_of_loop) {
  three_legs_full_control_round(legs_right[0], neutral_axis, backward_axis, 0,
                                legs_right[2], neutral_axis, backward_axis, 0,
                                legs_left[1], neutral_axis, backward_axis, 1);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_left[0], neutral_axis, backward_axis, 1,
                          legs_left[2], neutral_axis, backward_axis, 1,
                          legs_right[1], neutral_axis, backward_axis, 0,
                          legs_right[0], backward_axis, neutral_axis, 0,
                          legs_right[2], backward_axis, neutral_axis, 0,
                          legs_left[1], backward_axis, neutral_axis, 1);
    six_legs_full_control(legs_right[0], neutral_axis, backward_axis, 0,
                          legs_right[2], neutral_axis, backward_axis, 0,
                          legs_left[1], neutral_axis, backward_axis, 1,
                          legs_left[0], backward_axis, neutral_axis, 1,
                          legs_left[2], backward_axis, neutral_axis, 1,
                          legs_right[1], backward_axis, neutral_axis, 0);
  }
  three_legs_full_control_lineral(legs_right[0], backward_axis, neutral_axis, 0,
                                  legs_right[2], backward_axis, neutral_axis, 0,
                                  legs_left[1], backward_axis, neutral_axis, 1);
}

void moving_left(const int num_of_loop) {
  three_legs_full_control_round(legs_left[0], neutral_axis, max_axis, 1,
                                legs_left[2], neutral_axis, max_axis, 1,
                                legs_right[1], neutral_axis, min_axis, 0);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_right[0], neutral_axis, min_axis, 0,
                          legs_right[2], neutral_axis, min_axis, 0,
                          legs_left[1], neutral_axis, max_axis, 1,
                          legs_left[0], max_axis, neutral_axis, 1,
                          legs_left[2], max_axis, neutral_axis, 1,
                          legs_right[1], min_axis, neutral_axis, 0);
    six_legs_full_control(legs_left[0], neutral_axis, max_axis, 1,
                          legs_left[2], neutral_axis, max_axis, 1,
                          legs_right[1], neutral_axis, min_axis, 0,
                          legs_right[0], min_axis, neutral_axis, 0,
                          legs_right[2], min_axis, neutral_axis, 0,
                          legs_left[1], max_axis, neutral_axis, 1);
  }
  three_legs_full_control_lineral(legs_left[0], max_axis, neutral_axis, 1,
                                  legs_left[2], max_axis, neutral_axis, 1,
                                  legs_right[1], min_axis, neutral_axis, 0);
}

void moving_right(const int num_of_loop) {
  three_legs_full_control_round(legs_right[0], neutral_axis, max_axis, 0,
                                legs_right[2], neutral_axis, max_axis, 0,
                                legs_left[1], neutral_axis, min_axis, 1);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_left[0], neutral_axis, min_axis, 1,
                          legs_left[2], neutral_axis, min_axis, 1,
                          legs_right[1], neutral_axis, max_axis, 0,
                          legs_right[0], max_axis, neutral_axis, 0,
                          legs_right[2], max_axis, neutral_axis , 0,
                          legs_left[1], min_axis, neutral_axis, 1);
    six_legs_full_control(legs_right[0], neutral_axis, max_axis, 0,
                          legs_right[2], neutral_axis, max_axis, 0,
                          legs_left[1], neutral_axis, min_axis, 1,
                          legs_left[0], min_axis, neutral_axis, 1,
                          legs_left[2], min_axis, neutral_axis, 1,
                          legs_right[1], max_axis, neutral_axis, 0);
  }
  three_legs_full_control_lineral(legs_right[0], max_axis, neutral_axis, 0,
                                  legs_right[2], max_axis, neutral_axis, 0,
                                  legs_left[1], min_axis, neutral_axis, 1);
}

void rotation_left(const int num_of_loop) {
  three_legs_full_control_round(legs_left[0], neutral_axis, backward_axis, 1,
                                legs_left[2], neutral_axis, backward_axis, 1,
                                legs_right[1], neutral_axis, forward_axis, 0);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_right[0], neutral_axis, forward_axis, 0,
                          legs_right[2], neutral_axis, forward_axis, 0,
                          legs_left[1], neutral_axis, backward_axis, 1,
                          legs_left[0], backward_axis, neutral_axis, 1,
                          legs_left[2], backward_axis, neutral_axis, 1,
                          legs_right[1], forward_axis, neutral_axis, 0);
    six_legs_full_control(legs_left[0], neutral_axis, backward_axis, 1,
                          legs_left[2], neutral_axis, backward_axis, 1,
                          legs_right[1], neutral_axis, forward_axis, 0,
                          legs_right[0], forward_axis, neutral_axis, 0,
                          legs_right[2], forward_axis, neutral_axis, 0,
                          legs_left[1], backward_axis, neutral_axis, 1);
  }
  three_legs_full_control_lineral(legs_left[0], backward_axis, neutral_axis, 1,
                                  legs_left[2], backward_axis, neutral_axis, 1,
                                  legs_right[1], forward_axis, neutral_axis, 0);
}

void rotation_right(const int num_of_loop) {
  three_legs_full_control_round(legs_left[0], neutral_axis, forward_axis, 1,
                                legs_left[2], neutral_axis, forward_axis, 1,
                                legs_right[1], neutral_axis, backward_axis, 0);
  for (int a = 0; a < num_of_loop; a++) {
    six_legs_full_control(legs_right[0], neutral_axis, backward_axis, 0,
                          legs_right[2], neutral_axis, backward_axis, 0,
                          legs_left[1], neutral_axis, forward_axis, 1,
                          legs_left[0], forward_axis, neutral_axis, 1,
                          legs_left[2], forward_axis, neutral_axis, 1,
                          legs_right[1], backward_axis, neutral_axis, 0);
    six_legs_full_control(legs_left[0], neutral_axis, forward_axis, 1,
                          legs_left[2], neutral_axis, forward_axis, 1,
                          legs_right[1], neutral_axis, backward_axis, 0,
                          legs_right[0], backward_axis, neutral_axis, 0,
                          legs_right[2], backward_axis, neutral_axis, 0,
                          legs_left[1], forward_axis, neutral_axis, 1);
  }
  three_legs_full_control_lineral(legs_left[0], forward_axis, neutral_axis, 1,
                                  legs_left[2], forward_axis, neutral_axis, 1,
                                  legs_right[1], backward_axis, neutral_axis, 0);
}

void body_moving(const axis start_axis, const axis end_axis) {
  double step_x = (end_axis.x - start_axis.x) / max_common_points;
  double step_y = (end_axis.y - start_axis.y) / max_common_points;
  for (int a = 0; a <= max_common_points; a++) {
    angles ang_right = find_angles((step_y * a + start_axis.y), (lever - step_x * a + start_axis.x), neutral_axis.z, 0);
    angles ang_left = find_angles((step_y * a + start_axis.y), (lever + step_x * a + start_axis.x), neutral_axis.z, 1);
    move_leg(legs_left[0], ang_left, 1);
    delay(speed_delay);
    move_leg(legs_left[1], ang_left, 1);
    delay(speed_delay);
    move_leg(legs_left[2], ang_left, 1);
    delay(speed_delay);
    move_leg(legs_right[2], ang_right, 0);
    delay(speed_delay);
    move_leg(legs_right[1], ang_right, 0);
    delay(speed_delay);
    move_leg(legs_right[0], ang_right, 0);
    delay(speed_delay);
  }
}

void ride() {              // объезд препятствий
  distance_data data = all_distance();
  if ((data.front >= data.right) && (data.front >= data.left)) {
    if (data.front >= minimal_distance) {
      moving_forward(steps_in_loop_num);
    } else {
      moving_backward(big_steps_in_loop_num);
      rotation_right(steps_in_loop_num);
    }
  } else if ((data.right > data.front) && (data.right > data.left)) {
    if (data.right >= minimal_distance) {
      rotation_right(steps_in_loop_num);
      moving_forward(steps_in_loop_num);
    } else {
      moving_backward(big_steps_in_loop_num);
      rotation_right(steps_in_loop_num);
    }
  } else if ((data.left > data.front) && (data.left > data.right)) {
    if (data.left >= minimal_distance) {
      rotation_left(steps_in_loop_num);
      moving_forward(steps_in_loop_num);
    } else {
      moving_backward(big_steps_in_loop_num);
      rotation_right(steps_in_loop_num);
    }
  }
}

void reading_break() {
  parcel = Serial3.read();

  Serial.print("Program get : ");
  Serial.print(parcel);
  Serial.println();

  if (parcel == '%') {
    Serial.println("Get stop point");
    music.stopPlayback();
    controll.remove(&otherThread);
  }
}

void spider_sound() {
  music.play("Spider.wav");
}

void setup() {
  Serial.begin(9600);

  Wire.begin();

  byte error, address;
  int nDevices;

  Serial.println("Getting i2c address");

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }

    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(500);           // wait 5 seconds for next scan

  pwm.begin();
  pwm_2.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  pwm_2.setOscillatorFrequency(27000000);
  pwm_2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Serial.println("Start connecting distnce sensor");

  pinMode(2, INPUT);
  pinMode(3, OUTPUT);

  // Open serial communications and wait for port to open:

  Serial.println("Connecting bluetooth module");

  Serial3.begin(9600);
  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  Serial.println("Connecting speaker");
  music.speakerPin = 11; // 11 на Mega, 9 на Uno, Nano и т.д.
  music.setVolume(4);    //   от 0 до 7. Установка уровня громкости
  music.volume(1);    //   от 0 до 7. Установка уровня громкости
  music.quality(0);        //  1 для 2x передескритезации, 0 for для нормального режима

  Serial.println("Starting periodicly reading parcel");

  readingThread.onRun(reading_break);
  readingThread.setInterval(1);  // Configure readingThread
  controll.add(&readingThread);
}

void loop() {
  controll.run();
  switch (parcel) {
    case '1':
      start_working();
      break;
    case '2':
      moving_forward(steps_in_loop_num);
      break;
    case '3':
      moving_backward(steps_in_loop_num);
      break;
    case '4':
      moving_right(steps_in_loop_num);
      break;
    case '5':
      moving_left(steps_in_loop_num);
      break;
    case '6':
      rotation_right(steps_in_loop_num);
      break;
    case '7':
      rotation_left(steps_in_loop_num);
      break;
    case '8':
      legs_90_degree();
      break;
    case '9':
      moving_forward(big_steps_in_loop_num);
      break;
    case 'a':
      moving_backward(big_steps_in_loop_num);
      break;
    case 'b':
      moving_right(big_steps_in_loop_num);
      break;
    case 'c':
      moving_left(big_steps_in_loop_num);
      break;
    case 'd':
      rotation_right(big_steps_in_loop_num);
      break;
    case 'e':
      rotation_left(big_steps_in_loop_num);
      break;
    case 'f':
      legs_hi();
      break;
    case 'k':
      music.play("Shine.wav");
      break;
    case 'l':
      vermaht();
      break;
    case 'n':
      music.play("Rawr.wav");
      break;
    case 'h':
      while (parcel != '%') {
        ride();
      }
      break;
    case 'm':
      otherThread.onRun(spider_sound);
      otherThread.setInterval(4000);  // Configure readingThread
      controll.add(&otherThread);
      break;
    case '@':
      Serial.print("Start speaking");
      std::vector<char> v;
      int current_millis = millis();
      while ((millis() - current_millis) < 7500) {
        delay(150);
        if (Serial3.available()) {
          v.push_back(Serial3.read());
        }
      }
      speaking(v);
      delay(1000);
      break;
    case 'g':
      axis zero_point = {0, 0, 0.05};
      axis point = {0, 0, 0.05};
      while (parcel != '%') {                     // И ВОТ ЭТОТ ЦИКЛ
        switch (parcel) {
          case '2':
            point.y = zero_point.y + body_step;
            break;
          case '3':
            point.y = zero_point.y - body_step;
            break;
          case '4':
            point.x = zero_point.x + body_step;
            break;
          case '5':
            point.x = zero_point.x - body_step;
            break;
        }
        body_moving(zero_point, point);
        delay(max_speed);
        zero_point = point;
      }
      break;
    default:
      break;
  }
  delay(delay_between_functions);
}
