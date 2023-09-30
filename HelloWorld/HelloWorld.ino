#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int time_d = 200;
const int time_c = 2000;
int sensorValue = 0;

byte left_brow[8]={B00000,
                   B11111,
                   B00000,
                   B01110,
                   B10001,
                   B10101,
                   B10001,
                   B01110
};

byte right_brow[8]={B00000,
                    B11111,
                    B00000,
                    B01110,
                    B10001,
                    B10101,
                    B10001,
                    B01110
};

byte lefts_brow[8]={B11100,
                    B00111,
                    B00000,
                    B01110,
                    B10001,
                    B10101,
                    B10001,
                    B01110
};

byte rights_brow[8]={B00111,
                     B11100,
                     B00000,
                     B01110,
                     B10001,
                     B10101,
                     B10001,
                     B01110
};

byte rightu_brow[8]={B11100,
                     B00111,
                     B00000,
                     B01110,
                     B10001,
                     B10101,
                     B10001,
                     B01110
};

byte leftu_brow[8]={B00111,
                    B11100,
                    B00000,
                    B01110,
                    B10001,
                    B10101,
                    B10001,
                    B01110
};

byte leftl_brow[8]={B00000,
                    B11111,
                    B00000,
                    B01110,
                    B10001,
                    B11001,
                    B10001,
                    B01110
};

byte rightl_brow[8]={B00000,
                     B11111,
                     B00000,
                     B01110,
                     B10001,
                     B11001,
                     B10001,
                     B01110
};

byte leftr_brow[8]={B00000,
                    B11111,
                    B00000,
                    B01110,
                    B10001,
                    B10011,
                    B10001,
                    B01110
};

byte rightr_brow[8]={B00000,
                     B11111,
                     B00000,
                     B01110,
                     B10001,
                     B10011,
                     B10001,
                     B01110
};

byte middle_mouth[8]={B00000,
                      B00000,
                      B00000,
                      B00000,
                      B00000,
                      B11111,
                      B00000,
                      B00000
};

byte left_line[8]={B00000,
                   B00000,
                   B00000,
                   B00000,
                   B00000,
                   B00011,
                   B00000,
                   B00000
};

byte right_line[8]={B00000,
                    B00000,
                    B00000,
                    B00000,
                    B00000,
                    B11000,
                    B00000,
                    B00000
};

byte lefta_brow[8]={B00000,
                    B00000,
                    B11100,
                    B00111,
                    B00000,
                    B00000,
                    B00000,
                    B00000
};

byte righta_brow[8]={B00000,
                     B00000,
                     B00111,
                     B11100,
                     B00000,
                     B00000,
                     B00000,
                     B00000
};

byte rightb_brow[8]={B00000,
                     B00000,
                     B00000,
                     B01111,
                     B10000,
                     B00000,
                     B00000,
                     B00000
};

byte leftb_brow[8]={B00000,
                    B00000,
                    B00000,
                    B11110,
                    B00001,
                    B00000,
                    B00000,
                    B00000
};

byte c_brow[8]={B00000,
                B00000,
                B00000,
                B01110,
                B10001,
                B00000,
                B00000,
                B00000
};

byte middlea_mouth[8]={B00000,
                       B00000,
                       B00000,
                       B00000,
                       B00000,
                       B11111,
                       B00000,
                       B01110
};

void ordinary_smile() {
  lcd.createChar(1,left_brow);
  lcd.createChar(2,right_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(1));
  lcd.setCursor(3, 0);
  lcd.write(byte(2));
  
  delay(time_c);
  lcd.clear();
}

void serious_smile() {
  lcd.createChar(6,lefts_brow);
  lcd.createChar(7,rights_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(6));
  lcd.setCursor(3, 0);
  lcd.write(byte(7));

  delay(time_d);
  lcd.clear();
}

void unserious_smile() {
  lcd.createChar(8,leftu_brow);
  lcd.createChar(9,rightu_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(8));
  lcd.setCursor(3, 0);
  lcd.write(byte(9));

  delay(time_d);
  lcd.clear();
}

void ordinary_a_smile() {
  lcd.createChar(15,lefta_brow);
  lcd.createChar(16,righta_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(15));
  lcd.setCursor(3, 0);
  lcd.write(byte(16));
  
  delay(time_d);
  lcd.clear();
}

void ordinary_b_smile() {
  lcd.createChar(17,lefta_brow);
  lcd.createChar(18,righta_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(17));
  lcd.setCursor(3, 0);
  lcd.write(byte(18));
  
  delay(time_d);
  lcd.clear();
}

void ordinary_c_smile() {
  lcd.createChar(10,c_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(10));
  lcd.setCursor(3, 0);
  lcd.write(byte(10));


  delay(time_d);
  lcd.clear();
}

void ordinary_l_smile() {
  lcd.createChar(11,leftl_brow);
  lcd.createChar(12,rightl_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(11));
  lcd.setCursor(3, 0);
  lcd.write(byte(12));

  delay(time_c);
  lcd.clear();
}

void ordinary_r_smile() {
  lcd.createChar(13,leftr_brow);
  lcd.createChar(14,rightr_brow);
  lcd.setCursor(1, 0);
  lcd.write(byte(13));
  lcd.setCursor(3, 0);
  lcd.write(byte(14));

  delay(time_c);
  lcd.clear();
}

void mouth() {
  lcd.createChar(3,middle_mouth);
  lcd.createChar(4,left_line);
  lcd.createChar(5,right_line);
  
  lcd.setCursor(1, 1);
  lcd.write(byte(4));
  lcd.setCursor(2, 1);
  lcd.write(byte(3));
  lcd.setCursor(3, 1);
  lcd.write(byte(5));
}

void blinking() {
  int rn = random(6);
  if (rn == 2) {
  mouth();
  ordinary_smile();
  mouth();
  ordinary_a_smile();
  mouth();
  ordinary_b_smile();
  mouth();
  ordinary_c_smile();
  mouth();
  ordinary_b_smile();
  mouth();
  ordinary_a_smile();
  mouth();
  ordinary_smile();
  } else {
  mouth();
  ordinary_smile();    
  }
}

void setup()
{
  Serial.begin(9600);
	// initialize the LCD
	lcd.begin();
	// Turn on the blacklight and print a message.
	lcd.backlight();
  // drawing smile
}

void loop()
{
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  if(sensorValue > 950) { 
  serious_smile();
  } else if(sensorValue < 950 & sensorValue > 900) {
  mouth();
  ordinary_r_smile();  
  } else if(sensorValue < 900 & sensorValue > 850) {
  mouth();
  ordinary_l_smile();
  } else if(sensorValue < 200  & sensorValue > 100) {
  mouth();
  unserious_smile();
  } else {
  blinking();  
  }
}
