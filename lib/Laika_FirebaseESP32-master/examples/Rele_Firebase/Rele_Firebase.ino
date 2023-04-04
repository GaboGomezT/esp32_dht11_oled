#include <WiFi.h>
#include <Laika_FirebaseESP32.h>

#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN "/" al final
#define FIREBASE_AUTH "" // secreto de la base de datos 

#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red

#define RELE 33
String foco1;

void setup() {

  Serial.begin(115200);
  delay(1000);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WIFI");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected to WIFI!");
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  pinMode(RELE, OUTPUT);
}

void loop() {
  if (Firebase.failed()) {
    Serial.print("Error en base de datos firebase: ");
    Serial.println(Firebase.error());
    // return;
  }

  foco1 = Firebase.getString("/casa1/foco1");
  if (foco1 == "on") {
    Serial.println("rele prendido");
    digitalWrite(RELE , HIGH);
  } else if (foco1 == "off") {
    Serial.println("rele apagado");
    digitalWrite(RELE , LOW);
  }

}
