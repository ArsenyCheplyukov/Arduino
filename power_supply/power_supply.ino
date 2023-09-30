///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                                                           //
//                                                                             ПЕРЕМЕННЫЕ                                                                                                    //
//                                                                                                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                             //
const int diode                     = 3;                     //порт лампочки при сбое 
const int output_1                  = 9;                     //
const int output_2                  = 10;                    //3 порта выхода импульсов 
const int output_3                  = 11;                    //
const int sin_zero                  = 2;                     //
const int standart_voltage          = A7;                    //порт текущего напряжения 
const int voltage_check             = A5;                    //эталон напряжения 
const int standart_amperage         = A4;                    //порт текущего тока 
const int amperage_check            = A6;                    //эталон тока 
const int warning_voltage           = 1000;                  //приходящее значение при котором может сгореть ардуина 
const int warning_amperage          = 210;                   //
const int period_tranzistor         = 5;                     //период в микросекундах транзистора 
const double one_persent_tranzistor = 0.05;                  //один процент периода транзистора 
const int period_thyristor          = 10000;                 //период в микросекундах тиристора 
const int one_persent_thyristor     = 50;                    //один процент периода тиристора 
const int button_IU                 = 12;                    //ток - 0 , напряжение - 1 
const int button_mode               = 13;                    //тиристор - 0, транзистор - 1 
const int button_behavior           = 6;                     // 1 - блок питания, 0 - АКБ
const int button_type               = 5;                     // КТЦ - 1, дозаряд - 0
const int charge_positive           = 7;                     //заряд положительный
const int charge_negative           = 8;                     //заряд отрицательный для КТЦ
const int MAX_charge                = 300;                   // значение напряжения через которое лучше не переступать
const int start_charge_q            = 258;                   // минимальное значение для Дозаряда
const int start_charge_CTL          = 215;                   // минимальное значение для КТЦ
                                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                                                           //
//                                                                                 ФУНКЦИИ                                                                                                   //
//                                                                                                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void generate_pulse_tranzistor(const double &one_lenght) 
{ 
 digitalWrite(output_1,HIGH);                                                                                     
 digitalWrite(output_2,HIGH);                                                                                       //генерация импульсов 
 digitalWrite(output_3,HIGH); 
 delayMicroseconds(one_lenght);                                                                                     //их постоянство 
 digitalWrite(output_1,LOW); 
 digitalWrite(output_2,LOW);                                                                                        //постепенное выключение их 
 digitalWrite(output_3,LOW); 
 delayMicroseconds( period_tranzistor - one_lenght );                                                               //постоянство нуля 
} 

void regulate_pulse_tranzistor(const int &check_COM, const int &regulate_COM, double &lenght_of_one)
{
   if(check_COM > regulate_COM) {                                                                                   //сверяю 2 порта, если больше эталон то 
      lenght_of_one += one_persent_tranzistor;                                                                      //увеличиваю скважность 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //запускаю цикл генерации 
   } else if(check_COM < regulate_COM) {                                                                            //в противном случае  
      lenght_of_one -= one_persent_tranzistor;                                                                      // отнимаю процент 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //запускаю цикл 
   } else { 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //если установился просто подаю импульсы дальше 
   } 
}

void check_for_set_tranzistor(const int &check_COM, const int &regulate_COM, double &lenght_of_one, int number) 
{ 
  if(number == 1) {
    if(check_COM < warning_voltage) {                                                                               //наличие опасного напряжения  
        regulate_pulse_tranzistor(check_COM, regulate_COM, lenght_of_one);
        }
        else { 
     digitalWrite(diode, HIGH);                                                                                      //если напряжение огромно, то включаю светодиод 
   } 
 }
 else if(number == 0) {
        if(check_COM < warning_amperage) {                                                                           //наличие опасного напряжения  
        regulate_pulse_tranzistor(check_COM, regulate_COM, lenght_of_one);
        }
        else { 
     digitalWrite(diode, HIGH);                                                                                      //если напряжение огромно, то включаю светодиод 
    }
  }
} 
 
void generate_pulse_thyristor(const int &one_lenght) 
{ 
 if(digitalRead(sin_zero) == 1)                                                                                      //нахождение синуса нуля
{  
 while(!digitalRead(sin_zero))                                                                                       // выполняем при нуле в синусоиде
 { 
   delayMicroseconds(period_thyristor - one_lenght);                                                                 // длинный ноль  
   digitalWrite(output_1,LOW); 
   digitalWrite(output_2,LOW);                                                                                       // генерация нуля
   digitalWrite(output_3,LOW);
   delayMicroseconds(one_lenght);                                                                                    // длина единиц
   digitalWrite(output_1,HIGH); 
   digitalWrite(output_2,HIGH);                                                                                      // включение единиц 
   digitalWrite(output_3,HIGH); 
  } 
 } 
} 

