#include <Ultrasonic.h>
#include <Servo.h>

Servo servo;

const int angle                    = 10;   // угол при криво закреплённом датчике расстояния
const int servo_delay              = 150;  // задержка положения сервопривода
const int ride_delay_min           = 250;  // задержка проверки поезки минимум
const int ride_delay_mid           = 500;  // задержка проверки поезки средняя
const int ride_delay_max           = 750;  // задержка проверки поезки максимум
const float minimal_distanses_side = 15;   // расстояние по бокам от которого начинает работать цикл объезда
const float minimal_distanses      = 20;   // расстояние от которого начинает работать цикл объезда
const int IN1                      = 11;   // 1ый порт 1ого двигателя
const int IN2                      = 12;   // 2ой порт 1ого двигателя
const int EN1                      = 13;   // 1ый порт регулирования скважности питания
const int EN2                      = 9;    // 2ой порт регулирования скважности питания
const int IN3                      = 10;   // 1ый порт 2ого двигателя
const int IN4                      = 8;    // 2ой порт 2ого двигателя

struct distance_data {                   // расстояния от боков и до препятствия напрямую
  float right;
  float forward;
  float left;
};

Ultrasonic ultrasonic(17, 16);        // Объявляем объект sensor, указывая номера arduino подключенные к выводам TRIG и ECHO датчика

distance_data distance () {              // заполнение структуры расстояний (значения не точные, криво закрепил
  distance_data data;
  delay(servo_delay);
  servo.write(0);
  delay(servo_delay);
  data.right = ultrasonic.distanceRead(CM);
  delay(servo_delay);
  servo.write(90 - angle);
  delay(servo_delay);
  data.forward = ultrasonic.distanceRead(CM);
  delay(servo_delay);
  servo.write(180 - angle);
  delay(servo_delay);
  data.left = ultrasonic.distanceRead(CM);
  delay(servo_delay * 3 / 2);
  return data;
}

void motor_control(const int port_1, const int port_2, const int ride_delay) {
  digitalWrite (EN1, HIGH);
  digitalWrite (EN2, HIGH);
  digitalWrite (port_1, HIGH);
  digitalWrite (port_2, HIGH);
  if (ride_delay != 0) {
    delay(ride_delay);
    digitalWrite (EN1, LOW);
    digitalWrite (EN2, LOW);
    digitalWrite (port_1, LOW);
    digitalWrite (port_2, LOW);
  }
}

void ride(distance_data &data) {              // контроль движения (дополнительная подстройка)
  servo.write(90);
  while (data.forward > minimal_distanses) {
    motor_control(IN1, IN3, 0);       // движение вперёд
    data.forward = ultrasonic.distanceRead(CM);
  }
  digitalWrite (EN1, LOW);
  digitalWrite (EN2, LOW);
  digitalWrite (IN1, LOW);
  digitalWrite (IN3, LOW);
  if (data.forward <= minimal_distanses) {
    if ((data.right <= minimal_distanses_side) && (data.left <= minimal_distanses_side)) {
      motor_control(IN2, IN4, ride_delay_max);                     // движение назад
      motor_control(IN1, IN4, ride_delay_min);                     // движение назад-направо
    } else {
      if (data.right > minimal_distanses_side) {
        motor_control(IN1, IN4, ride_delay_min);                   // движение назад-направо
      } else if (data.left > minimal_distanses_side) {
        motor_control(IN2, IN3, ride_delay_min);                   // движение назад-налево
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
