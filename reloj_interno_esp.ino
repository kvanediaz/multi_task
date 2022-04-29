#include <ESP32Time.h>
ESP32Time rct;

/////////////fecha////////////
int seg = 00;
int mn = 00;
int hr = 12;
int di = 29;
int ms = 4;
int yea = 2022;

////////////////////////////////

////////////interrupciones/////////////
volatile int continterrupcion;   //evita que sea eliminada por las optimizaciones
int totalcontinterrupcion;       //contador adicional para interrrupciones
 
hw_timer_t * timer = NULL;        //apuntar el temporizador
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;   //sincroniza bucle principal con ISR
/////////////////////////////////////

TaskHandle_t TaskHandle_1;


void setup() {
 Serial.begin(9600);
rct.setTime(seg, mn, hr, di, ms, yea);

  timer = timerBegin(0, 80, true);    //iniciar temporizador 1s=80
  timerAttachInterrupt(timer, &onTimer, true);    //funcion de manejo, se ejecuta cuando halla interrupcion 
  timerAlarmWrite(timer, 1000000, true);      //valor del contador que se genera interrupcion 1s=1000000
  timerAlarmEnable(timer);   //habilita temporizdor con esta funcion

 /////  FREE RTOS  /////
   xTaskCreatePinnedToCore(
    TaskBlink, // Function to call
    "Taskname", // Name for this task, mainly for debug
    1024, // Stack size
    NULL, // pvParameters to pass to the function
    1, // Priority 
    NULL, // Task handler to use
    1 // Core where to run
    );

}

void loop() {
  String now = rct.getTime("%d/%m/%y  %r ");  //devuelve la hra con el formato espécifico %H:%M:%S
  int segundos = rct.getSecond();   //devuelve un int representando los segundos, valor entre (0-59)
  int minutos = rct.getMinute();    //devuelve un int representando los minutos, valor de (0-59)

  int hora = rct.getHour();   //devuelve el in representando la hora (0-12)
  String horario = rct.getAmPm(true);  //devuelve un String representado en AM PM
  int hora_24 = rct.getHour(true);   //devuelve un int representando la hora (0-24)
  int dia = rct.getDay();   //devuelve un entero representando el dia (1-31)
  int mes = rct.getMonth();  //devuelve un entero representando el mes  (0-11)
  int year = rct.getYear();   //devuelve un entero representando el año

  
  Serial.println("Fecha y Hora actual");
    Serial.println(now);

     //Serial.println(millis());
     delay(10000);
     
    /*
    Serial.println("Forma 2:");
    Serial.print(dia);
    Serial.print("/");
    Serial.print(mes);
    Serial.print("/");
    Serial.print(year);
    Serial.print(" ");
    Serial.print(hora_24);
    Serial.print(":");
    Serial.print(minutos);
    Serial.print(":");
    Serial.println(segundos);
    Serial.println("");
    */   

  }

/////funcion de incremento/////////////
void onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  continterrupcion++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void TaskBlink(void *pvParameters) {
    while(1){
    if (continterrupcion > 0) {
 
    portENTER_CRITICAL(&timerMux);
    continterrupcion--;
    portEXIT_CRITICAL(&timerMux);
 
    totalcontinterrupcion++;
 
    Serial.print("Ocurrio interrupcion. Total number: ");
    Serial.println(totalcontinterrupcion);}
    
     if(totalcontinterrupcion == 6){
     Serial.println(yea);}
     if (totalcontinterrupcion == 12)
     {Serial.println("////////////////////");
      }
      if(totalcontinterrupcion == 24)
      {
        Serial.println(totalcontinterrupcion);
        }
         if(totalcontinterrupcion == 30){
     Serial.println(seg + 2);}
          
    delay(2000);
    }
}