void regulate_pulse_thyristor(const int &check_COM, const int &regulate_COM, int &lenght_of_one) {
     if(check_COM > regulate_COM)  {                                                      //сверяю 2 порта, если больше эталон то 
     lenght_of_one += one_persent_thyristor;                                              //увеличиваю импульс
     generate_pulse_thyristor(lenght_of_one);                                             //запускаю цикл генерации 
     }
     else if(check_COM < regulate_COM) {                                                  //в противном случае 
     lenght_of_one -= one_persent_thyristor;                                              // отнимаю процент 
     generate_pulse_thyristor(lenght_of_one);                                             //запускаю цикл 
     }
     else { 
     generate_pulse_thyristor(lenght_of_one);                                             //если установился просто подаю импульсы дальше 
     } 
 }
 
void check_for_set_thyristor(const int &check_COM, const int &regulate_COM, int &lenght_of_one, int number) 
{ 
 if(number == 1) {                                                                        // если напряжение
   if(check_COM < warning_voltage) {                                           //наличие опасного напряжения 
      regulate_pulse_thyristor(check_COM, regulate_COM, lenght_of_one);
   }
   else { 
   digitalWrite(diode, HIGH);                                                  //если напряжение огромно, то включаю светодиод 
   }
 }
 else if(number == 0) {                                                        // если ток
     if(check_COM < warning_amperage) {                                        //наличие опасного тока
      regulate_pulse_thyristor(check_COM, regulate_COM, lenght_of_one);
     }
     else { 
 digitalWrite(diode, HIGH);                                                     //если напряжение огромно, то включаю светодиод 
  }
 }
} 

void charge_until_end() {                                                        // дозаряд
 if(digitalRead(button_mode) == 1) {     
 double one_lenght = 0; 
 generate_pulse_tranzistor(0.05);                                 
                                                                                  // проверяем выборку дозаряда
       if(analogRead(standart_voltage) <= MAX_charge) {                           // не переступать за максимальное значение
        digitalWrite(charge_positive,HIGH);                                       // включаем порт заряда
          if(analogRead(standart_amperage) < analogRead(amperage_check)) {        // регулировка тока   
               one_lenght += one_persent_tranzistor;
               generate_pulse_tranzistor(one_lenght); 
            
          } else if(analogRead(standart_amperage) > analogRead(amperage_check)) {
                one_lenght -= one_persent_tranzistor;
                generate_pulse_tranzistor(one_lenght);
            } else {
                generate_pulse_tranzistor(one_lenght);
            }
       }  else {                                                                  //пока не достигнет минимального порога
        digitalWrite(charge_positive,LOW);                                        //выключаю ЗАРЯД
        
       }
    
 } else if(digitalRead(button_mode) == 0) {  
   double one_lenght = 200; 
    generate_pulse_thyristor(one_lenght);                                         //то же самое, но с тиристором
       if(analogRead(standart_voltage) <= MAX_charge) {
        digitalWrite(charge_positive,HIGH);
          if(analogRead(standart_amperage) < analogRead(amperage_check)) {
               one_lenght += one_persent_thyristor;
               generate_pulse_thyristor(one_lenght); 
            
          } else if(analogRead(standart_amperage) > analogRead(amperage_check)) {
                one_lenght -= one_persent_thyristor;
                generate_pulse_thyristor(one_lenght);
            }
       }  else {
        digitalWrite(charge_positive,LOW);
        
       }
    
 }
}

