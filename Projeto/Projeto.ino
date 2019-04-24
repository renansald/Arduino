/*Esquemas de ligação:
   Modulo RFID:
  - RST -> 9;
  - MISO -> 12;
  - MOSI -> 11;
  - SCK -> 13;
  - SDA -> 10;
  -Alimentação 3.3v;
   Modulo SIM808L
  - RX -> 5;
  - TX -> 6;
    Modulo obd2
   - MISO -> 12;
   - MOSI -> 11;
   - SCK -> 13;
   - SC -> 4;
  APN USUARIO E SENHA OPERADORAS
    TIM: "timbrasil.br", "tim", "tim"
    Vivo: "zap.vivo.com.br", "vivo", "vivo"
    Claro: "claro.com.br", "claro", "claro"
    OI: "gprs.oi.com.br", "oi", "oi"
*/
//Bibliotecas
#include <SPI.h>
#include <Lista.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include "inetGSM.h"
#include <CAN.h>
#include <OBD2.h>

//Declaração de variaveis pinos
#define SS_PIN 10 //Pino SS do modulo RFID
#define RST_PIN 9 //Pino reset do modulo RFID
#define RX_SIM808 5 // Pino para comunicação RX com modulo SIM808
#define TX_SIM808 6 // Pino para comunicação TX com modulo SIM808
#define DEBUG true
#define LED_GREEN 4
#define LED_RED 3
#define PID VEHICLE_SPEED

//Variaveis globais
String latitudeAux, longitudeAux;
char schoolBusId[16];
unsigned long timer;
int timerUpload;

//Declara pinos para a comuniçacao Serial
SoftwareSerial sim808(RX_SIM808, TX_SIM808);

//Incializando Objeto RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Definição da chave de acesso do RFID
MFRC522::MIFARE_Key key;

//Inicializa a lista
Lista listaEmbarcados;

//Inicializa objeto para conexao GPRS
InetGSM gprsConexao;

void setup() {
  Serial.begin(9600);   //Inicia a serial
  SPI.begin();      //Inicia  SPI bus
  mfrc522.PCD_Init();   //Inicia MFRC522
  sim808.begin(9600);
  while (true) { //Conectando a OBD2
    Serial.print(F("Attempting to connect to OBD2 CAN bus ... "));
    if (!OBD2.begin()) {
      Serial.println(F("failed!"));
      delay(1000);
    } else {
      Serial.println(F("success"));
      break;
    }

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    //Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
    while (!mfrc522.PICC_IsNewCardPresent()) {
      delay(100);
    }
    if (mfrc522.PICC_ReadCardSerial()) {
      IdSchoolBus();
    }
    timer = millis() * 1000;
    // timerUpload recebe o valo que tá no servidor de tempo de upload da coordenada do onibus e velociodade
    
  }

  void loop() {
    if (mfrc522.PICC_ReadCardSerial()) {
      modo_leitura();
    }
    if (((millis() * 1000) - timer) >= timerUpload) {
      float speedBus = SpeedBus(PID);
      //coordinates() Olhar com Jamisson
      //Upload para o servidor dados onibus;
      timer = millis() * 1000;
    }
    // timerUpload recebe o valor que tá no servidor de tempo de upload da coordenada do onibus e velociodade

  }

  void modo_leitura() {
    char id[15];
    char latitude[11];
    char longitude[11];
    char hora[9];

    /*Mostra UID na serial
      Serial.print(F("UID da tag : "));
      String conteudo = "";
      byte letra;
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.println();*/

    byte sector         = 2;
    byte blockAddr      = 8;
    byte trailerBlock   = 11;
    byte status;
    byte buffer[18];
    byte size = sizeof(buffer);

    //Obtem os dados do setor 2, bloco 8 = Telefone
    sector         = 2;
    blockAddr      = 8;
    trailerBlock   = 11;

    //Autenticacao usando chave A
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      digitalWrite(LED_RED, HIGH);
      delay(3000);
      digitalWrite(LED_RED, LOW);
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      digitalWrite(LED_RED, HIGH);
      delay(3000);
      digitalWrite(LED_RED, LOW);
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    for (byte i = 0; i < 13; i++) {
      if (char(buffer[i]) == '!') {
        id[i] = '\0';
        i = 16;
      }
      else {
        Serial.print(char(buffer[i]));
        id[i] = (char(buffer[i]));
      }
    }
    /*
      Serial.println(Id);
      Serial.println();
    */
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

    //Captar a logitude/latitude
    // coordinates() pegar com o Jamisson o codigo dele
    latitudeAux.toCharArray(latitude, 11);
    longitudeAux.toCharArray(longitude, 11);
    timeGPS.toCharArray(hora, 9);

    addBuffer(id, latitude, longitude, hora);
    digitalWrite(LED_GREEN, HIGH);
    delay(3000);
    digitalWrite(LED_GREEN, LOW);
  }

  void addBuffer(char id[], char latitude[], char longitude[], char hora[]) {
    listaEmbarcados.adiciona(id, latitude, longitude, hora);
  }
}

/*void sms(char nome[], char telefone[], char latitude[], char longitude[], int acao, char hora[]) {
  Serial.println("SMS");
  String destinatario = "AT+CMGS=\"";
  destinatario += telefone;
  destinatario += "\"";
  String msg = (F("O aluno(a) "));
  msg += nome;
  if (acao == 0) {
    msg += (F(" embarcou no transporte escolar na seguinte localizacao "));
  }
  else {
    msg += (F(" desembarcou do transporte escolar na seguinte localizacao "));
  }
  msg += (F("https://maps.google.com/maps?ll="));
  msg += latitude;
  msg += (F(","));
  msg += longitude;
  msg += (F(" as "));
  msg += hora;
  sim808.println("AT+CMGF=1\r"); //Ativa a função de envio de sms
  delay(100);
  sim808.println(destinatario); //Define o destinatario da mensagem;
  delay(100);
  sim808.println(msg);
  delay(100);
  sim808.println((char)26);
  delay(20000);
  sim808.flush();
  Serial.println("Enviado");
  Serial.println(destinatario);
  Serial.println(msg);
  }*/

IdSchoolBus() {
  byte sector         = 2;
  byte blockAddr      = 8;
  byte trailerBlock   = 11;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);

  //Obtem os dados do setor 2, bloco 8 = Telefone
  sector         = 2;
  blockAddr      = 8;
  trailerBlock   = 11;

  //Autenticacao usando chave A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    digitalWrite(LED_RED, HIGH);
    delay(3000);
    digitalWrite(LED_RED, LOW);
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK)
  {
    digitalWrite(LED_RED, HIGH);
    delay(3000);
    digitalWrite(LED_RED, LOW);
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  for (byte i = 0; i < 13; i++)
  {
    if (char(buffer[i]) == '!') {
      schoolBusId[i] = '\0';
      i = 16;
    }
    else {
      Serial.print(char(buffer[i]));
      schoolBusId[i] = (char(buffer[i]));
    }
  }
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}

float SpeedBus(int pid) {
  // read the Speed of the bus
  float speedBus = OBD2.pidRead(pid);

  if (isnan(speedBus)) {
    return -1;
  } else {
    speedBus *= 1,60934; //convertion from miles to km
    return speedBus;
  }
}
