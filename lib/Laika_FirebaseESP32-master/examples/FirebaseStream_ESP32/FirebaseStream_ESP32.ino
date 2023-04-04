/*
 * Con este ejemplo podrmos usar el stream de la base de datos, es decir podemos}
 * estar consultando el coontenido de un nodo unicamente al cambio de datos
 * de esta forma no consumiremos tantos recursos en la base de datos
 * www.holalaika.com
 */

#include <Laika_FirebaseESP32.h>
#include <WiFi.h>

#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN / al final
#define FIREBASE_AUTH "" // secreto de la base de datos 

#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red


void setup() {
  Serial.begin(115200);

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
  
  Firebase.stream("/casa1/", [](FirebaseStream stream) { // monitorea el nodo principal
    String eventType = stream.getEvent();
    eventType.toLowerCase();

    Serial.print("event: ");
    Serial.println(eventType);
    if (eventType == "put") {
      Serial.println("-------------------------------------------");
      Serial.print("data: ");
      Serial.println(stream.getDataString()); 
      String path = stream.getPath(); // nombre del nodo
      String data = stream.getDataString();  // dato del nodo
      Serial.println(path); // nodo que esta cambiando
      Serial.println("-------------------------------------------"); 
    }
  });
}


void loop() {

}