void power_supply() 
{
   digitalWrite(charge_positive, HIGH);
   if(digitalRead(button_mode) == 1){
   double nanodelay = one_persent_tranzistor;   
   generate_pulse_tranzistor(nanodelay);                                         // минимальная скважность, позже увеличиваю 
   if(digitalRead(button_IU) == 1) {  
     int current_voltage = analogRead(standart_voltage); 
     int checked_voltage = analogRead(voltage_check);                            //считываю показания с портов 
     check_for_set_tranzistor(checked_voltage, current_voltage, nanodelay, 1);   //запускаю функцию 
    
 } else {  
   int current_amperage = analogRead(standart_amperage); 
   int checked_amperage = analogRead(amperage_check);                            //аналогично первому случаю  
   check_for_set_tranzistor(checked_amperage, current_amperage, nanodelay, 0);   
    }
  
} else if(digitalRead(button_mode) == 0) {
  generate_pulse_thyristor(200); 
  int microdelay = 200 + one_persent_thyristor-5;                                 // минимальные импульсы, позже увеличиваю 
    if(digitalRead(button_IU) == 1) {  
       int current_voltage = analogRead(standart_voltage); 
       int checked_voltage = analogRead(voltage_check);                           //считываю показания с портов 
       check_for_set_thyristor(checked_voltage, current_voltage, microdelay, 1);  //запускаю функцию 
        
    } else {
     int current_amperage = analogRead(standart_amperage); 
     int checked_amperage = analogRead(amperage_check);                            //аналогично первому случаю  
     check_for_set_thyristor(checked_amperage, current_amperage, microdelay, 0); 
      } 
    }
}

void CTL() 
{
   if(digitalRead(button_mode) == 1) {                                              //проверяю выборку
   double one_lenght = 0;   
   generate_pulse_tranzistor(0.05);                                                 //создаю длину
       if(analogRead(standart_voltage) <= MAX_charge) {                             //проверяю на максимальное значение
        digitalWrite(charge_positive,HIGH);                                         //выключаю РАЗРЯД, включаю ЗАРЯД
        digitalWrite(charge_negative,LOW);
          if(analogRead(standart_amperage) < analogRead(amperage_check)) {          //сверяю токи
               one_lenght += one_persent_tranzistor;               
               generate_pulse_tranzistor(one_lenght); 
            
          } else if(analogRead(standart_amperage) > analogRead(amperage_check)) {
                one_lenght -= one_persent_tranzistor;                                //обрабатываю импульсы транзистора
                generate_pulse_tranzistor(one_lenght);
            } else {
                generate_pulse_tranzistor(one_lenght);
            }
       }  else {                                                                     //пока не достигнет минимального порога
        digitalWrite(charge_positive,LOW);                                             
        digitalWrite(charge_negative,HIGH);                                          //включаю РАЗРЯД, выключаю ЗАРЯД
       }
 } else if(digitalRead(button_mode) == 0) {
  generate_pulse_thyristor(200);                                                     //то же самое, только с тиристором
     double one_lenght = 200;
       if(analogRead(standart_voltage) <= MAX_charge) {
        digitalWrite(charge_positive,HIGH);
        digitalWrite(charge_negative,LOW);
          if(analogRead(standart_amperage) < analogRead(amperage_check)) {
               one_lenght += one_persent_thyristor;
               generate_pulse_thyristor(one_lenght); 
          }
          else if(analogRead(standart_amperage) > analogRead(amperage_check)) {
                one_lenght -= one_persent_thyristor;
                generate_pulse_thyristor(one_lenght);
            }
       }  else {
        digitalWrite(charge_positive,LOW);
        digitalWrite(charge_negative,HIGH);
   }
 }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                                                           //
//                                                                                   ОСНОВА                                                                                                  //
//                                                                                                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{ 
  // configure hardware timer2 to generate a fast PWM on OC2B (Arduino digital pin 3)
  // set pin high on overflow, clear on compare match with OCR2B
  TCCR1A = TCCR1A & 0xe0 | 1;
  TCCR1B = TCCR1B & 0xe0 | 0x09;  
  OCR2A = 159;  // top/overflow value is 159 => produces a 100 kHz PWM
 Serial.begin(9600); 
 pinMode(button_IU ,INPUT); 
 pinMode(standart_voltage ,INPUT); 
 pinMode(voltage_check ,INPUT); 
 pinMode(standart_amperage,INPUT); //подключаю всё 
 pinMode(amperage_check ,INPUT); 
 pinMode(output_1 ,OUTPUT); 
 pinMode(output_2 ,OUTPUT); 
 pinMode(output_3 ,OUTPUT); 
 pinMode(button_mode, INPUT);
 pinMode(sin_zero,INPUT);
 pinMode(charge_positive,OUTPUT);
 pinMode(charge_negative,OUTPUT);
 pinMode(button_type,INPUT);
 pinMode(button_behavior,INPUT);
} 
 
void loop() 
{ 
 while(1)
 {
 if(digitalRead(button_behavior) == 1) 
 {
     power_supply();
  } 
    else
  {
    if(digitalRead(button_type) == 0) 
    {
        charge_until_end();
    } 
      else 
      {
      CTL();
      }
    }
  }
}
