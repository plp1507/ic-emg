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
int Ts = 100; //tempo de amostragem em ms ** Ts DEVE SER MAIOR QUE O TEMPO DE EXECUCAO DA ROTINA **
float PWM = 0; // variável do PWM
unsigned int AD = 0; //variável da conversão A/D
unsigned int ADm = 512; //variável valor médio do A/D
float PV = 1.8; //variável do processo (tensão do A/D)
float PV_ant = 1.8; //variável do processo (tensão do A/D) na amostragem anterior
int n = 0; //variável de contagem n
unsigned long tempo = 0; //tempo de contagem em ms
unsigned long texe; //tempo de execução da rotina em ms
char inBuffer[3]; //vetor de dados de entrada da serial
float SetP = 2.0;
//unsigned int SetPint = 200;
float k = 3.56/255; //ganho do processo
float tau = 1000; //constante de tempo do processo em ms
float Kp, Ki;
float erro, int_erro = 0;
void setup() //define parâmetros gerais
{
 Serial.begin(9600); //interface serial com velocidade de 9600 bps
 pinMode(13, OUTPUT);//define pino 13 como saída
 pinMode(14, OUTPUT);//define pino 14 como saída
 pinMode(15, INPUT); //define pino 15 como entrada (A/D)
 pinMode(2, OUTPUT);//define pino 2 (LED vermelho) como saída
 digitalWrite(2, 0); //apaga o LED vermelho
 pinMode(5, INPUT_PULLUP); //define pino 5 como entrada com resistor de pull-up (botão S2)
 attachInterrupt(5, serial_on, FALLING); // ativa a interrupção quando o botão S2 é pressionado

 Kp = 20 / k; //calculo da constante Kp

 Ki = 2*Kp*Ts/tau; //calculo da constante Ki
}
void loop() { //rotina principal em loop infinito
tempo = millis(); //armazena o valor do tempo na variável tempo (em ms)
while (n < 16){ //executa a leitura até que o número de amostras seja < 16
AD += analogRead(15); //efetua amostragem do conversor A/D no pino 15 e faz a soma cumulativa
n +=1; //incrementa a variável n
}
 ADm = AD / 16; //calcula o valor médio das amostras
 PV = 3.56*ADm/1024; //calcula o valor da tensão para Vfs=3.56 V
 AD = 0; //inicializa as variáveis AD e n
 n = 0;
//Controlador PI

 erro = SetP - PV; //calcula o erro entre SetP e PV
 if (erro> (SetP*0.10) || erro < (-SetP*0.10)) //reset do termo integrativo (anti-windup)
 {
int_erro = 0; //zera o erro integrativo se o erro for maior que 10% de SetP (em modulo). Isto reduz o overshoot
 }
else
{
int_erro += erro; //habilita o erro integrativo se o erro for menor que 10% de SetP (em modulo)
 }

 PWM = erro * Kp + int_erro * Ki;

 if (PWM > 255) PWM = 255; //verifica os limites possveis do PWM
 if (PWM < 0) PWM= 0;
 analogWrite(14, PWM); //escreve o valor do PWM no pino 14
 analogWrite(13, (255-PWM)); //escreve o valor complementar do PWM no pino 13
 Serial.print(SetP, 3);
 Serial.print("\t");
 Serial.print(PV, 3);
 Serial.print("\t");
 Serial.println(PWM);
while ((tempo+Ts) > millis()){ //eSetPera o tempo necessário para completar o tempo de amostragem Ts
if (Serial.available() > 0) { //analisa se há dados na serial enquanto espera o final do tempo de amostragem
Serial.readBytes(inBuffer, 3); //lê o dado da serial em 3 caracteres ASCII
SetP = atoi(inBuffer); //converte o valor ASCII lido em um número inteiro
 if (SetP > 350) SetP = 350; //verifica os limites superior e inferior de SetP
 if (SetP < 10) SetP=10;
SetP = SetP/100; //ajusta o valor de SetP para a escala adequada (0.1 < SetP <3.5)
 }
 }
}
