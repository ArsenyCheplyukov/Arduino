//передатчик с 4 кнопками 
#include <RH_ASK.h> 
#include <SPI.h> // в скетче не используется, но нужна для компиляции 

RH_ASK driver;
 
const int button1 = 2;
const int button2 = 3;
const int button3 = 4;
const int button4 = 5;

void moving_loop(uint8_t * choise) // отправка сообщения о нужной стороне
{
   driver.send((uint8_t *)choise, strlen(choise)); 
   driver.waitPacketSent(); 
   delay(1000);
}

void tapping_loop() 
{
    if(digitalRead(2) == HIGH) 
    { 
        moving_loop("right");
    } 
    else if(digitalRead(3) == HIGH) 
    { 
        moving_loop("left");
    } 
    else if(digitalRead(4) == HIGH) 
    { 
        moving_loop("down");
    } 
    else if(digitalRead(5) == HIGH) 
    { 
        moving_loop("up");
    }
}
void setup() 
{ 
Serial.begin(9600);                 // для отладочных целей 
pinMode(2,INPUT);                   // подключение 1ой кнопки 
pinMode(3,INPUT);                   //подключение 2ой кнопки 
pinMode(4,INPUT);                   //подключение 3ей кнопки 
pinMode(5,INPUT);                   //подключение 4ой кнопки 
   if (!driver.init()) 
   Serial.println("init failed");   // "Инициализация не удалась" 
}
 
void loop() 
{
   tapping_loop(); 
}
