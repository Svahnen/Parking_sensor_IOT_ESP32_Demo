#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "NETGEAR88";
const char* password = "xxxxxxxxxxxxx";
WiFiServer server(80);

long duration, distanceCm;
const int TRIG_PIN = 18;
const int ECHO_PIN = 17;

// Client variables
char linebuf[80];
int charcount = 0;
void setup() {

    Serial.begin(9600);
    pinMode(TRIG_PIN,OUTPUT);
    pinMode(ECHO_PIN,INPUT);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
        // Connect to WPA/WPA2 network.
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}
void loop() {

    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN,HIGH);
    // convert the time into a distance
    distanceCm = duration / 29.1 / 2 ;

    // Wait for sensor to have time to work b4 looking for clients
    delay(500);

    // listen for incoming clients
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New client");
        memset(linebuf, 0, sizeof(linebuf));
        charcount = 0;
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                // read char by char HTTP request
                linebuf[charcount] = c;
                if (charcount < sizeof(linebuf) - 1) charcount++;

                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println();
                    client.println("{");

                    // Create an empty string to put the int into
                    char distanseStr[100];
                    char distanceJson[] = "\"distance\": ";

                    // Convert distanceCm into a string and put it in distanseStr
                    sprintf(distanseStr, "%ld", distanceCm);
                    
                    // Combine the 2 strings
                    strcat(distanceJson,distanseStr);

                    // Print the result
                    Serial.print("distanceJson: ");
                    Serial.println(distanceJson);

                    client.println(distanceJson);
                    client.println("}");
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                    memset(linebuf, 0, sizeof(linebuf));
                    charcount = 0;

                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
    }
}
