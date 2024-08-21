
//UFPR-DELT-TE331
//Prof. Marlio Bonfim
//Programa em linguagem "Wiring - Arduino" para o ATMEGA328/168 que executa as seguintes funções:
//leitura analógica de um sinal no pino A0 (sensor de temperatura NTC)
//Cálculo da tensão, resistência e temperatura do NTC (de 0 a 40 ºC)
//escreve na porta serial os valores: A/D, tensão lida, resistẽncia e temperatura
//O tempo de amostragem (intervalo de leitura) é definido pela função 'delay()' em ms
// 
//Os valores lidos são mostrados na tela do computador através desta interface serial (monitor serial do do Arduino)



int AD = 0;    //variável da conversão A/D
float B = 3620;   //constante do NTC
float Ro = 10000;   //resistência do NTC a 25ºC
float Rlin = 8870;  //resistor de linearização para Tc=20ºC (temperatura central)
float Rntc;
float Vo;   //tensão de saída do NTC, lida pelo conversor A/D
float Temp;  //variável Temperatura em graus Celcius


void setup()          //define parâmetros gerais
{
   
   Serial.begin(115200);  //inicia interface serial com velocidade de 115200 bps

   pinMode(A0, INPUT); //define pino A0 como entrada (A/D), para leitura do NTC

}

void loop() {         //rotina principal em loop infinito

  AD = analogRead(A0); //efetua amostragem do conversor A/D no pino 5

  Vo = 5*float(AD)/1024;  //calcula o valor da tensão para Vref=5 V
  Rntc = Vo * Rlin /(5 - Vo);
  Temp = B/(log(Rntc/Ro)+B/298)-273; //calcula o valor da temperatura pela equação do sensor LM61
 
  Serial.print("AD:");//envia o valor da conversão A/D
  Serial.print(AD);
  Serial.print("\t Vo:");//envia o valor da tensão com 2 casas decimais
  Serial.print(Vo,3);
  Serial.print("\t Rntc:");//envia o valor da tensão com 2 casas decimais
  Serial.print(Rntc,0);
  Serial.print("\t T:");//envia o valor da Temperatura com 2 casas decimais
  Serial.println(Temp, 1);
  
delay(500); //espera tempo de 0,5 segundos para efetuar a nova aquisição
}
