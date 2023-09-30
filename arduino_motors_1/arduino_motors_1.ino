#include <iarduino_HC_SR04.h> 
#include <Servo.h>

Servo servo;
const int delay_for_increase     = 50;
const int ride_delay             = 1000;
const int minimal_distanses_side = 20;
const int minimal_distanses      = 28;
const int IN1                    = 12;
const int IN2                    = 11;
const int EN1                    = 13;
const int EN2                    = 9;
const int IN3                    = 10;
const int IN4                    = 8;

struct distance_data {
   int a;
   int b;
   int c;
};

iarduino_HC_SR04 sensor(16, 17);          // Объявляем объект sensor, указывая номера arduino подключенные к выводам TRIG и ECHO датчика

distance_data distance () {
  distance_data data = { 0, 0, 0};
  servo.write(5);
  data.a = distone = round(sensor.distance());
  servo.write(90);
  data.b = distone = round(sensor.distance());
  servo.write(175);
  data.c = distone = round(sensor.distance());
  return data
}

void motor_control_10(const int port_1, const int port_2, const int enable) {
  digitalWrite (port_1, HIGH);
  digitalWrite (port_2, HIGH);
  for (int i = 50; i <= 180; ++i)
  {
    analogWrite(enable, i);
    delay(delay_for_increase);
  }
  analogWrite (enable_1, 0);
  digitalWrite (port_1, LOW);
  digitalWrite (port_2, LOW);
  delay(8000);
}

void motor_control_1(const int port) {
  digitalWrite (port, HIGH);
  delay(ride_delay);
  digitalWrite (port, LOW);
}

void motor_control(const int port_1, const int port_2) {
  digitalWrite (port_1, HIGH);
  digitalWrite (port_2, HIGH);
  delay(ride_delay);
  digitalWrite (port_1, LOW);
  digitalWrite (port_2, LOW);
}

void ride(const distance_data data) {
  if(data.b > minimal_distanses) {
    motor_control(IN1, IN3); 
  } else {
    if((data.a > minimal_distanses_side) and (data.c > minimal_distanses_side)) {
      motor_conrtol(IN2, IN4);
      motor_control(IN1, IN4);
    } else {
      if(data.a > minimal_distanses_side) {
        motor_control(IN1, IN4);
      } else if (data.b > minimal_distanses_side) {
        motor_control(IN2, IN3);
      }
    }
  }
}

void setup()
{
  servo.attach(15);
  pinMode (EN1, OUTPUT);
  pinMode (EN2, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
}
void loop()
{
  distance_data data = distance();
  ride(data);
}
