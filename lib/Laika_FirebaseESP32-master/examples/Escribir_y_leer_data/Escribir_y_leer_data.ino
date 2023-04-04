#include <WiFi.h>
#include <Laika_FirebaseESP32.h>

#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN / al final
#define FIREBASE_AUTH "" // secreto de la base de datos 

#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  // escribir un valor Float
  Firebase.setFloat("number", 42.0);
  delay(1000);

  // leer un valor Float
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remover un nodo
  Firebase.remove("number");
  delay(1000);

  // escribir un valor String
  Firebase.setString("nodo", "el contenido");
  delay(1000);
  
  // escribir un valor bool
  Firebase.setBool("estatus", false);
  delay(1000);

  // agregar un nuevo valor /logs
  String name = Firebase.pushInt("logs", n++);
  
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}
