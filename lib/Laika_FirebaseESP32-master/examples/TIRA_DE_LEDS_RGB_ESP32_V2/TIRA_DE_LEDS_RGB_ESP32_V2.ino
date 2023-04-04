#include <WiFi.h>
#include <Laika_FirebaseESP32.h>
#include "analogWrite.h"

#define PATH "" // nodo principal ejemplo: casa1/

#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN "/" al final
#define FIREBASE_AUTH "" // secreto de la base de datos 

#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red

//  ------------------------------

// pines
#define RED 25
#define GREEN 26
#define BLUE 27

void setup() {

  Serial.begin(9600); // velocidad

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado !! ");

  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  analogWriteResolution(RED, 8); // resolución a 8bits
  analogWriteResolution(GREEN, 8);
  analogWriteResolution(BLUE, 8);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);


  //Stream--------------------------------------------------------------------------------
  Firebase.stream(PATH , [](FirebaseStream stream ) { // monitorea el nodo principal se ejecuta en una tarea en un el nucleo 1 de la esp32
    String eventType = stream.getEvent();
    eventType.toLowerCase();

    // Serial.print("event: ");
    // Serial.println(eventType);
    if (eventType == "put" or eventType == "patch") {
      String pathStream = stream.getPath(); // nombre del nodo
      String data = stream.getDataString();  // dato del nodo

      JsonVariant obj = stream.getData(); // lo guarda en un objeto de tipo json

      obj["rgb"].prettyPrintTo(Serial); // imprimimos el objeto en formato jspon en monitor serial

      analogWrite(RED, obj["rgb"]["r"]); 
      analogWrite(GREEN, obj["rgb"]["g"]);
      analogWrite(BLUE, obj["rgb"]["b"]);

    }
  });
}

void loop() {
  if (Firebase.failed()) {
    Serial.print("Error en connexion de firebase:");
    Serial.println(Firebase.error());
    delay(1000);
  }

  // apartir de aquí puedes meter el codigo que quieras el cual se ejecutara en el nucleo 0 de la esp32
}
