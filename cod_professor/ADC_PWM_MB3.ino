//UFPR-DELT-TE331
//Prof. Marlio Bonfim
// Programa em linguagem "Wiring - C++" para o Arduino - Atmega328 que executa as seguintes funções:
// leitura analógica de um sinal na porta A0
// saída PWM na porta D3
// altera o PWM através de valores enviados pelo teclado (comunicação pela interface serial)
// O tempo de amostragem é definido pela variável Ts em ms
// Os valores lidos são mostrados na tela do computador através da interface serial

unsigned int Ts = 500;  //tempo de amostragem em ms
int AD = 0;    //variável da conversão A/D
float B = 3620;   //constante do NTC
float Ro = 10000;   //resistência do NTC a 25ºC
float Rlin = 8870;  //resistor de linearização
float Rntc;
float Vo;   //tensão de saída do NTC, lida pelo conversor A/D
float Temp;  //variável Temperatura em graus Celcius
byte PWM=0;  //variável PWM (0 a 255)
unsigned long tempo = 0; //tempo de contagem em ms



void setup()          //define parâmetros gerais
{
   Serial.begin(115200);  //inicia interface serial com velocidade de 9600 bps
   Serial.setTimeout(50); //limita o tempo de espera de caracteres de entrada da serial

   pinMode(3, OUTPUT);//define pino 3 como saida PWM para Gate do MOSFET que irá acionar uma carga
   analogWrite (3, 0); //escreve o valor 0 na saída PWM
   pinMode(A0, INPUT); //define pino 6 como entrada de tensão 

}

void loop() {         //rotina principal em loop infinito
tempo = millis();      //armazena o valor do tempo na variável "tempo" (em ms)

//Leitura do A/D 

  AD = analogRead(A0); //efetua amostragem do conversor A/D no pino A0

  Vo = 5*float(AD)/1024;  //calcula o valor da tensão para Vref=5 V
  Rntc = Vo * Rlin /(5 - Vo);
  Temp = B/(log(Rntc/Ro)+B/298)-273; //calcula o valor da temperatura pela equação do NTC
 
//  Serial.print("AD:");//envia o valor da conversão A/D
//  Serial.print(AD);
//  Serial.print("\t Vo:");//envia o valor da tensão com 2 casas decimais
//  Serial.print(Vo,3);
//  Serial.print("\t Rntc:");//envia o valor da tensão com 2 casas decimais
//  Serial.print(Rntc,0);
//  Serial.print("\t Tntc:");
   Serial.print(PWM); Serial.print(" ");//envia o valor do PWM
//  Serial.print("\t PWM:");
   Serial.println(Temp, 1);//envia o valor da Temperatura com 1 casa decimal
    
  if (Serial.available() > 0) {  //verifica se há dados na serial 
  PWM = Serial.parseInt(); //lê caracteres ASCII da serial e converte em um número inteiro
  PWM = constrain(PWM, 0, 255);  //limita os valores da variável PWM entre 0 a 255
  analogWrite (3, PWM);  //atribui o valor do PWM à porta de saída 3
  }
 
while ((tempo+Ts) > millis()){}   //espera o tempo necessário para completar o tempo de amostragem Ts
}
