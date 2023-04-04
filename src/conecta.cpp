#include <WiFi.h>

#define WIFI_SSID ""     // nombre de la red 2.4 ghz
#define WIFI_PASSWORD "" // contraseña de la red

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
