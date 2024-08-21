//UFPR-DELT-TE331
//Prof. Marlio Bonfim
//Programa em linguagem "Wiring - Arduino" para o Atmega328 que executa as seguintes funções:
//leitura do sensor interno de temperatura
//filtragens digitais: sobre-amostragem e EWMA
//escreve na porta serial os valores: AD (dadocru), ADm (sobre-amostragem), EWMA
//o número de médias da filtragem é escolhido através de valores definidos via teclado, enviados ao Arduino pela serial
//Obs1: o valor das médias deve ser enviado sempre com 3 algarismos (ex:008 => para 8 médias). Número máx: 256
//Obs2: esta rotina deve ser usada em conjunto com um programa de comunicação com a interface serial
//      Os valores lidos são mostrados na tela do computador através desta interface serial

unsigned int AD = 0;
unsigned int m=16, n=0;
float ADm=0;  
float alfa=float(m-1)/m, EWMA=1;

char inBuffer[3]; //vetor de dados de entrada da serial


void setup() {
Serial.begin(115200);  //define a velocidade de comunicação serial em 115200
//Serial.print(alfa);
EWMA = ReadIntTemp();

}

void loop() {
  AD = 0; n=0;  //inicializa as variáveis
  
  while (n < m){  //realiza 'm' leituras do conversor AD
  AD += ReadIntTemp(); //efetua a leitura AD do sensor de temperatura interno
  n +=1;                //incrementa a variável n
}
  ADm = float(AD)/m;   //calcula a média por sobre-amostragem
   
  AD = ReadIntTemp();
  //float Temp = (AD - 324.31) / 1.22; //efetua a leitura AD do sensor de temperatura interno
  
  EWMA = EWMA*alfa + (1-alfa)*float(AD); //calcula a média móvel exponencial
  
  Serial.print((AD- 324.31) / 1.22);
  Serial.print(",");
  Serial.print(((ADm- 324.31) / 1.22)-2);
  Serial.print(",");
  Serial.println((EWMA- 324.31) / 1.22);

delay(100);
if (Serial.available() > 0) {  //analisa se há dados na serial 
Serial.readBytes(inBuffer, 3); //lê o dado da serial em 3 caracteres ASCII
m = constrain (atoi(inBuffer), 1, 256);     //converte o valor ASCII lido em um número inteiro, limitado de 1 a 256

alfa = float((m-1.0) / m);    //calcula a constante alfa do filtro EWMA
  }

}

int ReadIntTemp(){ //função para leitura do sensor de temperatura interno
  int ADTint;
  ADMUX = 0b11001000;   //select internal ref voltage 1.1 V and internal temperature sensor
  ADCSRA |= (1 << ADSC);            //  Start ADC conversion
  while (ADCSRA & (1 << ADSC));     // wait for the conversion to finish
  ADTint  += ADCL | (ADCH << 8);    //concatenate ADCL+ADCH bytes
  return ADTint;  
  
}
