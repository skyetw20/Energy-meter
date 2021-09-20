#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>

LiquidCrystal_I2C lcd=LiquidCrystal_I2C(0x27,16,2);



char * ssid = "mcu-s";
char * password = "12345678";
IPAddress ip(192,168,11,4);  
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server;


String id_c="" , id_s="S001";
float units_c=0.0;


const char *sta_ssid = "FTTH"; 
const char *sta_password = "nodemcu@80";


const char* host = "script.google.com";
const int httpsPort = 443;
#define ON_Board_LED 3


WiFiClientSecure client;

String GAS_ID = "AKfycbxFPOTMVLCK4MIY0QJwrhqBDCKtM-bYfpMH_3MJ5TQNGVMr82-z1yGje3kJ7fGQtGuO";


const int sensorIn =A0;
int mVperAmp=185;
double vpp=0;
double vp=0;
double vrms=0;
double irms=0;
double units=0;




void setup() {
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Welcome!!!");
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(ip,gateway,subnet);
  WiFi.softAP(ssid,password);
  server.on("/",handleindex);
  server.on("/update",handleupdate);
  server.begin();
  lcd.clear();
  lcd.print("Server started!!");
  delay(1000);
  lcd.clear();
  WiFi.begin(sta_ssid, sta_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.clear();
    lcd.print("Connecting...");
  }
    client.setInsecure();
  lcd.clear();
  if(WiFi.status() == WL_CONNECTED)
  {
    lcd.print("Connected!!!");
    delay(1000);
    lcd.clear();
    }
}


void loop() {
    server.handleClient();
    vpp=getVPP();
  vp=vpp/2.0;
  vrms=vp*0.707;
  irms=((vrms*1000)/mVperAmp)-0.07;
  units+= (((irms*243)/1000)/1800);
  String u="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ID=S001");
  lcd.setCursor(0,1);
  lcd.print("Units=");
  lcd.setCursor(6,1);
  u+=String(units);
  u+="KwH";
  lcd.print(u);
   if (WiFi.status()!= WL_CONNECTED)
    {
      lcd.clear();
      lcd.print("Connection lost!");
       delay(1000);
    }
   if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  String v1=String(id_s);
  String v2=String(units);
  String v3=String("updated");
  String url = "/macros/s/" + GAS_ID + "/exec?value1=" + v1 + "&value2=" + v2 + "&value3=" + v3;
  Serial.print("requesting URL: ");
  Serial.println(url);  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
  Serial.println("request sent");
delay(1000);
lcd.clear();

}
void handleindex() {
  server.send(200, "text/plain", String(units_c));
}
void handleupdate() {
  units_c=server.arg("units_c").toFloat();
  id_c=server.arg("id");
  Serial.println(units_c);
  Serial.println(id_c);
  server.send(200, "text/plain", "Updated" ); 
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  String v1=String(id_c);
  String v2=String(units_c);
  String v3=String("updated");
  String url = "/macros/s/" + GAS_ID + "/exec?value1=" + v1 + "&value2=" + v2 + "&value3=" + v3;
  Serial.print("requesting URL: ");
  Serial.println(url);  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
  Serial.println("request sent");             
}
double getVPP()
{double result;
int readvalue;
int maxvalue=0;
int minvalue=1024;
uint32_t start_time=millis();
while((millis()-start_time)<1000)
  {readvalue=analogRead(sensorIn);
  if (readvalue>maxvalue)
      {maxvalue=readvalue;
      }
   if (readvalue<minvalue)
    {minvalue=readvalue;
     }   
      
    }
    result=((maxvalue-minvalue)*5.0)/1024.0;
    return result;
  }
