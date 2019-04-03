/*Esquemas de ligação:
 * Modulo RFID:
  - RST -> 9;
  - MISO -> 12;
  - MOSI -> 11;
  - SCK -> 13; 
  - SDA -> 10;
  -Alimentação 3.3v;
  MODELO LINK GOOGLE MAPS:https://maps.google.com/maps?ll=lat,long
 * Modulo SIM808L
  - RX -> 5;
  - TX -> 6;
  APN USUARIO E SENHA OPERADORAS
    TIM: "timbrasil.br", "tim", "tim"
    Vivo: "zap.vivo.com.br", "vivo", "vivo"
    Claro: "claro.com.br", "claro", "claro"
    OI: "gprs.oi.com.br", "oi", "oi"
 */
//Bibliotecas
#include <SPI.h> //Faz a comunicação entre o arduino e o RFID
#include <Lista.h>
#include <MFRC522.h>
#include <SoftwareSerial.h> //Gera comunicção serie no arduino
#include "inetGSM.h"

//Declaração de variaveis pinos
#define SS_PIN 10 //Pino SS do modulo RFID
#define RST_PIN 9 //Pino reset do modulo RFID
#define RX_SIM808 5 // Pino para comunicação RX com modulo SIM808
#define TX_SIM808 6 // Pino para comunicação TX com modulo SIM808
#define DEBUG true
#define LED_GREEN 4
#define LED_RED 3

//Variaveis globais
String latitudeAux, longitudeAux, timeGPS;

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
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  //Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
}

void loop() {
 // if((gprsConexao.attachGPRS("zap.vivo.com.br", "vivo", "vivo")) && (listaEmbarcados.ehVazio() != 1)){
    //Função para o servidor
  //}
  //Aguarda cartao
  while (!mfrc522.PICC_IsNewCardPresent())
  {
    delay(100);
  }
  if (mfrc522.PICC_ReadCardSerial())
  {
    modo_leitura();
  }
  
}

void modo_leitura(){
  char nome[50];
  char telefone[13];
  char id[15];
  char latitude[11];
  char longitude[11];
  char hora[9];
  
  //Mostra UID na serial
  Serial.print(F("UID da tag : "));
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i]<0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();

  //Obtem os dados do setor 4, bloco 16 = ID
  byte sector         = 4;
  byte blockAddr      = 16;
  byte trailerBlock   = 19;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                  trailerBlock, &key, &(mfrc522.uid));
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
  //Obtem e mostra os dados do ID no LCD
  //lcd.clear();
  //lcd.setCursor(0, 0);
  for (byte i = 1; i < 16; i++)
  {
    if(char(buffer[i]) == '!'){
      id[i-1] = '\0';
      i = 16;
    }
    else{
    Serial.print(char(buffer[i]));
    //lcd.write(char(buffer[i]));
    id[i-1] = (char(buffer[i]));
    }
  }
  Serial.println(F("ID"));
  Serial.println(id);
  Serial.println();
  
  //Obtem os dados do setor 3, bloco 12 = Nome
  sector         = 3;
  blockAddr      = 12;
  trailerBlock   = 15;
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                  trailerBlock, &key, &(mfrc522.uid));
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
  //Obtem e mostra os dados do nome no LCD
  //lcd.clear();
  //lcd.setCursor(0, 0);
  for (byte i = 1; i < 16; i++)
  {
    if(char(buffer[i]) == '!'){
      nome[i-1] = '\0';
      i = 16;
    }
    else{
    Serial.print(char(buffer[i]));
    //lcd.write(char(buffer[i]));
    nome[i-1] = (char(buffer[i]));
    }
  }
  Serial.println(F("NOME NA VARIAVEL"));
  Serial.println(nome);
  Serial.println();
 
  //Obtem os dados do setor 2, bloco 8 = Telefone
  sector         = 2;
  blockAddr      = 8;
  trailerBlock   = 11;
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
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
  //lcd.setCursor(0, 1);
  for (byte i = 0; i < 13; i++)
  {
    if(char(buffer[i]) == '!'){
      telefone[i] = '\0';
      i = 16;
    }
    else{
    Serial.print(char(buffer[i]));
    //lcd.write(char(buffer[i]));
    telefone[i] = (char(buffer[i]));
    }
  }
  Serial.println(F("Numero"));
  Serial.println(telefone);
  Serial.println();
 
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  //Captar a logitude/latitude
  cordenadas("AT+CGNSINF", 1000, DEBUG);
  latitudeAux.toCharArray(latitude, 11);
  longitudeAux.toCharArray(longitude, 11);
  timeGPS.toCharArray(hora, 9);

  addLista(nome, telefone, id, latitude, longitude, hora);
  digitalWrite(LED_GREEN, HIGH);
  delay(3000);
  digitalWrite(LED_GREEN, LOW);
}

void addLista(char nome[], char telefone[], char id[], char latitude[], char longitude[], char hora[]){
  Serial.println("add Lista");
  if(!listaEmbarcados.existe(id)){
    listaEmbarcados.adiciona(nome, id, telefone, latitude, longitude, hora, "EMBARQUE");
    sms(nome, telefone, latitude, longitude, 0, hora);
  }
  else{
    listaEmbarcados.adiciona(nome, id, telefone, latitude, longitude, hora, "DESEMBARQUE");
    sms(nome, telefone, latitude, longitude, 1, hora);
  }
}

void sms(char nome[], char telefone[], char latitude[], char longitude[], int acao, char hora[]){
  Serial.println("SMS");
  String destinatario = "AT+CMGS=\"";
  destinatario += telefone;
  destinatario += "\"";
  String msg = (F("O aluno(a) "));
  msg += nome;
  if(acao == 0){
    msg += (F(" embarcou no transporte escolar na seguinte localizacao "));
  }
  else{
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
}

void cordenadas(String command , const int timeout , boolean debug){

  String data[5];

  sim808.println(command);
  long int time = millis();
  int i = 0;

  while((time+timeout) > millis()){
    while(sim808.available()){
      Serial.println("AQUIIIIIIIIIIIIII");
      char c = sim808.read();
      if (c != ',') {
         data[i] +=c;
         Serial.println(data[i]);
         delay(100);
      } else {
        i++;  
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (debug) {
    //state = data[1];
    timeGPS = data[2];
    latitudeAux = data[3];
    longitudeAux =data[4];
    Serial.println("Coordenada");
    Serial.println(timeGPS);
    Serial.println(latitudeAux);
    Serial.println(longitudeAux);
  }
}
