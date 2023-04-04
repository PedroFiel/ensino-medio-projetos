 #include <Servo.h>     // Adiciona a biblitoeca Servo
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
unsigned long tempo;
unsigned long int tempoAnterior = 0;
unsigned long int tempoAtual = 5000;

//Definindo os pinos
#define trigPin A0     //Pino TRIG do sensor no pino analógico A0
#define echoPin A1     //Pino ECHO do sensor no pino analógico A1
// motor um            // Ligação dos pinos da Ponte H L298N
#define enA  11        //pino enA na porta digital 10
#define in1  10         //pino in1 na porta digital 9
#define in2  9         //pino in2 na porta digital 8
// motor dois          // Ligação dos pinos da Ponte H L298N
#define enB  3         //pino enB na porta digital 5
#define in3  6        //pino in3 na porta digital 7
#define in4  5         //pino in4 na porta digital 6


#define mapa   (0) 
#define fogo   (1) 

Servo servoSensor;       // Crie um objeto Servo para controlar o Servo.
VR myVR(A5,2);    // 2:RX 3:TX, you can choose your favourite pins.

const int m = 20;
const int n = 20;
int linha=0, coluna=0, matriz[m][n] = {0};
char sentido='d'; //direita

uint8_t records[7]; // save record
uint8_t buf[64];


//função para procurar obtasculo a todo o tempo
int Procurar (void) { 
  float duracao = 0.0;              // variavael para quartar a duração do retorno do som
  float CM = 0.0;                   // variavael para quartar a distancia
  digitalWrite(trigPin, LOW);       //não envia som
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);      //envia som
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);       //não envia o som e espera o retorno do som enviado
  duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
  CM = (duracao / 58.8);            //Calcula a distância em centimetros
  Serial.print("Distancia em CM: ");
  Serial.println(CM);               //Imprimi no monitor serial a distancia
  return CM;                        // Return to CM.
}

//Variaveis
int DistanciaDireita, DistanciaEsquerda;  // variavel de Distâncias de ambos os lados
float Distancia = 0.00;                   // variavel para guardar a distancia

//Velocidades dos motores (você pode calibrar cada motor colocando os valores de 0 a 254)
int velocidadeMotorUm = 60;
int velocidadeMotorDois = 60
;

// Função que é executado na inicialização do Arduino
void setup() {
  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados
  
  //Define o servo na porta 13
  servoSensor.attach(13); 
  
  // Definir todos os pinos de controle do motor como saídas
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
       
  //Configuraçõs do sensor ultrassonico
  pinMode(trigPin, OUTPUT);     //define o pino TRIG como saída
  pinMode(echoPin, INPUT);      //define o pino ECHO como entrada


  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
      
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)mapa) >= 0){
    Serial.println("mapa loaded");
  }
  
  if(myVR.load((uint8_t)fogo) >= 0){
    Serial.println("fogo loaded");
  }
}


// Função principal do Arduino
void loop() {
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (100);                                      // Aguarda 100 milesugodos
  Distancia = Procurar ();                          // Medindo a Distancia em CM.
  if (Distancia < 30) {                             // Se há obstáculo encontrado a menos de 10cm.
   direcao ();                                      // Se Frente estiver bloqueado, mude de direção
  }
  else if (Distancia >= 30)  {                      // Se o obstáculo for encontrado entre a mais de 10cm 
    Frente ();                                      // Robô se move para a direção da Frente.
  }
  //Seu Robô http://SeuRobo.com.br/

  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case fogo:
        /** turn on LED */
        Parar();
        delay(5000);
        break;
      case mapa:
        /** turn off LED*/
        Mapa();
        delay(5000);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }


///////////////////////////////////

 

 
}
     

