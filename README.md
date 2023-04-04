# esp32_dht11_oled

Project generado con PlatformIO usando la placa ESP32 y el framework Arduino

Usando el sensor DHT11 y una pantalla Oled leyemos y desplegamos datos de temperatura y humedad.

> Antes de compilar el proyecto debes llenar los credenciales en el archivo [conecta.cpp](src/conecta.cpp)

También tiene la opción de subir los datos a firebase. Debes descomentar las líneas 10, 11, 21, 35 y 44 del archivo [readDHT_oled.cpp](src/readDHT11_oled.cpp) además de reemplazar los datos de Firebase por los suyos.
