#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


const char* ssid = "Saldanha";
const char* pass = "3NGdohawa11";
char sentenca[128];

char user[] = "arduino";
char password[] = "arduino";

IPAddress server_addr(127,0,0,1);
char INSERT_NOME[] = "INSERT INTO registrotemp (nome) VALUES (%s)";
char BANCODEDADOS[] = "USER banco_arduino";

WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  if (conn.connect(server_addr, 3306, user, password))
  {
    delay(1000);
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(BANCODEDADOS);
    delete cur_mem;
  }
  else {
    Serial.println("A conexão falhou");
    conn.close();
  }
}

int i = 1;

void loop() {
  if(i == 1){
  char nome[] = "Joao";
  sprintf(sentenca, INSERT_NOME, nome);
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(sentenca);
  delete cur_mem;
  }
  i = 0;
}
