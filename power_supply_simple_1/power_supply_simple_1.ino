const int output_1 = 9;
const int output_2 = 10;
const int output_3 = 11;
const double one_persent_tranzistor = 0.1;
const int period_tranzistor = 15;
 
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

void regulate_pulse_tranzistor()
{
  int lenght_of_one = one_persent_tranzistor;
  generate_pulse_tranzistor(lenght_of_one);
  while(1) {
   if(analogRead(A5) > analogRead(A7)) {                                                                            //сверяю 2 порта, если больше эталон то 
      lenght_of_one += one_persent_tranzistor;                                                                      //увеличиваю скважность 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //запускаю цикл генерации 
   } else if(analogRead(A5) < analogRead(A7)) {                                                                     //в противном случае  
      lenght_of_one -= one_persent_tranzistor;                                                                      // отнимаю процент 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //запускаю цикл 
   } else { 
      generate_pulse_tranzistor(lenght_of_one);                                                                     //если установился просто подаю импульсы дальше 
   } 
  }
}

void loop() 
{
 TCCR1A = TCCR1A & 0xe0 | 1;
 TCCR1B = TCCR1B & 0xe0 | 0x09;  
 OCR2A = 159;
 pinMode(output_1 ,OUTPUT); 
 pinMode(output_2 ,OUTPUT); 
 pinMode(output_3 ,OUTPUT); 
 pinMode(A5,INPUT);
 pinMode(A7,INPUT);
}

void setup() 
{
  regulate_pulse_tranzistor();
}
