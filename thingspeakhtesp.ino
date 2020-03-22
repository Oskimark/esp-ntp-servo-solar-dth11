#include "ThingSpeak.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>
#define DHTTYPE DHT11
#define DHTPIN D7
DHT dht(DHTPIN, DHTTYPE);
int transmit_t = 0;
int transmit_h = 0;
int anterior_t = 0;
int anterior_h = 0;
int manana= 0;
int mananita= 0;
int mediodia= 0;
int tarde= 0;
int noche= 0;
int hora = 0;

float value = 0;
int x = 0;
WiFiUDP ntpUDP;
Servo myservo;  // create servo object to control a servo

IPAddress ip(192,168,0,25);     
IPAddress gateway(192,168,0,1);   
IPAddress subnet(255,255,255,0); 
//----------- Enter you Wi-Fi Details---------//
char ssid[] = "nnnnn";   // your network SSID (name)
char pass[] = "nnnnnnn";   // your network password
//-------------------------------------------//

//----------- Channel Details -------------//
unsigned long Channel_ID = 88430; // Channel ID
const int Field_number1 = 1; // Don't change
const int Field_number2 = 2; // Don't change
const char * WriteAPIKey = "nnnnnnn"; // Your write API Key
// ----------------------------------------//
WiFiClient  client;
NTPClient timeClient(ntpUDP, "0.south-america.pool.ntp.org",-10800,6000);

void setup()
{
  dht.begin();
  Serial.begin(115200);
  Serial.println("https://github.com/Oskimark/esp-ntp-servo-solar-dth11");
  IPAddress ip(192, 168, 0, 25);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  myservo.attach(D2);  // attaches the servo on pin 9 to the servo object

}

void loop()
{
  leerht();
  comparar();

 // leerhora();
  delay(10000);
}





void upload()
{
  internet();
  x = ThingSpeak.writeFields(Channel_ID, WriteAPIKey);
   timeClient.begin(); 
   timeClient.update(); //sincronizamos con el server NTP
   hora=timeClient.getHours();
  //Serial.println(timeClient.getFormattedTime());
   Serial.println(hora);
  switch (hora) {
    case 7: 
      if (manana==0)
      {myservo.attach(D2);
        myservo.write(0);                 // sets the servo position according to the scaled value
      delay(1000);
      myservo.detach();
      
      manana=1;
      noche=0;}        
      break;
   
    case 10: 
      if (mananita==0)
      {myservo.attach(D2);
        myservo.write(45);                  // sets the servo position according to the scaled value
      delay(1000);
      myservo.detach();
      mananita=1; }          
      break;
   
    case 13: 
      if (mediodia==0)
      {myservo.attach(D2);
        myservo.write(90);                  // sets the servo position according to the scaled value
      delay(1000);
      myservo.detach();
      mediodia=1; }          
      break;
   
    case 16: 
      if (tarde==0)
      {myservo.attach(D2);
        myservo.write(120);                  // sets the servo position according to the scaled value
      delay(1000);
      myservo.detach();
      tarde=1; }          
      break;
   
    case 18: 
      if (noche==0)
      {myservo.attach(D2);
        myservo.write(180);                  // sets the servo position according to the scaled value
      delay(1000);
      myservo.detach();
      noche=1;
      manana=0;
      mananita=0; 
      mediodia=0;
      tarde=0; }          
      break;
   }






  
  if (x == 200)Serial.println("Data Updated.");
  if (x != 200)
  {
    Serial.println("Data upload failed, retrying....");
    delay(15000);
    upload();
  }
  delay(15000);

}
void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED)
    {// WiFi.config(ip, gateway, subnet);
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
     Serial.println(WiFi.localIP());
  
  }
}
void comparar()
{
 if ((anterior_h != transmit_h)||(anterior_t!=transmit_t))
   upload();
 
 Serial.println("sin cambios");
}
void leerht()
{
anterior_h=transmit_h;//guardo los ultimos valores para compararlos
anterior_t=transmit_t;
 
float h = dht.readHumidity();
float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

 transmit_h=(int)h;
 transmit_t=(int)t;
 
 ThingSpeak.setField(1, transmit_t);
 ThingSpeak.setField(2, transmit_h);
  //ThingSpeak.setField(3, number3);
   delay(500);
  Serial.println(transmit_h);
  Serial.println(transmit_t);
  }
