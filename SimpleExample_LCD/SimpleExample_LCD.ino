#include <PCF8574.h>
#include <Wire.h>                  // Include Wire library (required for I2C devices)
#include <LiquidCrystal_I2C.h>     // Include LiquidCrystal_I2C library 
 
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows
 
void setup() {
  lcd.begin(0,0);
  lcd.init();
  lcd.setCursor(0, 0);               // Go to column 0, row 0
  lcd.print("Hello, world!");
 
}
 
void loop() {
 
}
