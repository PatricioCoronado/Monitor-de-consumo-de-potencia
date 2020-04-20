Patricio Coronado Collado abril 2020
Monitor de las variables de consumo de la linea de red eléctrica de un equipo o electrodoméstico.
Programado con un ESP32 y con un sensor PZEM-004T V3.0
El ESP32 está conectado a un Bluetooth HC-05 para conectar con el PC. (no utilizo el Bluetooth del ESP).
El ESP32 lee las variables de consumo, se conecta la la red LAN del router doméstico y pone los datos
en una base de datos en una RaspberryPi conectada en la misma LAN.
