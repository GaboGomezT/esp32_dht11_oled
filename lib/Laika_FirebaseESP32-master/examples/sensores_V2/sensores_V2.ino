//librerias
#include <WiFi.h>
#include <Laika_FirebaseESP32.h>
#include <SimpleDHT.h> // librería DHT11 temperatura y humedad
#include <Tone32.h> // librería necesaria para el buzzer
//------------------------------
#define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN "/" al final
#define FIREBASE_AUTH "" // secreto de la base de datos 


#define WIFI_SSID ""  // nombre de la red 2.4 ghz
#define WIFI_PASSWORD ""   // contraseña de la red

// variables -----------------------------
#define RELE 33 // D1 RELE 
#define LED1 12 // AZUL
#define LED2 13 // AMARILLO
#define LED3 15 // ROJO

#define PATH "casa1/" //nodo principal

String foco1; // lee estado de firebase
String foco2; // lee estado de firebase

#define PIN_DHT11 2 // pin del DHT11
int antHum = 0;
SimpleDHT11 dht11(PIN_DHT11);

#define SENSOR_PIR 16 //   SENSOR PIR 
String alarma;

#define BUZZER_PIN 32 // buzzer

#define INTERVALO_PIR 1200
unsigned long tiempo_3 = 0;

#define INTERVALO_DHT11 3000
unsigned long tiempo_2 = 0;


void print_tiempo(unsigned long tiempo_millis);
// FIN Variables --------------------------
void setup() {
  Serial.begin(9600); // velocidad

  // CONFIGURAMOS----------------
  pinMode(RELE, OUTPUT);  // FOCO 1
  pinMode(LED1 , OUTPUT);
  pinMode(LED2 , OUTPUT);
  pinMode(LED3 , OUTPUT);
  digitalWrite(LED3 , LOW); // desde un inicio lo apagamos
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

  //Stream--------------------------------------------------------------------------------
  Firebase.stream( PATH,  [](FirebaseStream stream) { // monitorea el nodo principal

    Serial.println("nucleo");
    String eventType = stream.getEvent();
    eventType.toLowerCase();

    Serial.print("event: ");
    Serial.println(eventType);
    if (eventType == "put" or eventType == "patch") {
      String pathStream = stream.getPath(); // nombre del nodo
      String data = stream.getDataString();  // dato del nodo

      JsonVariant obj = stream.getData(); // lo guarda en un objeto de tipo json

      if (obj["foco1"] == "on") {
        digitalWrite(RELE , HIGH);
      } else if (obj["foco1"] == "off") {
        digitalWrite(RELE , LOW);
      }

      if (obj["foco2"] == "on") {
        Leds(true);
      } else if (obj["foco2"] == "off") {
        Leds(false);
      }

      if (obj["alarma"] == "on") {
        alarma = "on";
      } else if (obj["alarma"] == "off") {
        alarma = "off";
      }
      
    }
  });

  //------------------------------------------------------------------------


}

void loop() {
  yield(); // es recomendable poner esta funcion de arduino para WD (El perro guardian)
  if (Firebase.failed()) {
    Serial.print("Error en connexion de firebase:");
    Serial.println(Firebase.error());
    delay(1000);
  }


  if (millis() > tiempo_2 + INTERVALO_DHT11) {
    tiempo_2 = millis();
    Serial.println("DHT11");

    // Temperatura:
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
      Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
      return;
    }

    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, ");
    Serial.print((int)humidity); Serial.println(" H");

    int hum = (int)humidity;
    if (hum != antHum) {
      Firebase.setInt("/casa1/temperatura", (int)temperature);
      Firebase.setInt("/casa1/humedad", (int)humidity);
      antHum = (int)humidity;
    }
  }

  if (millis() > tiempo_3 + INTERVALO_PIR) {
    tiempo_3 = millis();
    Serial.println("PIR");


    // SENSOR_PIR PIR -------------------------------------------------------
    alarma =  Firebase.getString("/casa1/alarma");


    if (alarma == "on") {
      bool state = digitalRead(SENSOR_PIR); // Lee el estado del SENSOR_PIR PIR
      if (state == true) {
        Serial.println("Movimiento detectado");
        Firebase.setString("/casa1/movimiento", "on");
        tone(BUZZER_PIN, NOTE_C4, 150, 0);
        noTone(BUZZER_PIN, 0);
        tone(BUZZER_PIN, NOTE_D4, 150, 0);
        noTone(BUZZER_PIN, 0);
      } else {
        Serial.println("Movimiento NO detectado");
        Firebase.setString("/casa1/movimiento", "off");
        noTone(BUZZER_PIN, 0);
      }
    } else {
      Firebase.setString("/casa1/movimiento", "off");
    }
    // ---------------------------------------------------------


  }

}

// activación de los leds
void Leds(bool estado) {
  digitalWrite(LED1 , estado);
  digitalWrite(LED2 , estado);
  digitalWrite(LED3 , estado);
}
