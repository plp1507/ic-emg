#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define Nm          400
#define REF        1820
#define N_AMOSTRAS    100

float alfa = (Nm - 1)/Nm;

int contador = 0;

typedef struct{
  int pino;
  float medida;
  float mmex = 0;
  float absol;
} Eletrodo;

Eletrodo placa[6];

void initPINS(){
  for(int i = 0; i < 5; i++){
    placa[i].pino = 32 + i;
    pinMode(placa[i].pino, INPUT);
  }
  placa[5].pino = 39; pinMode(placa[5].pino, INPUT);

  pinMode(25, OUTPUT);
  dacWrite(25, 130);
}

void setup() {
  initPINS();
  SerialBT.begin("esp_protese");
}

void loop() {
  contador++;

  for(int i = 0; i < 6; i++){
    placa[i].medida = analogRead(placa[i].pino)-REF;
    placa[i].absol = abs(placa[i].medida);
    placa[i].mmex = (placa[i].absol*(1-alfa)) + (placa[i].mmex*alfa);
  }

  if(contador == N_AMOSTRAS){
    for(int i = 0; i < 5; i++){
      SerialBT.print(placa[i].mmex);
      SerialBT.print(",");
    }
    SerialBT.println(placa[5].mmex);
    contador = 0;
  }
}
