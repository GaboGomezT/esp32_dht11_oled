#include "display.h"

void setDisplay()
{
    // Inicializamos el display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("Error al iniciar el display SSD1306"));
        delay(1000);
        ESP.restart();
    }

    // Limpiamos el buffer del display
    display.clearDisplay();

    // Establecemos el tamaño y el tipo de fuente para el texto
    display.setTextSize(1);
    display.setTextColor(WHITE);
}
