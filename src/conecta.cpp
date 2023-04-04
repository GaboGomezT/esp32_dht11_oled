#include <WiFi.h>

#define WIFI_SSID "Totalplay-76AA"       // nombre de la red 2.4 ghz
#define WIFI_PASSWORD "76AADB2F9hd6mTss" // contraseña de la red

void conecta()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WIFI");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Conectado al wifi");
}
