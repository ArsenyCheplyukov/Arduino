#include<SoftwareSerial.h>

SoftwareSerial BTSerial(9, 8);

void setup() {
  Serial.begin(112500);
  Serial.println("Sketch HC-05");
  Serial.println("Arduino with HC-05 is ready");
  Serial.println("Make sure Both NL & CR are set");
  Serial.println("");
  //Set to HC-05 default baud rate, found using AT+UART.  It is usually 38400.
  BTSerial.begin(112500);
  Serial.println("BTserial started at 9600");
  Serial.println("");
}

void loop() {

  // Read from the Bluetooth module and send to the Arduino Serial Monitor
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }

  // Read from the Serial Monitor and send to the Bluetooth module
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}
