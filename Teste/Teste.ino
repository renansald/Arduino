#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <Stepper.h>

//const int stepsPerRevolution = 64;
char auth[] = "e8d73cc50bc445f499e604d09bf88a2d";
char ssid[] = "SN";
char pass[] = "cunhacoelho30";
//Stepper myStepper(stepsPerRevolution,D1,D3,D2,D4);

WidgetTerminal terminal(V0);
int aux;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  //myStepper.setSpeed(60);
  pinMode(A0, OUTPUT);
  Serial.println("Informe o angulo de rotacao:");
}

BLYNK_WRITE(V0){
    analogWrite(A0, param.asInt());
    }
    
void loop() {
  Blynk.run();
  analogWrite(A0, aux);
  Serial.println(aux);
  Serial.println(A0);
  delay(5000);
  aux = 0;
  //aux = ((aux * 64)/360) * 32;
  //Serial.print(aux);
  //myStepper.step(aux);
}
