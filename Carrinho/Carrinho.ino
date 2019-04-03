#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <NodeMotorDriver.h>

const char* ssid = "MotoX4";
const char* password = "MotoX4Renan";

int led = LED_BUILTIN;

ESP8266WebServer server(80);

NodeMotorDriver nodeMotorDriver(5, 0, 4, 2);

void setup() {
  pinMode(led, OUTPUT);
  
  Serial.begin(9600);

  startWiFi();

  SPIFFS.begin();
  
  startServer();
}

void loop() {
  server.handleClient();
}

void startWiFi() {
  WiFi.softAP(ssid, password);
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
}

void startServer() {
  // aguarda por requisições na URL raiz "/" ou "/jsdata"
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/materialize.min.js", SPIFFS, "/materialize.min.js");
  server.serveStatic("/materialize.min.css", SPIFFS, "/materialize.min.css");
  server.serveStatic("/manifest.json", SPIFFS, "/manifest.json");
  server.serveStatic("/nipplejs.min.js", SPIFFS, "/nipplejs.min.js");
  server.serveStatic("/robot.png", SPIFFS, "/robot.png");
  server.on("/jsdata", handleJsData);  
  server.begin();
}

// recebe os dados do Joystick, parseia dados JSON e chama função de movimentação dos motores
void handleJsData() {
  String data = server.arg("plain");
  server.send(204,"");  
  DynamicJsonBuffer jBuffer(4000);
  JsonObject& root = jBuffer.parseObject(data);
  int dist = root["distance"];
  String angle = root["direction"]["angle"];
  String state = root["type"];
  Serial.println(dist);
  Serial.println(state);
  moveMotors(dist, angle, state);
  digitalWrite(led, !digitalRead(led));  
}

// move motores para determinada posição
void moveMotors(int dist, String angle, String state) {
  if(state == "end") {
    nodeMotorDriver.stop();     
  } else {      
    if(angle == "up") nodeMotorDriver.goForward(map(dist, 0, 100, 0, 1024));
    if(angle == "down") nodeMotorDriver.goBackward(map(dist, 0, 100, 0, 1024));
    if(angle == "left") nodeMotorDriver.turnLeft(map(dist, 0, 100, 0, 1024));
    if(angle == "right") nodeMotorDriver.turnRight(map(dist, 0, 100, 0, 1024));
  }   
}
