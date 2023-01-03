#include "WiFi.h"
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "NETGEAR88";
const char* password = "XXXXXXXXXXXX";

const int TRIG_PIN = 18;
const int ECHO_PIN = 17;
long duration, distanceCm;

WebServer server(80);

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];
void create_json(char *tag, float value) {  
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  serializeJson(jsonDocument, buffer);  
}
 
void add_json_object(char *tag, float value) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
}


void getDistance() {
  create_json("distance", distanceCm);
  server.send(200, "application/json", buffer);
}

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);

  // Wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi"); 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to the WiFi network"); 
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

  server.on("/sensor", getDistance);
  // start server
  server.begin();
}

void loop()
{
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 digitalWrite(TRIG_PIN, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG_PIN, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_PIN, LOW);
 duration = pulseIn(ECHO_PIN,HIGH);
 
 // convert the time into a distance
 distanceCm = duration / 29.1 / 2 ;

 Serial.println(distanceCm);

 delay(1000);
}
