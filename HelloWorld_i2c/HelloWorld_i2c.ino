#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



#define BACKLIGHT_PIN     13

LiquidCrystal_I2C lcd(0x27);  // Set the LCD I2C address

//LiquidCrystal_I2C lcd(0x27, BACKLIGHT_PIN);  // Set the LCD I2C address

void setup()
{
  lcd.begin(20, 4);                      // Инициализация дисплея  
  //lcd.backlight();                 // Подключение подсветки
  
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("Hello");       // Набор текста на первой строке
  //lcd.setCursor(0,1);              // Установка курсора в начало второй строки
  //lcd.print("ArduinoMaster");       // Набор текста на второй строке
}
void loop()
{
}
