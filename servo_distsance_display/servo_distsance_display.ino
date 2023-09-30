#include <iarduino_HC_SR04.h>           // Подключаем библиотеку
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> //используем библиотеку для работы с сервоприводом

Servo servo; //объявляем переменную servo типа Servo
// Set the LCD address to 0x27 for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x27, 20, 4);

iarduino_HC_SR04 sensor(2, 3);          // Объявляем объект sensor, указывая номера arduino подключенные к выводам TRIG и ECHO датчика

const int time_s = 100;
const int time_m = 1000;
const int itter_num = 5;

// Можно использовать любые выводы Arduino


byte eye_line[8] = {B00000,
                    B00000,
                    B00000,
                    B00000,
                    B00000,
                    B11111,
                    B00000,
                    B00000
                   };

byte eye_lefts[8] = {B00000,
                     B00000,
                     B00000,
                     B00000,
                     B11100,
                     B00111,
                     B00000,
                     B00000
                    };

byte eye_rights[8] = {B00000,
                      B00000,
                      B00000,
                      B00000,
                      B00111,
                      B11100,
                      B00000,
                      B00000
                     };


byte eye_c[8] = {B00000,
                 B00000,
                 B00000,
                 B00000,
                 B00000,
                 B01110,
                 B10001,
                 B00000
                };



byte eye_a_b[8] = {B00000,
                   B00000,
                   B01110,
                   B10001,
                   B10001,
                   B10001,
                   B10001,
                   B01110
                  };

byte eye_a[8] = {B00000,
                 B00000,
                 B01110,
                 B11111,
                 B11011,
                 B11011,
                 B11111,
                 B01110
                };

byte eye_a_s[8] = {B00000,
                   B00000,
                   B01110,
                   B11111,
                   B11111,
                   B11111,
                   B11111,
                   B01110
                  };

byte eye_r_b[8] = {B00000,
                   B00000,
                   B01100,
                   B11000,
                   B10000,
                   B10000,
                   B11000,
                   B01100
                  };

byte eye_r[8] = {B00000,
                 B00000,
                 B01110,
                 B11101,
                 B11000,
                 B11000,
                 B11101,
                 B01110
                };

byte eye_r_s[8] = {B00000,
                   B00000,
                   B01110,
                   B11111,
                   B11101,
                   B11101,
                   B11111,
                   B01110
                  };

byte eye_l_s[8] = {B00000,
                   B00000,
                   B01110,
                   B11111,
                   B10111,
                   B10111,
                   B11111,
                   B01110
                  };

byte eye_l[8] = {B00000,
                 B00000,
                 B01110,
                 B10111,
                 B10011,
                 B10011,
                 B10111,
                 B01110
                };

byte eye_l_b[8] = {B00000,
                   B00000,
                   B01110,
                   B00111,
                   B00011,
                   B00011,
                   B00111,
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

void eye_simple_big() {
  lcd.createChar(5, eye_a_b);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_m);
  lcd.clear();
}

void eye_simple() {
  lcd.createChar(5, eye_a);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_m);
  lcd.clear();
}

void eye_simple_small() {
  lcd.createChar(5, eye_a_s);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_m);
  lcd.clear();
}

void eye_right_big() {
  lcd.createChar(6, eye_r_b);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  delay(time_m);
  lcd.clear();
}

void eye_right() {
  lcd.createChar(6, eye_r);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  delay(time_m);
  lcd.clear();
}

void eye_right_small() {
  lcd.createChar(6, eye_r_s);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  delay(time_m);
  lcd.clear();
}

void eye_left_big() {
  lcd.createChar(7, eye_l_b);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(6));
  lcd.setCursor(11, 1);
  lcd.write(byte(6));
  delay(time_m);
  lcd.clear();
}

void eye_left() {
  lcd.createChar(7, eye_l);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(6));
  lcd.setCursor(11, 1);
  lcd.write(byte(6));
  delay(time_m);
  lcd.clear();
}

void eye_left_small() {
  lcd.createChar(7, eye_l_s);
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

void blinking() {
  lcd.createChar(5, eye_lefts);
  lcd.createChar(6, eye_rights);
  lcd.createChar(7, eye_line);
  lcd.createChar(8, eye_c);
  mouth();
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(6));
  delay(time_s);
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_s);
  lcd.setCursor(9, 1);
  lcd.write(byte(8));
  lcd.setCursor(11, 1);
  lcd.write(byte(8));
  delay(time_s);
  lcd.setCursor(9, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  delay(time_s);
  lcd.setCursor(9, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 1);
  lcd.write(byte(6));

  delay(time_m);
  lcd.clear();
}

void cat_mimic_zero(const int & data_one) {
  if (data_one > 2) {
    eye_right_small();
  } else if (data_one > 1) {
    eye_right();
  } else {
    eye_right_big();
  }
}

void cat_mimic_ninety (const int & data_one) {
  if (data_one > 2) {
    eye_simple_small();
  } else if (data_one > 1) {
    eye_simple();
  } else {
    eye_simple_big();
  }
}

void cat_mimic_hundred (const int & data_one) {
  if (data_one > 2) {
    eye_left_small();
  } else if (data_one > 1) {
    eye_left();
  } else {
    eye_left_big();
  }
}

void cat_mimic(const int & data_one, const int & randnum, const int & type) {
  if (randnum == 0) {
    blinking();
  } else {
    if (type == 1) {
      cat_mimic_zero(data_one);
    } else if (type == 2) {
      cat_mimic_ninety(data_one);
    } else {
      cat_mimic_hundred(data_one);
    }
  }
}

void setup() {
  servo.attach(4); //привязываем привод к порту 4
  Serial.begin(9600);                   // Инициализация передачи данных в монитор последовательного порта
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop() {

  servo.write(0);
  for (int i = 0; i <= itter_num; i++) {
    int distone = round(sensor.distance() / 100);
    Serial.println(distone);
    int randnum = random(10);
    cat_mimic(distone, randnum, 1);
  }
  servo.write(90);
  for (int i = 0; i <= itter_num; i++) {
    int distone = round(sensor.distance() / 100);
    Serial.println(distone);
    int randnum = random(10);
    cat_mimic(distone, randnum, 2);
  }
  servo.write(180);
  for (int i = 0; i <= itter_num; i++) {
    int distone = round(sensor.distance() / 100);
    Serial.println(distone);
    int randnum = random(10);
    cat_mimic(distone, randnum, 3);
  }
}
