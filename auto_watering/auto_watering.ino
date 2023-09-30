const int MAX_FLOWTIME = 15; // seconds
const int MIN_FLOWTIME = 4; // seconds

// Второй регулятор управляет частотой полива от раза в день до раза в неделю
const int MAX_PERIOD = 7; // days
const int MIN_PERIOD = 1; // days

const int MAX = 1015;
const int MIN = 0;

int volumePin = A0; // Пин, к которому подцеплен регулятор, отвечающий за объём поливаемой воды
int periodPin = A1; // Пин, к которому подцепелн регулятор, отвечающий за период между поливами
int pumpPin = 5; // Пин, к которому подсоединено управление насосом

int volume;

// Процедура, включающая насос на время, заданное в volume
void water() {
  digitalWrite(pumpPin, HIGH); // включаем насос
  delay(volume);
  digitalWrite(pumpPin, LOW); // выключаем насос 
}


void setup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW); 
}

void loop() {
  // Считываем значения регуляторов (переменных резисторов) и приводим их к заданным пределам
  volume = map(analogRead(volumePin), MIN, MAX, MIN_FLOWTIME, MAX_FLOWTIME) * 1000; 
  period = map(analogRead(periodPin), MIN, MAX, MIN_PERIOD, MAX_PERIOD) * 1000 * 60 * 60 * 24; 
 
  water();
}
