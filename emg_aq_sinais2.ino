//https://www.manualdomaker.com/article/isr-interrupcoes-e-timer-com-esp32/
//https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/#google_vignette

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define REF        1820;
#define N_AMOSTRAS  100;
#define Nm           93;

const float alfa = (Nm - 1)/Nm;

int contador = 0;

typedef struct{
  int pino;
  float absol;
  float mmex = 0;
} Eletrodo;

Eletrodo placa[6];

void initPINS(){

  for(int i = 0; i < 5; i++){
    placa[i].pino = 32 + i;
    pinMode(placa[i].pino, INPUT);
  }
  placa[5].pino = 39;
  pinMode(placa[5].pino, INPUT);

  pinMode(25, OUTPUT);
  dacWrite(25, 130);
}

hw_timer_t *Timer0_Cfg = NULL;

void IRAM_ATTR Timer0_ISR(){
  
  contador++;

  for(int i = 0; i < 6; i++){
    placa[i].absol = analogRead(placa[i].pino) - REF;
    placa[i].absol = abs(placa[i].absol);
    placa[i].mmex = (1-alfa)*placa[i].absol + alfa*placa[i].mmex;
  }  
}

void setup() {
  Serial.begin(1000000);
  initPINS();

  //Tamostr = N_ticks*(escalar/80MHz). 80MHz = periodo do clock do ESP32
  //35 (escalar) e 1000 (ticks) -> frequencia de amostragem = 22.8kHz.
  Timer0_Cfg = timerBegin(0, 35, true);
  timerAttachInterrupt(Timer0_Cfg, &Timer0_ISR, true);
  timerAlarmWrite(Timer0_Cfg, 1000, true);
  timerAlarmEnable(Timer0_Cfg);

  SerialBT.begin("esp_protese");
}

void loop() {
  if(contador == N_AMOSTRAS){
    SerialBT.print(placa[0].mmex);
    SerialBT.print(",");
    SerialBT.print(placa[1].mmex);
    SerialBT.print(",");
    SerialBT.print(placa[2].mmex);
    SerialBT.print(",");
    SerialBT.print(placa[3].mmex);
    SerialBT.print(",");
    SerialBT.print(placa[4].mmex);
    SerialBT.print(",");
    SerialBT.println(placa[5].mmex);
    contador = 0;
  }
}
