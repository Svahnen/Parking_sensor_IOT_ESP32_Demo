#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "NETGEAR88";
const char* password = "XXXXXXXXXX";

unsigned long last_time = 0;
unsigned long timer_delay = 10000;

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
 
  Serial.println("First set of readings will appear after 10 seconds");
}

void loop() {
  
  if ((millis() - last_time) > timer_delay) {
  
    if(WiFi.status()== WL_CONNECTED){
      String server = "http://192.168.1.8";
      
      json_array = GET_Request(server.c_str());
      Serial.println(json_array);
      JSONVar my_obj = JSON.parse(json_array);
  
      if (JSON.typeof(my_obj) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(my_obj);
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
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}
