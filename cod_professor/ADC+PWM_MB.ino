//UFPR-DELT-TE149
//Prof. Marlio Bonfim
//Programa em linguagem "Wiring - Energia" para o MSP430G2553 que executa as seguintes funções:
//leitura analógica de um sinal no pino 15
//efetua a média de 16 amostras com ganho de resolução de 2 bits
//recebe o valor do PWM pela serial com 3 algarismos (de 000 a 255)
//aciona os pinos 13 e 14 com o valor do PWM
//escreve na serial os valores: PWM, A/D, tensão lida, tempo de execução
//O tempo de amostragem é definido pela variável Ts em ms
//Obs1: esta rotina também pode ser usada no Arduino
//Obs2: esta rotina deve ser usada em conjunto com um programa de comunicação com a interface serial


int Ts = 500;  //tempo de amostragem em ms
int PWM = 0;   // variável do PWM
int AD = 0;    //variável da conversão A/D
int ADm = 512;   //variável valor médio do A/D
float Vm = 1.8;   //variável tensão do A/D
int n = 0;        //variável de contagem n
unsigned long time = 0; //tempo de contagem em ms
unsigned long texe; //tempo de execução da rotina em ms
char inBuffer[3]; //vetor de dados de entrada da serial

void setup()          //define parâmetros gerais
{
  Serial.begin(9600);  //interface serial com velocidade de 9600 bps
   pinMode(13, OUTPUT);//define pino 13 como saída
   pinMode(14, OUTPUT);//define pino 14 como saída
   pinMode(15, INPUT); //define pino 14 como entrada (A/D)
}

void loop() {         //rotina principal em loop infinito
time = millis();      //armazena o valor do tempo na variável time (em ms)

while (n < 16){      //executa a leitura até que o número de amostras seja < 16
AD += analogRead(15); //efetua amostragem do conversor A/D no pino 15 e faz a soma cumulativa
n +=1;                //incrementa a variável n
}
  ADm = AD / 4;       //calcula o valor médio das amostras e considera o ganho de resolução
  Vm = 3.6*ADm/4096;  //calcula o valor da tensão para Vfs=3.6 V
 
  AD = 0;             //inicializa as variáveis AD e n
  n = 0;
  Serial.print("PWM: "); //envia o valor do PWM pela interface serial
  Serial.print(PWM);
  Serial.print("\t ADm: ");//envia o valor da conversão A/D
  Serial.print(ADm);
  Serial.print("\t Vm: ");//envia o valor da tensão com 3 casas decimais
  Serial.print(Vm, 3);
  Serial.print("\t texe(ms): "); //envia o valor do tempo de execução da rotina
  Serial.println(millis()-time);
  
if (Serial.available() > 0) {  //analisa se há dados na serial 
Serial.readBytes(inBuffer, 3); //lê o dado da serial em 3 caracteres ASCII
PWM = atoi(inBuffer);          //converte o valor ASCII lido em um número inteiro
  }
  
analogWrite(14, PWM); //escreve o valor do PWM no pino 14
analogWrite(13, (255-PWM));  //escreve o valor complementar do PWM no pino 13

while ((time+Ts) > millis()){}  //espera o tempo necessário para completar o tempo de amostragem Ts
}
