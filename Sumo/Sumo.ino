//Bibliotecas
#include <Ultrasonic.h> //Controla os ultrassonicos
#include <SharpIR.h> //Controla o Sharp

//Pinos de controle do L298N IN1 e IN2 controlam o motor Esquerdo e IN3 e IN 4 controlam o motor direito
#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13

//Pinos dos sensores de borda(A leitura deles correspondem a 1 para preto e zero para branco)
#define traseiraEsquerda 2
#define traseiraDireita 3
#define dianteiraEsquerda 4
#define dianteiraDireita 5

//Pinos sensores ultrassonicos
#define echoEsquerdo 7
#define triggerEsquerdo 6
#define echoDireito 9
#define triggerDireito 8

//Pino sharp
#define sharp A0
#define modelo 1080 //1080 PARA O MODELO 2Y0A21Y OU 20150 PARA O MODELO 2Y0A02Y

//Pino LED
#define led 1

//Variaveis globais
int ctrl;

//Inicializa bibliotecas
SharpIr sharpIR(sharp, modelo);
Ultrasonic ultrasonicoEsquerdo(triggerEsquerdo, echoEsquerdo);
Ultrasonic ultrasonicoDireito(triggerDireito, echoDireito);


void setup(){
//Define os pinos como saida
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 pinMode(traseiraEsquerda, INPUT);
 pinMode(traseiraDireita, INPUT);
 pinMode(dianteiraEsquerda, INPUT);
 pinMode(dianteiraDireita, INPUT);
 pinMode(led, OUTPUT)
 //Lógica para aguarda 5 segundos, regra obrigatória da robocore
 delay(1000);
 digitalWrite(led, HIGH);
 delay(1000);
 digitalWrite(led,LOW);
 delay(1000);
 digitalWrite(led, HIGH);
 delay(1000);
 digitalWrite(led, LOW);
 delay(1000);
 digitalWrite(led, HIGH);
}
  
void loop(){
  long timeUltrasonicoEsquerdo = ultrasonicoEsquerdo.timing();
  long timeUltrasonicoDireito = ultrasonicoDireito.timing();  
  //Desvio condiciona de busca e atque ao inimigo
  if((digitalRead(dianteiraEsquerda) == 0) || (digitalRead(dianteiraDireita) == 0)){
    MoveTras();
  }
  else if((digitalRead(traseiraEsquerda) == 0) || (digitalRead(traseiraDireita) == 0)){
    MoveFrente();
  }
  else if((sharpIR.distance() < 70) && (digitalRead(dianteiraEsquerda) == 1) && (digitalRead(dianteiraDireita) == 1)){
    MoveFrente();
    ctrl = 0; 
  }
  else if((ultrasonicoEsquerdo.convert(timeUltrasonicoEsquerdo, Ultrasonic::CM) < 70)){
    GirarAEsquerda();
    ctrl = 1;
  }
  else if((ultrasonicoDireito.convert(timeUltrasonicoDireito, Ultrasonic::CM) < 70)){
    GirarADireita();
    ctrl = 0;
  }
  else{
    if(ctrl == 1){
      GirarAEsquerda();
    }
    else{
      GirarADireita()
    }
  }
}

// tem que fazer os testes com os motores para configurar isso certo
void MoveFrente(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void MoveTras(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void GirarAEsquerda(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void GirarADireita(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
