#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define REF      1820
#define N_AMOSTRAS 10

int contador = 0;

float Nm = 1;
float alfa = (Nm-1)/Nm;

typedef struct {
  int pino;
  float medida;
  float ret;
  float mmex;
} placa;

placa eletrodo;

void setup() {
  eletrodo.pino = 32;
  eletrodo.mmex = 0;
  
  pinMode(32, INPUT);
  pinMode(25, OUTPUT);
  dacWrite(25, 128);
  Serial.begin(1000000);
  SerialBT.begin("esp_protese");
}

void loop() {
  contador++;
  eletrodo.medida = analogRead(eletrodo.pino);
  eletrodo.ret = eletrodo.medida - REF;
  eletrodo.ret = abs(eletrodo.ret);
  eletrodo.mmex = (alfa)*eletrodo.mmex + (1-alfa)*eletrodo.medida;

  if(contador == N_AMOSTRAS){
    SerialBT.print(eletrodo.medida);
    SerialBT.print(",");
    SerialBT.print(eletrodo.ret);
    SerialBT.print(",");
    SerialBT.println(eletrodo.mmex);
    contador = 0;
  }
}
