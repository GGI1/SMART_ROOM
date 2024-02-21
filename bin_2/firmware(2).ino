

#include <OneWire.h>
#include <DallasTemperature.h>
#include <IRremote.h>

const int A7_PIN = A7;      // Пин фоторезистора
const int ECHO_PIN = 2;     // Пин echo датчика HC-SR04
const int TRIG_PIN = 3;     // Пин trig датчика HC-SR04
const int IR_PIN = 4;       // Пин инфракрасного диода
const int TEMP_PIN = 5;     // Пин терморезистора DS18B20

unsigned long lastLightValue = 0;
unsigned long lastDistance = 0;
float lastTempC = 0.0;
uint32_t timer;

String SendStr;
char target[] = "cmd";

OneWire oneWire(TEMP_PIN);  // Создание экземпляра OneWire для взаимодействия с датчиком температуры
DallasTemperature sensors(&oneWire);  // Создание экземпляра DallasTemperature
IRrecv irrecv(IR_PIN);
decode_results results;


struct sens {
  int LV;
  unsigned long CD;
  float tC;
  
};

sens D;




  //if (comand.length()>=4)
    //comand = comand.substring(4);
   
}




void setup() {
  Serial.begin(9600);
  pinMode(A7_PIN, INPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);  // Переводим пин ИК-датчика в режим ввода
  pinMode(TEMP_PIN, INPUT);

  sensors.begin();  // Инициализация библиотеки для работы с датчиками температуры
  irrecv.enableIRIn(); // Включение приемника ИК-сигналов
}

String sens_upd(bool test){
  
  float tempC = sensors.getTempCByIndex(0);  // Получение температуры от датчика DS18B20

  int lightValue = analogRead(A7_PIN);  // Чтение значения фоторезистора

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long currentDistance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;  // Измерение расстояния



  SendStr =  String(lightValue) + "," + String(currentDistance)  + "," +  String(tempC);
  
  D.LV = lightValue;
  D.CD = currentDistance;
  D.tC = tempC;

  if (test){
    String tesst = "  Свет:" + String(D.LV) + "   Дистанция:" + String(D.CD) + "    Температура:" + String(D.tC);
    return(tesst);
  }
  return(SendStr);
}

void loop() {
  
  if (timer > 1000){
    
    Serial.println(sens_upd(0));    //Вызываем ф-ю без теста
    
    timer = millis();
   
  }
   


  if (irrecv.decode(&results)) {
    // Если получен сигнал от ИК-пульта
    Serial.print("IR Code: 0x");
    Serial.println(results.value, HEX);

    // Дополнительная обработка кода, если необходимо

    irrecv.resume(); // Подготовка к приему следующего сигнала
  }

  if (Serial.available() > 0) {           //проверка и принятие команд
    if (Serial.find(target))
      Serial.print("Выполняю команду ");
      if(Serial.find(" reboot"){
        Serial.println("ПЕРЕЗАГРУЗКА");
        asm volatile(" jmp 0");
      }
      if (Serial.find(" test")){
        Serial.println("ТЕСТ:");
        Serial.println(sens_upd(1));
        
      }   
  }





}
