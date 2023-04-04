/*
  Vcc a pin 5 or 3.3V
  GND a GND
  PZC a pin 16 (GPIO posibles 0,1,2,4,5,7,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33,34,35,36,39)
  PD a pin 17 (GPIO posibles 0,1,2,3,4,5,12,13,14,15,16,17,18,19,21,22,23,25,26,27,32,33 )
*/

#include <Laika_FirebaseESP32.h>
#include <WiFi.h>

#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN "/" al final
#define FIREBASE_AUTH "" // secreto de la base de datos 

#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red

#define PATH "" //nodo principal ejemplo: casa1/

//Libraries
#include <RBDdimmer.h> //https://github.com/RobotDynOfficial/RBDDimmer Se requiere esta librería
//Parameters
const int zeroCrossPin  = 16;
const int acdPin  = 17;
int MIN_POWER  = 0;
int MAX_POWER  = 80;
int POWER_STEP  = 2;
//Variables
int power  = 0;
//Objects
dimmerLamp acd(acdPin, zeroCrossPin);
void setup() {
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("ESP32 System"));
  acd.begin(NORMAL_MODE, ON);


  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.stream(PATH , [](FirebaseStream stream) { // monitorea el nodo principal
    String eventType = stream.getEvent();
    eventType.toLowerCase();

    Serial.print("event: ");
    Serial.println(eventType);
    if (eventType == "put" or eventType == "patch") {
      //      Serial.println("-------------------------------------------");
      //      Serial.print("data: ");
      //      Serial.println(stream.getDataString());
      //      String pathStream = stream.getPath(); // nombre del nodo
      //      String data = stream.getDataString();  // dato del nodo
      //      Serial.println(pathStream); // nodo que esta cambiando
      //      Serial.println("-------------------------------------------");

      JsonVariant obj = stream.getData();
      
      power = obj["dimmer"];
      acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
      //delay(100);


    }
  });
}
void loop() {
  //testDimmer();
}
void testDimmer() { /* function testDimmer */
  ////Sweep light power to test dimmer
  for (power = MIN_POWER; power <= MAX_POWER; power += POWER_STEP) {
    acd.setPower(power); // setPower(0-100%);
    Serial.print("lampValue -> ");
    Serial.print(acd.getPower());
    Serial.println("%");
    delay(100);
  }
  for (power = MAX_POWER; power >= MIN_POWER; power -= POWER_STEP) {
    acd.setPower(power); // setPower(0-100%);
    Serial.print("lampValue -> ");
    Serial.print(acd.getPower());
    Serial.println("%");
    delay(100);
  }
}
