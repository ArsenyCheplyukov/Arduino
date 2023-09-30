// Подключаем библиотеку управления сервомоторами.
#include <Servo.h>
// Подключаем библиотеку, управляющую дальномером.
#include <Ultrasonic.h>
// Инициализируем дальномер
// Контакт Trig соединен с 17 портом, Echo с 16 портом.
Ultrasonic sonar(17, 16);
// Создаем сервомотор поворота головы.
Servo neck;
// Объявляем переменные для хранения состояния двух моторов.
int motor_L1, motor_L2, input_L;
int motor_R1, motor_R2, input_R;
// Константы — постоянные значения для уточнения углов.
const int left_ang=168;
const int front_ang=102;
const int right_ang=20;
// Временные константы служат для точного задания времени на поворот, разворот, движение вперед. Робот, находящий выход из лабиринта 169
// в миллисекундах.
const int time_90=390;
const int time_180=750;
const int time_10cm=220;
const int time_7cm=120;

// Функция инициализации управления моторами.
void setup_motor_system(int L1, int L2, int iL, int R1, int R2, int iR)
{
 // Заносим в переменные номера контактов (пинов) Arduino.
 motor_L1=L1; motor_L2=L2; input_L=iL;
 // Для левых и правых моторов машинки.
 motor_R1=R1; motor_R2=R2; input_R=iR;
 // Переводим указанные порты в состояние вывода данных.
 pinMode(motor_L1, OUTPUT);
 pinMode(motor_L2, OUTPUT);
 pinMode(input_L, OUTPUT);
 pinMode(motor_R1, OUTPUT);
 pinMode(motor_R2, OUTPUT);
 pinMode(input_R, OUTPUT);
}
// Функция задает скорость двигателей.
void setspeed(int LeftSpeed, int RightSpeed)
{
 // Задаем ширину положительного фронта от 0 до 255.
 analogWrite(input_L, LeftSpeed);
 analogWrite(input_R, RightSpeed);
 // Чем больше, тем интенсивнее работает мотор.
}
// движение вперед.
void forward()
{
 // Если двигатель будет работать не в ту сторону,
 // поменять на нем контакты местами.
 digitalWrite(motor_L1, HIGH);
 digitalWrite(motor_L2,LOW);
 digitalWrite(motor_R1, HIGH);
 digitalWrite(motor_R2,LOW);
}
// Поворот налево с блокировкой левых колес.
void forward_left()
{
 // блокируем вращение левых колес.
 digitalWrite(motor_L1, LOW);
 digitalWrite(motor_L2, LOW);
 // правые колеса вращаются.
 digitalWrite(motor_R1, HIGH);
 digitalWrite(motor_R2, LOW);
}
// Поворот налево.
void left()
{
 // левые колеса вращаются назад
 digitalWrite(motor_L1, LOW);
 digitalWrite(motor_L2, HIGH);
 // правые колеса вращаются.
 digitalWrite(motor_R1, HIGH);
 digitalWrite(motor_R2, LOW);
}
// Поворот направо с блокировкой правых колес.
void forward_right()
{
 // левые колеса вращаются.
 digitalWrite(motor_L1, HIGH);
 digitalWrite(motor_L2, LOW);
 // блокируем вращение правых колес.
 digitalWrite(motor_R1, LOW);
 digitalWrite(motor_R2, LOW);
}
void right()
{
 // левые колеса вращаются.
 digitalWrite(motor_L1, HIGH);
 digitalWrite(motor_L2, LOW);
 // правые колеса вращаются назад.
 digitalWrite(motor_R1, LOW);
 digitalWrite(motor_R2, HIGH);
}
// Включаем движение назад.
void backward()
{
 // Меняем направление вращения двигателей.
 digitalWrite(motor_L2, HIGH);
 digitalWrite(motor_L1,LOW);
 digitalWrite(motor_R2, HIGH);
 digitalWrite(motor_R1,LOW);
}
void backward_left()
{
 // Меняем направление вращения двигателей.
 digitalWrite(motor_L2, LOW);
 digitalWrite(motor_L1,LOW);
 digitalWrite(motor_R2, HIGH);
 digitalWrite(motor_R1,LOW);
}
void backward_right()
{
 // Меняем направление вращения двигателей.
 digitalWrite(motor_L2, HIGH);
 digitalWrite(motor_L1,LOW);
 digitalWrite(motor_R2, LOW);
 digitalWrite(motor_R1,LOW);
}
void _stop()
{
 // Блокируем все колеса.
 digitalWrite(motor_L2, LOW);
 digitalWrite(motor_L1,LOW);
 digitalWrite(motor_R2, LOW);
 digitalWrite(motor_R1,LOW);
}

void setup()
{
 // Инициализируем сервомотор, управление 9 портом.
 neck.attach(15);
 // Заносим в переменные номера контактов (пинов) Arduino.
 // Для левых и правых моторов машинки.
 setup_motor_system(11, 12, 13, 10, 8, 9);
 // Двигатели остановлены.
 setspeed(0, 0);
 neck.write(front_ang);
 delay(150);
 setspeed(255, 255);
 _stop();
}
// Основная программа.
void loop()
{
 setspeed(255, 255);
 // Создаем переменные для хранения двух дистанций до препятствий спереди, справа.
 int Dist_left, Dist_front, Dist_right;
 _stop();
 // Ждем, так как поворот занимает небольшое время.
 delay(250);
 // Записываем расстояние до препятствия впереди.
 Dist_front = sonar.distanceRead(CM);
 // Поворачиваем голову направо.
 neck.write(right_ang);
 // Ждем, так как поворот занимает небольшое время.
 delay(250);
 // Записываем расстояние до препятствия впереди. 170 Глава 11
 Dist_right = sonar.distanceRead(CM);
 neck.write(front_ang);
 // Если условия позволяют двигаться прямо
 if((Dist_right>=8)&&(Dist_right<=15)&&(Dist_front>=15))
 {
 forward(); // едем вперед.
 delay(time_10cm);
 }
 else
 {// если появился поворот направо
 if(Dist_right>15)
 {
 // если абсолютно уперлись в стену!
 // узкий правый поворот.
 if(Dist_front<5)
 {
 right(); // поворачиваем направо.
 delay(time_90);
 }
 else // нормальный правый поворот.
 { // поворот направо в движении.
 right();
 delay(time_90/3);
 forward(); // едем вперед 7 см.
 delay(time_90/3);
 }
 }
 else
 // если уперлись в стену.
 if(Dist_front<15)
 {
 left(); // поворачиваем налево.
 delay(time_90);
 }
 else
 { // подворот налево в движении.
 left();
 delay(time_90/3);
 forward(); // едем вперед 5 см.
 delay(time_90/3);
 }
 }
}
