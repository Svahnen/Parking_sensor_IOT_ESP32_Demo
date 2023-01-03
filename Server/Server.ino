const int TRIG_PIN = 18;
const int ECHO_PIN = 17;
void setup() {
 // initialize serial communication:
 Serial.begin(9600);
 pinMode(TRIG_PIN,OUTPUT);
 pinMode(ECHO_PIN,INPUT);
}
void loop()
{
 long duration, distanceCm;

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
