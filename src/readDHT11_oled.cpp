#include <DFRobot_DHT11.h>
#include <Laika_FirebaseESP32.h>
#include "setDisplay.h"
#include "conecta.h"
#include "display.h"

DFRobot_DHT11 DHT;
#define DHT11_PIN 2

// #define FIREBASE_HOST "" // // databaseUrl SIN https:// y SIN "/" al final
// #define FIREBASE_AUTH ""         // secreto de la base de datos

int temp;
int hum;

void setup()
{
    Serial.begin(9600);
    setDisplay();
    conecta();
    // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
    // Limpiamos el buffer del display
    display.clearDisplay();

    DHT.read(DHT11_PIN);
    temp = DHT.temperature;
    hum = DHT.humidity;

    Serial.print("temp:");
    Serial.print(temp);
    // Firebase.setInt("casa1/temp", temp);
    // Mostramos el texto en el display
    display.setCursor(0, 0);
    display.println("Temperatura: ");
    display.println(temp);

    Serial.print("  humi:");
    Serial.println(hum);
    Serial.println("\n");
    // Firebase.setInt("casa1/hum", hum);
    // Mostramos el texto en el display
    display.setCursor(0, 20);
    display.println("Humedad: ");
    display.println(hum);

    // Actualizamos el display
    display.display();
    delay(1000);
}
