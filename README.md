Patricio Coronado Collado abril 2020
# DESCRIPCIÓN
 Proyecto reallizado para probar la capacidad del ESP32. de Espressif
 https://www.espressif.com/en/products/socs/esp32/overview
 Se utiliza la posibilidad ofrecida por este dispositivo
 de programarlo por OTA (mediante WiFi).
 Se conecta al PC mediante el módulo HC-05 Bluetooth.
 Lee la potencia mediante un PZEM-004T ver.3
 Lee la temperatura mediante un DHT22.
 Conecta con una base de datos instalada en una Raspberry Pi
	y le pone datos mediante POST . Se incluyen las APIs en php
	utilizadas. Se puede ver una gráfica del consumo de potencia
	y temperatura.

# DEPENDENCIAS
Librerias Arduino (incluidas en el proyecto):
DHTesp, HTTPClientESP32,PZEM-004T-v30-master,SegaSCPI
#
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto0.png "perfil de consumo")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto7.png "placa de desarrollo")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto8.png "ESP32")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto3.png "placa con HC-05")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto4.png "sensado de corriente")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto5.png "sistema completo")
![Alt text](https://github.com/PatricioCoronado/Monitor-de-consumo-de-potencia/blob/master/ficheros/foto6.png "phpMyAdmin en Raspberry Pi")
