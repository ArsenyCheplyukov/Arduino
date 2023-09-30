//передатчик с 4 кнопками
#include <SoftwareSerial.h>
#include <Keypad.h>

const int bRX_Pin = 2; // Подключаем контакт TXD Bluetooth к выводу 2
const int bTX_Pin = 3; // Подключаем контакт RXD Bluetooth к выводу 3
// pins from reciever
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

const int hexaKeys[4][4] = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 16}
};

byte rowPins[4] = {11, 10, 9, 8}; 
byte colPins[4] = {7, 6, 5, 4}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4);
SoftwareSerial Serial1(bRX_Pin, bTX_Pin);

void keyboard_to_bluetooth(const int &button_number) {
if(button_number == hexaKeys[1][1]) 
{  
   Serial1.print(hexaKeys[1][1]);
} 
else if(button_number == hexaKeys[1][2]) 
{ 
   Serial1.print(hexaKeys[1][2]); 
} 
else if(button_number == hexaKeys[1][3]) 
{ 
   Serial1.print(hexaKeys[1][3]);
} 
else if(button_number == hexaKeys[1][4]) 
{ 
   Serial1.print(hexaKeys[1][4]); 
} 
else if(button_number == hexaKeys[2][1]) 
{ 
   Serial1.print(hexaKeys[2][1]); 
} 
else if(button_number == hexaKeys[2][2]) 
{ 
   Serial1.print(hexaKeys[2][2]); 
} 
else if(button_number == hexaKeys[2][3]) 
{ 
   Serial1.print(hexaKeys[2][3]); 
}
else if(button_number == hexaKeys[2][4]) 
{
   Serial1.print(hexaKeys[2][4]); 
}
else if(button_number == hexaKeys[3][1]) 
{ 
   Serial1.print(hexaKeys[3][1]); 
} 
else if(button_number == hexaKeys[3][2]) 
{ 
   Serial1.print(hexaKeys[3][2]); 
}
else if(button_number == hexaKeys[3][3]) 
{ 
   Serial1.print(hexaKeys[3][3]); 
} 
else if(button_number == hexaKeys[3][4]) 
{ 
   Serial1.print(hexaKeys[3][4]); 
} 
else if(button_number == hexaKeys[4][1]) 
{ 
   Serial1.print(hexaKeys[4][1]); 
}
else if(button_number == hexaKeys[4][2]) 
{ 
   Serial1.print(hexaKeys[4][2]); 
} 
else if(button_number == hexaKeys[4][3]) 
{ 
   Serial1.print(hexaKeys[4][3]); 
} 
else if(button_number == hexaKeys[4][4]) 
{ 
   Serial1.print(hexaKeys[4][4]); 
}
}
void setup() 
{ 
pinMode(bRX_Pin, INPUT);
pinMode(bTX_Pin, OUTPUT);
pinMode(4, INPUT);
pinMode(5, INPUT);
pinMode(6, INPUT);
pinMode(7, INPUT);
pinMode(8, INPUT);
pinMode(9, INPUT);
pinMode(10, INPUT);
pinMode(11, INPUT);
Serial1.begin(9600);
Serial.begin(9600); // для отладочных целей 
}

void loop() 
{ 
Serial1.write("AT+INQ");
Serial1.write("AT+CONN1");
  
int button_number = customKeypad.getKey();
  
if (button_number){
   keyboard_to_bluetooth(button_number);
} 

}
