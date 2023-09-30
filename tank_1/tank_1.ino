//танк с приёмником 
#include <RH_ASK.h> 
#include <SPI.h> // в скетче не используется, но нужна для компиляции 

int motor1=2;
int motor11=3;
int motor22=4;
int motor2=5; 

RH_ASK driver; 

void rotation(const int &motor_first, const int &motor_second) 
{
  digitalWrite(motor_first, HIGH);
  digitalWrite(motor_second, HIGH);
  delay(1000);
  digitalWrite(motor_first, LOW);
  digitalWrite(motor_second,LOW);
}

void moving(char* comming_answer) 
{
    if(0 == strcmp((char*)comming_answer, "up")) 
    { 
             rotation(motor1, motor2);
    } 
    else if(0 == strcmp((char*)comming_answer, "down"))
    { 
            rotation(motor11, motor22);
    } 
    else if(0 == strcmp((char*)comming_answer, "right")) 
    { 
            rotation(motor1, motor22);
    } 
    else if(0 == strcmp((char*)comming_answer, "left")) 
    { 
            rotation(motor11,motor2); 
    } 
}

void setup() 
{ 
   Serial.begin(9600); // только в отладочных целях 
   if (!driver.init()) 
   Serial.println("init failed"); // "Инициализация не удалась" 
} 

void loop() 
{ 
    uint8_t buf[15] = {0}; 
    uint8_t buflen = sizeof(buf); 
    if (driver.recv(buf, &buflen)) // неблокирующая функция 
    { 
        int i;    // пришло сообщение с хорошей контрольной суммой, выгружаем его: 
        Serial.println((char*)buf); 
        moving((char*)buf);
    } 
}
