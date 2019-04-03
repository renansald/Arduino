#include <Stepper.h> 
 
const int stepsPerRevolution = 64;
int aux; 
  
//Inicializa a biblioteca utilizando as portas de 8 a 11 para 
//ligacao ao motor 
Stepper myStepper(stepsPerRevolution, 8,10,9,11); 
void setup() 
{ 
 //Determina a velocidade inicial do motor 
 myStepper.setSpeed(500);
 Serial.begin(9600);
 pinMode(2, INPUT);
} 
  
void loop() 
{ 
  Serial.println("Informe o angulo desejado:");
  aux = digitalRead(1);
  Serial.println(aux);
  aux = ((aux * 64) / 360) * 32;
  Serial.println(aux);
  myStepper.step(aux);
 //Gira o motor no sentido horario a 90 graus
 //for (int i = 0; i<=0; i++)
 //{
 //myStepper.step(aux); 
 /*for(int i = 0; i <= 360; i = i + 90){
  aux = ((i * 64) / 360) * 32;
  myStepper.setSpeed(i);
  myStepper.step(aux);
 }
 delay(2000); 
 //}*/
  
 /*Gira o motor no sentido anti-horario a 120 graus
 for (int i = 0; i<=2; i++)
 {
 myStepper.step(682); 
 delay(2000);
 }
 
 //Gira o motor no sentido horario, aumentando a
 //velocidade gradativamente
 for (int i = 10; i<=60; i=i+10)
 {
 myStepper.setSpeed(i);
 myStepper.step(40*i);
 }
 delay(2000);*/ 
}
