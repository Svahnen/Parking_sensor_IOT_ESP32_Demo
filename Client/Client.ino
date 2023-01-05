#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "NETGEAR88";
const char* password = "xxxxxxxxxxxx";

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);


unsigned long last_time = 0;
unsigned long timer_delay = 500;

int red = 0xFF0000;
int yellow = 0xFFFF00;
int green = 0x00FF00;

String json_array;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WIFI...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  #if defined(NEOPIXEL_POWER)
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);

  #endif
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(10); // not so bright
 
  Serial.println("First set of readings will appear after 10 seconds");
}

void loop() {
  
  if ((millis() - last_time) > timer_delay) {
  
    if(WiFi.status()== WL_CONNECTED){
      String server = "http://192.168.1.8";
      
      json_array = GET_Request(server.c_str());

      // Look for json lenght
      int lenght = 15;
      while(json_array[lenght] != '\n')
      {
        lenght++;
      }
      int counter = 0;
      for (int i = 15; i < lenght; ++i){
        //Serial.print(json_array[i]);
        counter++;
      }
      //Serial.println();

      // Look for how many times the foor loop looped and add extra numbers depending on how many unused characters comes after
      if (counter == 2){
        //Serial.println("red");
        pixels.fill(red);
      } else if (counter == 3){
        //Serial.println("yellow");
        pixels.fill(yellow);
      } else if (counter == 4){
        //Serial.println("green");
        pixels.fill(green);
      }
      pixels.show();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    last_time = millis();
  }
}
String GET_Request(const char* server) {
  HTTPClient http;    
  http.begin(server);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}