//..............................................importing libraries
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//..............................................creating an instance of LiquidCrystal
LiquidCrystal_I2C lcd=LiquidCrystal_I2C(0x27,16,2);


//..............................................ssid and password of the module acting as server
const char* ssid = "mcu-s";
const char* password = "12345678";
const char* host="192.168.11.4";
WiFiClient client;


//..............................................declaring variables
const int sensorIn =A0;
int mVperAmp=185;
double vpp=0;
double vp=0;
double vrms=0;
double irms=0;
double units=0;
String id="C001";
String u="";


//............................................set up function
void setup() {
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
    lcd.clear();
    lcd.print("Welcome!!");
    delay(1000);
    WiFi.begin(ssid,password);
    
    while(WiFi.status()!= WL_CONNECTED)
    {
      lcd.clear();
      lcd.print("Connecting..");
      delay(1000);
    }
    if (WiFi.status()== WL_CONNECTED)
    {
      lcd.clear();
      lcd.print("Connected!!");
      delay(1000);
    }
    
 
    }



void loop() {
  vpp=getVPP();
  vp=vpp/2.0;
  vrms=vp*0.707;
  irms=((vrms*1000)/mVperAmp)-0.07;
  units+= (((irms*243)/1000)/1800);
  u="";
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ID=C001");
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




  
  if (client.connect(host,80))
  {String url="/update?id=";
  url+=String(id);
  url+="&units_c=";
  url+=String(units);
  client.print(String("GET ")+url+" HTTP/1.1\r\n"+"Host: "+host + "\r\n"+"Connection: keep-alive\r\n\r\n");
  }
  delay(1000);
  lcd.clear();
}




double getVPP()
{
  double result;
  int readvalue;
  int maxvalue=0;
  int minvalue=1024;
  uint32_t start_time=millis();
  while((millis()-start_time)<1000)
  {
    readvalue=analogRead(sensorIn);
    if (readvalue>maxvalue)
      { 
        maxvalue=readvalue;
      }
   if (readvalue<minvalue)
    {
      minvalue=readvalue;
    }   
      
  }
  result=((maxvalue-minvalue)*5.0)/1024.0;
  return result;
}
