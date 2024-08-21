#include "BluetoothSerial.h"

//denota o período de amostragem: 1/Ts (em microssegundos, ou medida  
//de tempo referente também à variável tempo_atual na função loop());
//fs = 1/Ts = 5kHz, Ts = 200 us;
const unsigned long Ts = 200;

//frequência de corte superior do filtro EWMA
const unsigned long fc = 500;

//número de médias;
//p/ cálculo da média móvel:
//Nm = fs/2*fc = 1/Ts/2*fc = 1/2*Ts*fc
//Nm é definido pela freq. de corte superior do filtro EWMA e a frequência de amostragem utilizada (5kHz);
int Nm = int((1/(0.000002*fc*Ts)));

//variável referente à referência de tensão do circuito:
//a referência do circuito (no código) vai ser o valor que a placa apresentar
//enquanto nenhuma medição/movimento é realizada(o) (modo "idle");
float ref = 2000;

float invNm = 1/float(Nm);

BluetoothSerial SerialBT;

//vetor de structs contendo pinos e medidas de todas as placas
typedef struct{
  int pino;
  float readVALUE;
  float mmex;
} Medidas;

//vetor de structs placa é inicializado com tamanho 6 (para 6 placas)
Medidas placa[6];

//funçao de inicializaçao dos pinos do microcontrolador, declaraçao das portas gpio a serem usadas
void initPINS(){
  placa[0].pino = 32;
  placa[1].pino = 33;
  placa[2].pino = 34;
  placa[3].pino = 35;
  placa[4].pino = 36;
  placa[5].pino = 39;
  
  for(int i = 0; i < 6; i++){
    pinMode(placa[i].pino, INPUT);
  }

  pinMode(25, OUTPUT);
  //gera a tensão de referência no pino GPIO25
  dacWrite(25, 128);
}

float medidas_tx[6];

void read_e_tx(){
  for(int i = 0; i < 6; i++){
    //retificação do sinal
    placa[i].readVALUE = float(analogRead(placa[i].pino));
    placa[i].readVALUE = abs(placa[i].readVALUE - ref);

    //aplicação da média movel exponencial
    placa[i].mmex = (placa[i].readVALUE*invNm) + (placa[i].mmex*(1-invNm));
    
    medidas_tx[i] = placa[i].mmex;
  }
  
  SerialBT.println(medidas_tx);
}

void setup(){
  initPINS();

  Serial.begin(115200);
  
  //nome do ESP32 para parear;
  SerialBT.begin("esp-protese");
}

void loop(){
  //salva o tempo antes da leitura das portas (em us);
  unsigned long tempo_atual = micros();

  read_e_tx();

  //compara o tempo que o ESP demorou para fazer a leitura com o período de amostragem;
  while(micros() - tempo_atual < Ts){
  
  //espera o tempo do período de amostragem passar para iniciar outro processo de leitura.
  }
}
