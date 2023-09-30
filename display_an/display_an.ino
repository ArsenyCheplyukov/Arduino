#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int time_s = 100;
const int time_m = 1000;
const int time_b = 5000;

byte eye_a[8] = {B00000,
                 B00000,
                 B01110,
                 B11111,
                 B11011,
                 B11011,
                 B11111,
                 B01110
                };

byte eye_r[8] = {B00000,
                 B00000,
                 B01110,
                 B11111,
                 B11101,
                 B11101,
                 B11111,
                 B01110
                };

byte eye_l[8] = {B00000,
                 B00000,
                 B01110,
                 B11111,
                 B10111,
                 B10111,
                 B11111,
                 B01110
                };

byte eye_u[8] = {B00000,
                 B00000,
                 B01110,
                 B11011,
                 B11011,
                 B11111,
                 B11111,
                 B01110
                };

byte eye_d[8] = {B00000,
                 B00000,
                 B01110,
                 B11111,
                 B11111,
                 B11011,
                 B11011,
                 B01110
                };


byte mouth_f[8] = {B00000,
                   B11100,
                   B00011,
                   B00000,
                   B11111,
                   B00000,
                   B00011,
                   B11100
                  };

byte mouth_s[8] = {B00000,
                   B00000,
                   B00000,
                   B10000,
                   B10000,
                   B10000,
                   B00000,
                   B00000
                  };

byte mouth_t[8] = {B00000,
                   B00111,
                   B11000,
                   B00000,
                   B11111,
                   B00000,
                   B11000,
                   B00111
                  };

byte mouth_fo[8] = {B00000,
                    B00000,
                    B00000,
                    B00001,
                    B00001,
                    B00001,
                    B00000,
                    B00000
                   };

void mouth() {
  lcd.createChar(1, mouth_f);
  lcd.createChar(2, mouth_s);
  lcd.createChar(3, mouth_t);
  lcd.createChar(4, mouth_fo);

  lcd.setCursor(8, 2);
  lcd.write(byte(1));
  lcd.setCursor(9, 2);
  lcd.write(byte(2));
  lcd.setCursor(11, 2);
  lcd.write(byte(4));
  lcd.setCursor(12, 2);
  lcd.write(byte(3));
}

void eye_simple() {
  lcd.createChar(7, eye_a);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_m);
  lcd.clear();
}

void eye_right() {
  lcd.createChar(5, eye_r);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  delay(time_m);
  lcd.clear();
}

void eye_left() {
  lcd.createChar(6, eye_l);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(6));
  lcd.setCursor(11, 1);
  lcd.write(byte(6));
  delay(time_m);
  lcd.clear();
}

void eye_up() {
  lcd.createChar(8, eye_u);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(8));
  lcd.setCursor(11, 1);
  lcd.write(byte(8));
  delay(time_m);
  lcd.clear();
}

void eye_down() {
  lcd.createChar(8, eye_d);        // нельзя задавать более восьми активных символов, РАБОТАЕТ НЕСТАБИЛЬНО
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(8));
  lcd.setCursor(11, 1);
  lcd.write(byte(8));
  delay(time_m);
  lcd.clear();
}

void setup() {
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop() {
  //  int sensorValue = analogRead(A0);
  //  Serial.println(sensorValue);
  eye_simple();
  eye_right();
  eye_simple();
  eye_up();
  eye_simple();
  eye_left();
  eye_simple();
  eye_down();
  eye_simple();
}
