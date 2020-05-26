**Patricio Coronado Collado abril 2020**
# DESCRIPCIÓN
 Proyecto realizado para probar la capacidad del [ESP32 de Espressif](https://www.espressif.com/en/products/socs/esp32/overview)
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto8.png "ESP32")
## 
 Se utiliza la posibilidad ofrecida por este dispositivo
 de programarlo por OTA (mediante WiFi). Se conecta al PC mediante el módulo HC-05 Bluetooth.
 Lee la potencia mediante un PZEM-004T ver.3. Lee la temperatura mediante un DHT22.
 Conecta por WiFi con una base de datos en una LAN instalada en una Raspberry Pi
 y le pone datos mediante POST .Se crea una gráfica delos datos en el servidor.
## 
# DEPENDENCIAS
Librerias Arduino (incluidas en el proyecto):
## DHTesp, HTTPClientESP32, PZEM-004T-v30-master, [SegaSCPI](https://github.com/PatricioCoronado/SegaSCPI)
##
# NUEVAS COMPETENCIAS
**Programación de APIs en php. El fichero potenciap.php recibe cuando se envia con POST y el fichero potenciaw.php recibe cuando se envía con GET**
##
![API php](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/php.png "APIs")
## 
**Programación OTA. Util para programar el dispositivo sin acceder a el fisicamente**
##
![OTA](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/OTA.png "OTA")
## 
**Envío de variable cliente-servidor HTTP mediante POST. Mas seguro que con GET**
## 
![POST y GET](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/POST_GET.png "post y get")
![phpMyAdmin](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto6.png "phpMyAdmin en Raspberry Pi")
![Grafica](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto0.png "gráfica de consumo")
## 
**Uso de base de datos MariaDB sobre Raspberry Pi en LAN (Intentar sobre WAN en futuras versinones)**
##
![MariaDB](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/MariaDB.png "MariaDB y Raspberry Pi")
![Code](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/code.png "Visual Studio Code")
## 
# IMAGENES DEL HARDWARE
![Placa ESP32](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto7.png "placa de desarrollo")
![Bluetooth](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto3.png "placa con HC-05")
![Bobina sensora](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto4.png "sensado de corriente")
![Sistema](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto5.png "sistema completo")