// Função para pegar as distancias de cada direção
void direcao () {        
  Parar ();                                         // O robô Para
  ParaTras ();                                      // O robô vai para tras
  Parar ();                                         // O robô Para
  servoSensor.write (180);                          // Gira o Servo com o sensor a 180 graus
  delay (1000);              
  DistanciaEsquerda = Procurar ();                  // Defina a Distancia da Esquerda 
  delay (500);               
  servoSensor.write (0);                            // Gira o Servo com o sensor a 0 graus
  delay (500);               
  DistanciaDireita = Procurar ();                   // Defina a Distancia da Direita
  delay (500);               
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (500);              
  CompareDistance ();                               // Encontre a distância mais longa.
}

// Função para calcular qual a distancia é melhor para o robô ir
void CompareDistance () {                   
  if (DistanciaDireita > DistanciaEsquerda) {       // Se a direita está menos obstruída.
    Vireadireita ();                                // O robô vai virar a direita 
    if (sentido=='d')
      sentido='b'; //vai para baixo na matriz
    else if(sentido=='b')
      sentido='e'; //vai para a esquerda na matriz
    else if(sentido=='e')
      sentido='c'; //vai para cima na matriz
    else if(sentido=='c')
      sentido='d'; //vai para a direita na matriz
      
  }
  else if (DistanciaEsquerda > DistanciaDireita) {  // Se Esquerda estiver menos obstruída.
    VireaEsquerda ();                               // Robô Vire na direção esquerda.
    if (sentido=='d')
      sentido='c'; //vai para cima na matriz
    else if(sentido=='b')
      sentido='d'; //vai para a direita na matriz
    else if(sentido=='e')
      sentido='b'; //vai para baixo na matriz
    else if(sentido=='c')
      sentido='e'; //vai para a esquerda na matriz    
  }
  else {                                            // Se ambos estiverem igualmente obstruídos.
    Retorne ();                                     // Robô Vire-se.
  }
  
}


void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void Mapa()
{
  for (linha = 0; linha < m; linha++) {
    for (coluna = 0; coluna < n; coluna++) {
      Serial.print(matriz[linha][coluna]);
    }
    Serial.print("\n");
  }
}

// Função para fazer o carro parar
void Parar()
{
  Serial.println("Robô: Parar ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(100);  //aguarda um tempo
}

// Função para fazer o robô andar para frente
void Frente()
{
  Serial.println("Robô: Frente ");
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
  matriz[linha][coluna] = 1;

  

  
  //adicionar o controle de tempo usando a função milis()
  
  if(sentido=='d' && coluna<n && (millis() - tempoAnterior) > 5000 ){ //direita na matriz
    coluna++;
    matriz[linha][coluna] = 1;
    Serial.print(matriz[linha][coluna]);
    tempoAnterior = millis();
  }else if(sentido=='e' && coluna>0 && (millis() - tempoAnterior) > 5000  ){ //esquerda na matriz
    coluna--;
    matriz[linha][coluna] = 1;
    Serial.print(matriz[linha][coluna]);
    tempoAnterior = millis();
  }else if(sentido=='b' && linha<m && (millis() - tempoAnterior) > 5000  ){ //desce (baixo) pela matriz
    linha++;
    matriz[linha][coluna] = 1;
    Serial.print(matriz[linha][coluna]);
    tempoAnterior = millis();
  }else if(sentido=='c' && linha>0 && (millis() - tempoAnterior) > 5000  ){ //sobe (cima) pela matriz
    linha--; 
    matriz[linha][coluna] = 1;
    Serial.print(matriz[linha][coluna]);
    tempoAnterior = millis();   
  }




    
}

// Função que faz o robô andar para trás e emite som quando ele dá ré
void ParaTras()
{
  Serial.println("Robô: Ré ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois       
                    
}

// Função que faz o robô virar à direita, https://SeuRobo.com.br/
void Vireadireita()
{
  Serial.println("Robô: Direita ");
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW );
  digitalWrite(in4, HIGH);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o robô virar à esquerda
void VireaEsquerda()
{
  Serial.println("Robô: Esquerda ");                
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

//Função para o robô virar para o outro lado
void Retorne () {    
  Serial.println("Robô: Girar ");      
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay (700);                                      //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}
