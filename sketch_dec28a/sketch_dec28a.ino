#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); //i2c-адрес, кол-во символов, кол-во строк

void setup()
{
  lcd.begin();
  lcd.backlight();
}

void loop()
{
  for (char x=33; x<=125; x++)
  { 
  lcd.print(x); //вывод переменной на дисплей
  delay(250);
  
  if (x==125)
  lcd.clear(); //очистка дисплея
  }
}
