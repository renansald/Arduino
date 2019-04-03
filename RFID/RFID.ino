// Pinos montagem RST -> 9, MISO -> 12, MOSI -> 11, SCK -> 13, SDA -> 10
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
 
//Pinos Reset e SS módulo MFRC522
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

char nome[17];
 
#define pino_botao_le A2
#define pino_botao_gr A3
 
MFRC522::MIFARE_Key key;
 
void setup()
{
  pinMode(pino_botao_le, INPUT);
  pinMode(pino_botao_gr, INPUT);
  Serial.begin(9600);   //Inicia a serial
  SPI.begin();      //Inicia  SPI bus
  mfrc522.PCD_Init();   //Inicia MFRC522
 
  //Inicializa o LCD 16x2
  lcd.begin(16, 2);
  mensageminicial();
 
  //Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
}
 
void loop()
{
  //Verifica se o botao modo leitura foi pressionado
  int modo_le = digitalRead(pino_botao_le);
  if (modo_le != 0)
  {
    lcd.clear();
    Serial.println("Modo leitura selecionado");
    lcd.setCursor(2, 0);
    lcd.print("Modo leitura");
    lcd.setCursor(3, 1);
    lcd.print("selecionado");
    while (digitalRead(pino_botao_le) == 1) {}
    delay(3000);
    modo_leitura();
  }
  //Verifica se o botao modo gravacao foi pressionado
  int modo_gr = digitalRead(pino_botao_gr);
  if (modo_gr != 0)
  {
    lcd.clear();
    Serial.println("Modo gravacao selecionado");
    lcd.setCursor(2, 0);
    lcd.print("Modo gravacao");
    lcd.setCursor(3, 1);
    lcd.print("selecionado");
    while (digitalRead(pino_botao_gr) == 1) {}
    delay(3000);
    modo_gravacao();
  }
}
void mensageminicial()
{
  Serial.println("nSelecione o modo leitura ou gravacao...");
  Serial.println();
  lcd.clear();
  lcd.print("Selecione o modo");
  lcd.setCursor(0, 1);
  lcd.print("leitura/gravacao");
}
 
void mensagem_inicial_cartao()
{
  Serial.println("Aproxime o seu cartao do leitor...");
  lcd.clear();
  lcd.print(" Aproxime o seu");
  lcd.setCursor(0, 1);
  lcd.print("cartao do leitor");
}
 
void modo_leitura()
{
  mensagem_inicial_cartao();
  //Aguarda cartao
  while ( ! mfrc522.PICC_IsNewCardPresent())
  {
    delay(100);
  }
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag : ");
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
 
  //Obtem os dados do setor 3, bloco 12 = Nome
  byte sector         = 3;
  byte blockAddr      = 12;
  byte trailerBlock   = 15;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                  trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  //Mostra os dados do nome no Serial Monitor e LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  for (byte i = 1; i < 16; i++)
  {
    Serial.print(char(buffer[i]));
    lcd.write(char(buffer[i]));
    nome[i] = char(buffer[i]);
  }
  Serial.println("NOME NA VARIAVEL");
  for(int i = 0; i < 17; i++){
  Serial.print(nome[i]);
  }
  Serial.println();
 
  //Obtem os dados do setor 2, bloco 8 = Sobrenome
  sector         = 2;
  blockAddr      = 8;
  trailerBlock   = 11;
 
  //Autenticacao usando chave A
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                  trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  //Mostra os dados do sobrenome no Serial Monitor e LCD
  lcd.setCursor(0, 1);
  for (byte i = 0; i < 16; i++)
  {
    Serial.print(char(buffer[i]));
    lcd.write(char(buffer[i]));
  }
  Serial.println();
 
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
  delay(3000);
  mensageminicial();
}
 
void modo_gravacao()
{
  mensagem_inicial_cartao();
  //Aguarda cartao
  while ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(100);
  }
  if ( ! mfrc522.PICC_ReadCardSerial())    return;
 
  //Mostra UID na serial
  Serial.print(F("UID do Cartao: "));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  //Mostra o tipo do cartao
  Serial.print(F("nTipo do PICC: "));
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
 
  byte buffer[34];
  byte block;
  byte status, len;
 
  Serial.setTimeout(20000L) ;
  Serial.println(F("Digite o Telefone+#"));
  lcd.clear();
  lcd.print("Digite o telefon");
  lcd.setCursor(0, 1);
  lcd.print("e+#");
  len = Serial.readBytesUntil('#', (char *) buffer, 30) ;
  for (byte i = len; i < 30; i++) buffer[i] = ' ';
 
  block = 8;
  //Serial.println(F("Autenticacao usando chave A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 8
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  block = 9;
  //Serial.println(F("Autenticacao usando chave A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 9
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

    Serial.setTimeout(20000L) ;
  Serial.println(F("Digite o nome, em seguida o caractere #"));
  lcd.clear();
  lcd.print("Digite o nome e");
  lcd.setCursor(0, 1);
  lcd.print("em seguida #");
  len = Serial.readBytesUntil('#', (char *) buffer, 30) ;
  for (byte i = len; i < 30; i++) buffer[i] = ' ';
 
  block = 12;
  //Serial.println(F("Autenticacao usando chave A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 12
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  block = 13;
  //Serial.println(F("Autenticacao usando chave A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 13
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  Serial.println(F("Digite o ID, em seguida o caractere #"));
  lcd.clear();
  lcd.print("Digite o ID e");
  lcd.setCursor(0, 1);
  lcd.print("em seguida #");
  len = Serial.readBytesUntil('#', (char *) buffer, 20) ;
  for (byte i = len; i < 20; i++) buffer[i] = ' ';
 
  block = 16;
  //Serial.println(F("Autenticacao usando chave A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 16
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  block = 17;
  //Serial.println(F("Authenticating using key A..."));
  status=mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                    block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
 
  //Grava no bloco 17
  status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    //return;
  }
  else
  {
    Serial.println(F("Dados gravados com sucesso!"));
    lcd.clear();
    lcd.print("Gravacao OK!");
  }
 
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  delay(5000);
  mensageminicial();
}
