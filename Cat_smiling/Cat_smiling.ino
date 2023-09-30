#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int time_s = 100;
const int time_m = 1000;
const int time_b = 5000;

const byte eye_middle[8]={B00000,
                          B01110,
                          B10001,
                          B10101,
                          B10001,
                          B01110,
                          B00000,
                          B00000
};

const byte eye_right[8]={B00000,
                         B01110,
                         B10001,
                         B10011,
                         B10001,
                         B01110,
                         B00000,
                         B00000
};

const byte eye_left[8]={B00000,
                        B01110,
                        B10001,
                        B11001,
                        B10001,
                        B01110,
                        B00000,
                        B00000
};

const byte brow_c[8]={B00000,
                      B00000,
                      B00000,
                      B00000,
                      B00000,
                      B00000,
                      B01110,
                      B10001
};

const byte brow_line[8]={B00000,
                         B00000,
                         B00000,
                         B00000,
                         B00000,
                         B00000,
                         B11111,
                         B00000
};

 const byte brow_rising[8]={B00000,
                            B00000,
                            B00000,
                            B00000,
                            B00000,
                            B00000,
                            B00111,
                            B11100
};

const byte brow_falling[8]={B00000,
                            B00000,
                            B00000,
                            B00000,
                            B00000,
                            B00000,
                            B11100,
                            B00111
};

const byte mouth_f[8]={B00000,
                       B00000,
                       B00011,
                       B00000,
                       B00000,
                       B00000,
                       B00000,
                       B00000
};

const byte mouth_s[8]={B00000,
                       B00000,
                       B11111,
                       B00000,
                       B00000,
                       B00000,
                       B00000,
                       B00000
};

const byte mouth_t[8]={B00000,
                       B00000,
                       B11000,
                       B00000,
                       B00000,
                       B00000,
                       B00000,
                       B00000
};

void serious() {
  lcd.setCursor(11, 1);
  lcd.write(byte(7));
  lcd.setCursor(13, 1);
  lcd.write(byte(6));
  lcd.setCursor(11, 2);
  lcd.write(byte(1));
  lcd.setCursor(13, 2);
  lcd.write(byte(1));
  
  delay(time_m);
  lcd.clear();
}

void unserious() {
  lcd.setCursor(11, 1);
  lcd.write(byte(6));
  lcd.setCursor(13, 1);
  lcd.write(byte(7));
  lcd.setCursor(11, 2);
  lcd.write(byte(1));
  lcd.setCursor(13, 2);
  lcd.write(byte(1));
  
  delay(time_m);
  lcd.clear();
}

void middle() {
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 2);
  lcd.write(byte(1));
  lcd.setCursor(13, 2);
  lcd.write(byte(1));
  
  delay(time_m);
  lcd.clear();
}

void c() {
  lcd.setCursor(11, 1);
  lcd.write(byte(4));
  lcd.setCursor(13, 1);
  lcd.write(byte(4));
  lcd.setCursor(11, 2);
  lcd.write(byte(1));
  lcd.setCursor(13, 2);
  lcd.write(byte(1));
  
  delay(time_m);
  lcd.clear();
}

void line() {
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 2);
  lcd.write(byte(1));
  lcd.setCursor(13, 2);
  lcd.write(byte(1));
  
  delay(time_m);
  lcd.clear();
}

void right() {
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 2);
  lcd.write(byte(2));
  lcd.setCursor(13, 2);
  lcd.write(byte(2));
  
  delay(time_m);
  lcd.clear();
}

void left() {
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  lcd.setCursor(11, 2);
  lcd.write(byte(3));
  lcd.setCursor(13, 2);
  lcd.write(byte(3));
  
  delay(time_m);
  lcd.clear();
}

void mouth() {
  lcd.setCursor(11, 3);
  lcd.write(byte(8));
  lcd.setCursor(12, 3);
  lcd.write(byte(9));
  lcd.setCursor(13, 3);
  lcd.write(byte(10));

  
  delay(time_m);
  lcd.clear();
}

void blinking() {
  int randSeed = random(5);
  if(randSeed == 2) {
  mouth();
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  delay(time_s);
  lcd.clear();
  mouth();
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  delay(time_s);
  lcd.clear();
  mouth();
  lcd.setCursor(11, 1);
  lcd.write(byte(5));
  lcd.setCursor(13, 1);
  lcd.write(byte(5));
  delay(time_s);
  lcd.clear();
  } else {
  middle();
  mouth();
  }
}

void setup() {
  Serial.begin(9600);
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.createChar(1,eye_middle);
  lcd.createChar(2,eye_right);
  lcd.createChar(3,eye_left);
  lcd.createChar(4,brow_c);
  lcd.createChar(5,brow_line);
  lcd.createChar(6,brow_rising);
  lcd.createChar(7,brow_falling);
  lcd.createChar(8,mouth_f);
  lcd.createChar(9,mouth_s);
  lcd.createChar(10,mouth_t);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  if(sensorValue > 950) {
     serious();
     mouth();
  } else if (sensorValue > 900) {
    right();
    mouth();
  } else if (sensorValue > 850) {
    left();
    mouth();
  } else if (sensorValue > 800) {
    line();
    mouth();
  } else if (sensorValue > 750) {
    c();
    mouth(); 
  } else if (sensorValue > 700) {
    middle();
    mouth(); 
  } else if (sensorValue > 750) {
    unserious();
    mouth(); 
  } else {
    blinking() ;
  }
}
