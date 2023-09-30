#include "LCD.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

void setup()
{  
   lcd.begin(20,4); 
   lcd.setBacklightPin(3,POSITIVE);
   lcd.setBacklight(HIGH);
   lcd.setCursor(0,0);
   lcd.print("Hello, World!");
   lcd.setCursor(0,1);
   lcd.print("This is:"); 
   lcd.setCursor(0,2);
   lcd.print("An I2C Display");   
   lcd.setCursor(4,3);
   lcd.print("Kinda Cool...");
}
void loop()
{
}
