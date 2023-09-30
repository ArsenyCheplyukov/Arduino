#include <Keypad.h>
#include <SoftwareSerial.h>    // подключаем библиотеку
SoftwareSerial mySerial(2, 3); // указываем пины tx и rx

const byte statePin = 13;

String massage1 = "AT";
String massage2 = "AT+INQ";
String massage3 = "AT+CONN1";

char hexaKeys[4][4] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
  {'9', 'a', 'b', 'c'},
  {'d', 'e', 'f', 'g'}
};

byte rowPins[4] = {12, 11, 9, 10};
byte colPins[4] = {8, 7, 6, 5};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, 4, 4 );

void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  Serial.begin(9600);         // включаем hardware-порт
  mySerial.begin(9600);  // включаем software-порт
}

void loop() {
if (digitalRead(statePin) == HIGH) { 
  if (mySerial.available()) {
    char c = mySerial.read();  // читаем из software-порта
    Serial.print(c);                   // пишем в hardware-порт
  }
  if (Serial.available()) {
    char c = Serial.read();      // читаем из hardware-порта
    mySerial.write(c);            // пишем в software-порт
  }
  char key = customKeypad.getKey();
  while (key) {
    mySerial.write(key); // Передаем название нажатой клавиши в сериал порт
  }
} else {
  Serial.println("start setting");
  delay(5000);
  mySerial.println(massage1);
  Serial.println("1");
  delay(5000);
  mySerial.println(massage2);
  Serial.println("2");
  delay(5000);
  mySerial.println(massage3);
  Serial.println("3");
  delay(5000);
  Serial.println("end of connecting");
}
}
