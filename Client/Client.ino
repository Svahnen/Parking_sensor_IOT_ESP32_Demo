#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

int red = 0xFF0000;
int yellow = 0xFFFF00;
int green = 0x00FF00;
int currentcolor = green; 

const char* ssid = "NETGEAR88";
const char* password = "XXXXXXXX";

//Your Domain name with URL path or IP address with path
String serverName = "https://";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi"); 

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to the WiFi network"); 
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());

#if defined(NEOPIXEL_POWER)
 pinMode(NEOPIXEL_POWER, OUTPUT);
 digitalWrite(NEOPIXEL_POWER, HIGH);

#endif
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 pixels.setBrightness(10); // not so bright
}
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    Serial.println("You can try to ping me"); 
    pixels.fill(currentcolor);
    pixels.show();
  }
  else{
    Serial.println("Connection lost"); 
  }
  // Send HTTP GET request
  int httpResponseCode = http.GET();

  if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  delay(1000); 
}
