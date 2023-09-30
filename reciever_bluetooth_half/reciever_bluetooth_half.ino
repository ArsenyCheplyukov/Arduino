#include <SoftwareSerial.h>    // подключаем библиотеку
SoftwareSerial mySerial(19, 18); // указываем пины tx и rx

char hexaKeys[4][4] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'a', 'b', 'c'},
  {'d', 'e', 'f', 'g'}
};

const int fingers_up   = 2;
const int fingers_down = 5;
const int hand_up      = 3;
const int hand_down    = 4;
const int lever_1_up   = 7;
const int lever_1_down = 8;
const int lever_2_up   = 6;
const int lever_2_down = 9;
const int shoulder_up  = 10;
const int shoulder_down = 11;
const int right        = 12;
const int left         = 13;
const int up           = 16;
const int down         = 17;
const int button_1     = 14;
const int button_2     = 15;

void rotation(const int motor_first)
{
  digitalWrite(motor_first, HIGH);
  delay(1000);
  digitalWrite(motor_first, LOW);
}

void moving(const char comming_answer) {
  if (comming_answer == hexaKeys[0][0]) {
    rotation(shoulder_up);
  }
  else if (comming_answer == hexaKeys[0][1]) {
    rotation(shoulder_down);
  }
  else if (comming_answer == hexaKeys[0][2]) {
    rotation(lever_2_up);
  }
  else if (comming_answer == hexaKeys[0][3]) {
    rotation(lever_2_down);
  }
  else if (comming_answer == hexaKeys[1][0]) {
    rotation(lever_1_up);
  }
  else if (comming_answer == hexaKeys[1][1]) {
    rotation(lever_1_down);
  }
  else if (comming_answer == hexaKeys[1][2]) {
    rotation(fingers_up);
  }
  else if (comming_answer == hexaKeys[1][3]) {
    rotation(hand_up);
  }
  else if (comming_answer == hexaKeys[2][0]) {
    rotation(button_1);
  }
  else if (comming_answer == hexaKeys[2][1]) {
    rotation(fingers_down);
  }
  else if (comming_answer == hexaKeys[2][2]) {
    rotation(button_2);
  }
  else if (comming_answer == hexaKeys[2][3]) {
    rotation(hand_down);
  }
  else if (comming_answer == hexaKeys[3][0]) {
    rotation(up);
  }
  else if (comming_answer == hexaKeys[3][1]) {
    rotation(left);
  }
  else if (comming_answer == hexaKeys[3][2]) {
    rotation(right);
  }
  else if (comming_answer == hexaKeys[3][3]) {
    rotation(down);
  }
}


void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(fingers_up, OUTPUT);
  pinMode(fingers_down, OUTPUT);
  pinMode(hand_up, OUTPUT);
  pinMode(hand_down, OUTPUT);
  pinMode(lever_1_up, OUTPUT);
  pinMode(lever_1_down, OUTPUT);
  pinMode(lever_2_up, OUTPUT);
  pinMode(lever_2_down, OUTPUT);
  pinMode(shoulder_up, OUTPUT);
  pinMode(shoulder_down, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  pinMode(button_1, OUTPUT);
  pinMode(button_2, OUTPUT);

  Serial.begin(9600);         // включаем hardware-порт
  mySerial.begin(9600);  // включаем software-порт

  Serial.println("start setting");
}

void loop() {
  if (mySerial.available()) { // раньше было IF
    char c = mySerial.read();  // читаем из software-порта
    if (c != '0') {
      Serial.print(c);                   // пишем в hardware-порт
      moving(c);
    }
  }
  if (Serial.available()) {
    char c = Serial.read();      // читаем из hardware-порта
    mySerial.write(c);            // пишем в software-порт
  }
}
