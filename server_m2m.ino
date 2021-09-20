#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <wire.h>
#include <LiquidCrystal_I2C.h>



const char* ssid = "mcu_host";
const char* password = "11223344";
int max_connection = 8;


IPAddress staticIP(10,10,10,1);
IPAddress gateway(10,10,10,1);
IPAddress subnet(255,255,255,0);

int httpCode;

HTTPClient http;

ESP8266WebServer server(80); 


void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}
void dro() {
  server.send(200, "text/plain", "Connection established" );
  Serial.println("connection established");
  digitalWrite(D2, HIGH);               
}
void drc() {
  server.send(200, "text/plain", "Door Closed" );
  Serial.println("Door Closed");
  digitalWrite(D2, LOW);;               
}


void setup(void){
 
  Serial.begin(115200);
  Serial.println("");
  WiFi.mode(WIFI_AP);          
  WiFi.softAP(ssid, password, 1, false, max_connection); 
  WiFi.softAPConfig(staticIP, gateway, subnet);
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());


  IPAddress myIP = WiFi.softAPIP();
  Serial.print("HotSpt IP:");
  Serial.println(myIP);
   pinMode(D2, OUTPUT);
  
   delay(1000);
  
   server.on("/", handleRoot);
   server.on("/dro", dro);
   server.on("/drc", drc);
   server.begin();
   Serial.println("HTTP server started");
}

void loop(void){

      server.handleClient();         
} 
