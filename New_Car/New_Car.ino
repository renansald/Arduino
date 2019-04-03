#define BLYNK_PRINT Serial
#define maxSpeed 1200
#define minSpeed 590

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

const char* auth = "e8d73cc50bc445f499e604d09bf88a2d";
const char* ssid = "Saldanha";
const char* password = "3NGdohawa11";
  int x = 512;
  int y = 512;
  int alerta;
  int farol;

void setup() {
  //pinMode(led,OUTPUT);
  pinMode(D1,OUTPUT);//Direito
  pinMode(D2,OUTPUT);//Esquerdo
  pinMode(D3,OUTPUT);//Direito
  pinMode(D4,OUTPUT); //Esquerdo
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);
}

void Alerta(){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
    delay(100);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    delay(100);
}

void Farol(int faroll){
  if(faroll == 1){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  }
  else{
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }
}

BLYNK_WRITE(V0){
x = param[0].asInt();
y = param[1].asInt();
}

BLYNK_WRITE(V1){
  alerta = param.asInt();
}

BLYNK_WRITE(V2){
  farol = param.asInt();
}

void loop() {
  Blynk.run();
  Serial.println(x);
  Serial.println(y);
  if(alerta == 1){
   Alerta(); 
  }
  if((alerta != 1)){
    Farol(farol);
  }
  if((y > 524) && (x > 400) && (x < 600)){
    Serial.println("frente");
    analogWrite(D1, map(y, 525, 1023, minSpeed, maxSpeed));
    digitalWrite(D3, HIGH);
    analogWrite(D2, map(y, 525, 1023, minSpeed, maxSpeed));
    digitalWrite(D4, HIGH);
  }
  else if((y < 500) && (x > 400) && (x < 600)){
    Serial.println("tras");
    int aux = y * (-1);
    analogWrite(D1, map(y, -499, 0, minSpeed, maxSpeed));
    digitalWrite(D3, LOW);
    analogWrite(D2, map(y, -499, 0, minSpeed, maxSpeed));
    digitalWrite(D4, LOW);
  }
  else if((x > 600) && (y > 524)){
    Serial.println("df");
    analogWrite(D2, map(y, 525, 1023, minSpeed, maxSpeed));
    digitalWrite(D4, HIGH);
    int aux = x*(-1);
    analogWrite(D1, map(aux,-1023, -601, minSpeed, (maxSpeed/1.333333)));
    digitalWrite(D3, HIGH);
  }
  else if((x > 600) && (y < 500)){
    Serial.println("Dt");
    int aux1 = y *(-1);
    analogWrite(D2, map(aux1, -499, 0, minSpeed, maxSpeed));
    digitalWrite(D4, LOW);
    int aux = x*(-1);
    analogWrite(D1, map(aux,-1023, -601, minSpeed, (maxSpeed/1.333333)));
    digitalWrite(D3, LOW);
  }
  else if((x < 400) && (y > 524)){
    Serial.println("EF");
    analogWrite(D1, map(y, 525, 1023, minSpeed, maxSpeed));
    digitalWrite(D3, HIGH);
    analogWrite(D2, map(x, 0, 400, minSpeed, (maxSpeed/1.333333)));
    digitalWrite(D4, HIGH);
  }
  else if((x < 400) && (y < 500)){
    Serial.println("Etras");
    int aux = y * (-1);
    analogWrite(D1, map(aux, -499, 0, minSpeed, maxSpeed));
    digitalWrite(D3, LOW);
    analogWrite(D2, map(x, 0, 400, minSpeed, (maxSpeed/1.333333)));
    digitalWrite(D4, LOW);
  }
  else{
    Serial.println("P");
    analogWrite(D1, 0);
    analogWrite(D3, LOW);
    digitalWrite(D2, 0);
    digitalWrite(D4, LOW);
  }
}
