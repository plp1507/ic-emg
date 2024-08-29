//UFPR-DELT-TE149
//Prof. Marlio Bonfim
//Programa em linguagem "Wiring - Energia" para o MSP430G2553 para Controle Digital PI que executa as seguintes funções:
//leitura analógica de um sinal no pino 15
//efetua a média de 16 amostras com ganho de resolução de 2 bits
//recebe o valor do SetP (set-point) pela serial com 3 algarismos (de 000 a 255)
//compara o valor de SetP com PV (leitura do AD convertida para tensao) e gera a variavel erro
//calcula a integral de erro caso o erro seja inferior a 10% do SetP
//calcula a derivada do PV fazendo a subtraço entre os valores anterior e atual
//calcula o valor do PWM usando a equaço do controlador PID
//aciona os pinos 13 e 14 com o valor do PWM
//escreve na serial os valores: SetP, PV, PWM, tempo de execução
//Obs: o botão S2 é usado para habilitar ou desabilitar a escrita na serial (estado inicial: desabilitado)
//O tempo de amostragem é definido pela variável Ts em ms ** Ts DEVE SER MENOR QUE tau/10 **
//Obs: esta rotina também pode ser usada no Arduino

//#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>                    // Biblioteca DHT Sensor Adafruit 

// Constants setup
#define DHTTYPE DHT22
#define DHTPIN 2
#define PWMPIN 11

// Global variables
uint8_t dutycycle = 0;
uint8_t m_amostra = 16;
int Ts = 2000; //tempo de amostragem em ms ** Ts DEVE SER MAIOR QUE O TEMPO DE EXECUCAO DA ROTINA **
float PWM = 0; // variável do PWM
unsigned int AD = 0; //variável da conversão A/D
unsigned int ADm = 512; //variável valor médio do A/D
float PV = 1.8; //variável do processo (tensão do A/D)
float PV_ant = 1.8; //variável do processo (tensão do A/D) na amostragem anterior
int n = 0; //variável de contagem n
unsigned long tempo = 0; //tempo de contagem em ms
unsigned long texe; //tempo de execução da rotina em ms
char inBuffer[3]; //vetor de dados de entrada da serial
float SetP = 80;
//unsigned int SetPint = 200; não sei pq o professor botou isso aqui
float k = -0.055074293222838; //ganho do processo
float tau = 51454.5542825; //constante de tempo do processo em ms
float Kp, Ki;
float erro, int_erro = 0;

// Objects
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() //define parâmetros gerais
{
 Serial.begin(9600); //interface serial com velocidade de 9600 bps
 pinMode(PWMPIN, OUTPUT);
 dht.begin();   
 Kp = -91.951291070114533; //calculo da constante Kp
 Ki = -4.525078091; //calculo da constante Ki
}

void loop() { //rotina principal em loop infinito
 tempo = millis(); //armazena o valor do tempo na variável tempo (em ms)
 PV = read_umd(); //efetua amostragem do conversor A/D com a função read_umd e faz a soma cumulativa
 //PV = 3.56*AD/1024; //calcula o valor da tensão para Vfs=3.56 V
 //AD = 0; //inicializa a variável AD

//Controlador PI

 erro = SetP - PV; //calcula o erro entre SetP e PV
 if (erro> (SetP*0.1) || erro < (-SetP*0.1)) //reset do termo integrativo (anti-windup)
 {
  int_erro = 0; //zera o erro integrativo se o erro for maior que 10% de SetP (em modulo). Isto reduz o overshoot
 }
 else
 {
  int_erro += erro; //habilita o erro integrativo se o erro for menor que 10% de SetP (em modulo)
 }

 PWM = erro * Kp + int_erro * Ki;

 if (PWM > 255) {
  PWM = 255; //verifica os limites possveis do PWM
  }
 if (PWM < 30) {
  PWM= 0;
  }
 analogWrite(PWMPIN, PWM); //escreve o valor do PWM no pino PWMPIN
 Serial.print(SetP, 3);
 Serial.print("\t");
 Serial.print(PV, 3);
 Serial.print("\t");
 Serial.println(PWM);

if (Serial.available() > 0) { //analisa se há dados na serial enquanto espera o final do tempo de amostragem
Serial.readBytes(inBuffer, 3); //lê o dado da serial em 3 caracteres ASCII
SetP = atoi(inBuffer); //converte o valor ASCII lido em um número inteiro
 if (SetP > 80) {
  SetP = 80; //verifica os limites superior e inferior de SetP
  }
 if (SetP < 20) {
  SetP=20;
  }
//SetP = SetP/100; //ajusta o valor de SetP para a escala adequada (0.1 < SetP <3.5)
}
}

uint8_t read_umd(){
  delay(Ts);                         // atraso entre as medições
  sensors_event_t event;                        // inicializa o evento
  dht.humidity().getEvent(&event);              // faz a leitura de umidade
  if (isnan(event.relative_humidity))           // se algum erro na leitura
  {
    Serial.println("!!Erro na leitura da Umidade!!");
    return 0;
  }
  else
  {
    float umd = event.relative_humidity;
    return umd;
  }
}
