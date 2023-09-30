#include <SoftwareSerial.h>
const int bRX_Pin      = 2; // Подключаем контакт TXD Bluetooth к выводу 2
const int bTX_Pin      = 3; // Подключаем контакт RXD Bluetooth к выводу 3

const int hexaKeys[4][4] = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 16}
};

const int fingers_up   = 0; 
const int fingers_down = 5;
const int hand_up      = 1;
const int hand_down    = 4;
const int lever_1_up   = 8;
const int lever_1_down = 7;
const int lever_2_up   = 9;
const int lever_2_down = 6;
const int shoulder_up  = 12;
const int shoulder_down= 13;
const int right        = 11;
const int left         = 10;
const int up           = 16;
const int down         = 17;
const int button_1     = 14;
const int button_2     = 15;

SoftwareSerial Serial1(bRX_Pin, bTX_Pin); // Инициализируем новый последовательный порт

void rotation(const int &motor_first) 
{ 
digitalWrite(motor_first, HIGH); 
delay(1000); 
digitalWrite(motor_first, LOW); 
} 

void moving(const int &comming_answer) { 
if(comming_answer == hexaKeys[1][1]) { 
rotation(fingers_up); 
} 
else if(comming_answer == hexaKeys[1][2]) { 
rotation(fingers_down); 
} 
else if(comming_answer == hexaKeys[1][3]) { 
rotation(hand_up); 
} 
else if(comming_answer == hexaKeys[1][4]) { 
rotation(hand_down);
}
else if(comming_answer == hexaKeys[2][1]) { 
rotation(lever_1_up); 
} 
else if(comming_answer == hexaKeys[2][2]) { 
rotation(lever_1_down); 
} 
else if(comming_answer == hexaKeys[2][3]) { 
rotation(lever_2_up); 
} 
else if(comming_answer == hexaKeys[2][4]) { 
rotation(lever_2_down); 
}
else if(comming_answer == hexaKeys[3][1]) { 
rotation(shoulder_up); 
} 
else if(comming_answer == hexaKeys[3][2]) { 
rotation(shoulder_down);
} 
else if(comming_answer == hexaKeys[3][3]) { 
rotation(right); 
} 
else if(comming_answer == hexaKeys[3][4]) { 
rotation(left); 
}
else if(comming_answer == hexaKeys[4][1]) { 
rotation(up); 
} 
else if(comming_answer == hexaKeys[4][2]) { 
rotation(down);
}
} 

void setup() 
{ 
pinMode(bRX_Pin, INPUT);
pinMode(bTX_Pin, OUTPUT);
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

Serial1.begin(9600);
Serial.begin(9600); // только в отладочных целях 
Serial.print("Ready to start");
} 

void loop() 
{ 

if (Serial1.available()) {
int argument = Serial1.read();
   moving(argument);

} 
}
