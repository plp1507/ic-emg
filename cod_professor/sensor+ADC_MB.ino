//UFPR-DELT-TE149
//Prof. Marlio Bonfim
//Programa em linguagem "Wiring - Energia" para o MSP430G2553 que executa as seguintes funções:
//leitura analógica de um sinal no pino 5 (sensor de temperatura LM61)
//escreve na porta serial os valores: A/D, tensão lida, temperatura, tempo de execução
//O tempo de amostragem é definido pela variável Ts em ms
//Obs1: esta rotina também pode ser usada com o Arduino
//Obs2: esta rotina deve ser usada em conjunto com um programa de comunicação com a interface serial
//      Os valores lidos são mostrados na tela do computador através desta interface serial

int Ts = 1000;  //tempo de amostragem em ms
int AD = 0;    //variável da conversão A/D
float Vm = 1.8;   //variável tensão do A/D
float Temp = 25;  //variável Temperatura em graus Celcius
unsigned long tempo = 0; //tempo de contagem em ms
unsigned long texe; //tempo de execução da rotina em ms

void setup()          //define parâmetros gerais
{
   analogReference(INTERNAL1V5);//define tensao de referencia interna para o AD de 1,5 Volts
                          //Obs: para o Arduino Uno ou equivalente usar:  (INTERNAL1V1)
                          //Caso este comando nao seja utilizado, a tensao de referencia sera a tensao de alimentacao
   delay(1000);         //aguarda tensao de referncia estabilizar
   Serial.begin(9600);  //inicia interface serial com velocidade de 9600 bps
   pinMode(2, OUTPUT);//define pino 2 (LED vermelho) como saida
   pinMode(5, INPUT); //define pino 5 como entrada (A/D)
 
}

void loop() {         //rotina principal em loop infinito
tempo = millis();      //armazena o valor do tempo na variável tempo (em ms)
  digitalWrite(2, 1); //acende LED vermelho
  AD = analogRead(5); //efetua amostragem do conversor A/D no pino 5

  Vm = 1.5*AD/1024;  //calcula o valor da tensão para Vfs=1.5 V

  Temp = 100 *(Vm -0.6); //equação de calibraço do sensor LM61
 
  Serial.print("AD:");//envia o valor da conversão A/D
  Serial.print(AD);
  Serial.print("\t V:");//envia o valor da tensão com 3 casas decimais
  Serial.print(Vm, 2);
  Serial.print("\t T:");//envia o valor da Temperatura com 3 casas decimais
  Serial.print(Temp, 2);
  Serial.print("\t tex(ms):"); //envia o valor do tempo de execução da rotina
  Serial.println(millis()-tempo);
  
  digitalWrite(2, 0); //apaga LED vermelho  
  
while ((tempo+Ts) > millis()){}   //espera o tempo necessário para completar o tempo de amostragem Ts

}
