/**********************************************************************
 * proyecto: Monitor de red eléctrica
 * Escribe en una base de datos variables de la red eléctrica
 * aplicación: 
 * plataforma: ESP32
 * sensor PZEM-004T
 * Patricio Coronado
 * Abril de 2020
 * modificado 
 * Los datos se envian a una base de datos en una Raspberry pi 
 * En la red local.
  *********************************************************************/
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include "HTTPClientESP32.h"//Librería local para http
#include "soc/soc.h"//for disable brownout detector
#include "soc/rtc_cntl_reg.h"//for disable brownout detector
#include "SegaSCPI.h"
#include "WiFi_OTA.h"
#include "PZEM004Tv30.h"
/**********************************************************************
  Prototipos de funciones
***********************************************************************/
void escribe_base_de_datos(void);
void lee_potencia(void);
/**********************************************************************
  Prototipos de funciones y menú scpi
***********************************************************************/
//Funciones scpi comunes a todos los sistemas
void errorSCPI(void);
void opcSCPI(void);
void idnSCPI(void);
void clsSCPI(void);
// Funciones particulares de nuestro sistema
void scpi_envia_datos(void);
void scpi_envia_IP(void);
void scpi_cambia_timer(void);
void scpi_depuracion_on(void);
void scpi_depuracion_off(void);
void scpi_activa_proceso(void);
void scpi_desactiva_proceso(void);
void scpi_escribe_BD(void);
void scpi_reset(void);
void scpi_contadores(void);
MENU_SCPI  //menú de  comandos y submenús OBLIGATORIO
{
	SCPI_COMANDO(TIMER,TI,scpi_cambia_timer) //Envía la IP actual
  SCPI_COMANDO(IP,IP,scpi_envia_IP) //Envía la IP actual
	SCPI_COMANDO(DATOSRED,DR,scpi_envia_datos) //Comando que ejecuta la función funcion2
  SCPI_COMANDO(DEPURACIONON,DON,scpi_depuracion_on) //Pone a true la depuración
  SCPI_COMANDO(DEPURACIONOFF,DOFF,scpi_depuracion_off) //Pone false la depuracion
  SCPI_COMANDO(DESACTIVA,DES,scpi_desactiva_proceso)//Para que no envie datos la la base de datos
  SCPI_COMANDO(ACTIVA,ACT,scpi_activa_proceso)//Para que envie datos la la base de datos
  SCPI_COMANDO(ESCRIBEBD,EBD,scpi_escribe_BD)//Pone un dato en la base de datos
	SCPI_COMANDO(RESET,RST,scpi_reset)
  SCPI_COMANDO(CONTADORES,CON,scpi_contadores)
  // TO DO Añadir aquí comandos o submenús
	//Comandos de funciones propias de SegaSCPI 
	SCPI_COMANDO(ERROR,ERR,errorSCPI)// Envía el ultimo error
  SCPI_COMANDO(*IDN,*IDN,idnSCPI)// Identifica el instrumento
	SCPI_COMANDO(*OPC,*OPC,opcSCPI)// Devuelve un 1 al PC
	SCPI_COMANDO(*CLS,*CLS,clsSCPI)// Borra la pila de errores
};
tipoNivel Raiz[]= SCPI_RAIZ 
String misErrores[]=
{//Los errores de 0 a 6 son de scpi (No se usan)
	"7 mi error 7",
	"8 mi error 8",
};
/**********************************************************************
    Defines y constantes
***********************************************************************/
#define LED_BUILTIN 2
#define debug if(depuracion)Serial1.printf 
#define puerto segaScpi.PuertoActual //Para referirse al puerto scpi
/**********************************************************************
    Variables globales
***********************************************************************/
int Timer,TiempoActual,TiempoAnterior; //en milisegundos
#define TIMER_MAX 300 
#define TIMER_MIN 10 //Mínimo 10 segundos
#define VALOR_TIMER 30000
float voltage,current,power,energy,frequency,pfactor;
bool depuracion=true;
bool ProcesoActivado=true;
unsigned int ContadorExitos=0;
unsigned int ContadorFracasos=0;
SegaSCPI segaScpi(Raiz,"Monitor de red",misErrores);//Instanciamos el objeto SCPI	global
PZEM004Tv30 pzem(&Serial2);//Serial2 at pins IO-16 (RX2) and IO-17 (TX2)
char web[256]; //URL para enviar el GET
/**********************************************************************
 					setup
**********************************************************************/
void setup() 
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //desabilita brownout
  pinMode(LED_BUILTIN,2);
  Serial.begin(115200);
  //Puerto bluetooth para depuración
  Serial1.begin(115200, SERIAL_8N1, 22, 23);//RXD2 pin 22,TXD2 pin 23
  //Inicializa WiFi y OTA
  setupWIFI();//Conecta a la WiFi
  setupOTA();//Para flasear por OTA
  TiempoActual=TiempoAnterior=0;
  Timer=VALOR_TIMER;
}
void loop() 
{
  // Si recibe algo por el puerto Serial lo procesa con SegaSCPI 
  if (Serial.available()){segaScpi.scpi(&Serial);}
  // Si recibe algo por el puerto Serial1 lo procesa con SegaSCPI 
  if (Serial1.available()){segaScpi.scpi(&Serial1);}
  // Ejecuta comando de flash poo OTA si se solicita
  ArduinoOTA.handle();//Ejecuta OTA
  TiempoActual = millis();
  // Si ha transcurrido el tiempo "Timer" lee datos y los envía a la base de da
  if(((TiempoActual-TiempoAnterior) > Timer) && ProcesoActivado)
  {
    debug("cominezo....\n");
    lee_potencia();
    escribe_base_de_datos();
    TiempoAnterior=millis();
  }
}
/****************************************************************
	funcion scpi: para leer todos los datos del sensor y enviarlos
  por el puerto serie
*****************************************************************/
void scpi_envia_datos(void)
{
  voltage = pzem.voltage();
  if(isnan(voltage)) {voltage=-1;}
  current = pzem.current();
  if(isnan(current)){current=-1;}
  power = pzem.power();
  if(isnan(power)){power=-1;}
  energy = pzem.energy();
  if(isnan(energy)){energy=-1;}
  frequency = pzem.frequency();
  if(isnan(frequency)){frequency=-1;}
  pfactor = pzem.pf();
  if(isnan(pfactor)){pfactor=-1;}
  if(voltage==-1 || current==-1 || power==-1 || energy==-1 || frequency==-1 || pfactor==-1)
  puerto->println("DAT error en la lectura");
  else
  puerto->printf("DAT voltaje=%.1f corriente=%.2f potencia=%.2f energia=%.2f frecuencia=%.1f factor de portencia=%.3f\r\n\n",voltage,current,power,energy,frequency,pfactor);
 }
  /************************************************************************
    funcion scpi: Cambia el tiempo de envío de datos a la base de datos
    El timepo se cambia en segundos
 *************************************************************************/
  void scpi_cambia_timer(void)
  {
      int timer;
      timer=Timer/1000;//Pasa a segundos
      segaScpi.actualizaVarEntera(&timer,TIMER_MAX,TIMER_MIN);//timer es en segundos
      Timer=timer*1000;//Pasa a milisegundos
  }
  /************************************************************************
    funciones scpi: Actuan sobre la variable bool depuracion
 *************************************************************************/
void scpi_depuracion_off(void) 
{
  if(segaScpi.FinComando[0]=='?'){puerto->println(depuracion);}
  else {depuracion=false;}
}
void scpi_depuracion_on(void) 
{
  if(segaScpi.FinComando[0]=='?'){puerto->println(depuracion);}
   else {depuracion=true;}
}
 /************************************************************************
    funciones scpi: Actuan sobre la variable bool ProcesoActivado
 *************************************************************************/
void scpi_activa_proceso(void) 
{
  if(segaScpi.FinComando[0]=='?'){puerto->println(ProcesoActivado);}
  else {ProcesoActivado=true;}
}
void scpi_desactiva_proceso(void)
{
  if(segaScpi.FinComando[0]=='?'){puerto->println(ProcesoActivado);}
  else{ProcesoActivado=false;}
}
 /************************************************************************
    funcion scpi: Envía la IP actual
 *************************************************************************/
 void scpi_envia_IP(void) { puerto->println(WiFi.localIP()); }
 /************************************************************************
    funcion scpi: que provoca una lectura de potencia y escritura el la 
    base de datos
 *************************************************************************/
 void scpi_escribe_BD(void) { lee_potencia();escribe_base_de_datos();}
/************************************************************************
    Funciones scpi comunes a todos los sistemas
 *************************************************************************/
 /************************************************************************
    Función del Comando: ERROR ó ERR
    Envia por el puerto el último error registrado por SEGAINVEX-SCPI
 *************************************************************************/
void errorSCPI(void){segaScpi.errorscpi(0);}
/*************************************************************************
  Función del Comando: *IDN"
   Envia por el puerto una cadena que identifica al sistema
 *************************************************************************/
void idnSCPI(void){segaScpi.enviarNombreDelSistema();}
 /************************************************************************
  Función del Comando:*OPC
  Envia por el puerto el carácter uno
 *************************************************************************/
void opcSCPI(void){	segaScpi.PuertoActual->println("1");}
/*************************************************************************
    Comando: CLS
    Limpia la pila de errores de SEGAINVEX-SCPI
 *************************************************************************/
void clsSCPI(void){segaScpi.errorscpi(-1);}
/*************************************************************************
    Comando: RST
    Resetea el ESP32
 *************************************************************************/
void scpi_reset(void)
{
  ESP.restart();
}
/*************************************************************************
    Comando: CON
    Envía al PC los contadores de éxitos y fracasos
 *************************************************************************/
void scpi_contadores(void)
{
  puerto->printf("Exitos=%u \rFracasos=%u\r\n",ContadorExitos,ContadorFracasos);
}
/*************************************************************************
    Función que lee la potencia del sensor
*************************************************************************/
void lee_potencia(void)
{
  power = pzem.power();
  if(isnan(power)){power=-1;}
}
/*************************************************************************
    Función que pone datos en la base de datos
*************************************************************************/
void escribe_base_de_datos(void)
{
  int httpCode;
  digitalWrite(LED_BUILTIN,HIGH);
  if(WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http; //
    //URL de entrada a la base de datos
    sprintf(web,"http://192.168.1.102/potencia.php/?valor1=%.1f",power);//Rellena el string "web"
    debug("%s\r\n",web);
    debug("...enviando la solicitud al servidor.....\r\n");
    http.begin(web); //inicia la conexión
    httpCode = http.GET();//Envía dato. HttpCode es el código recibido desde el servidos
    debug("httpCode=%u\r\n",httpCode);
    //Aqui se gestina la recepción de la página solicitada
    if(httpCode > 0 ) //Si conecta correctamente
    {
      debug("exito en el intento de conexion con la URL!\r\n");
      if(httpCode == HTTP_CODE_OK) 
        {//Si recibe una página...
            String payload = http.getString(); //payload contendrá #OK
            //debug.println(payload);
            if (payload.indexOf("#OK")!=-1) 
            {
              debug("#0K\r\n");ContadorExitos++;
            }
            else 
            {
              debug("#ERROR\r\n");ContadorFracasos++;
            }
        }
    }
    else 
    {
      debug("fallo el intento de conexion con la URL!\r\n");
      ContadorFracasos++;
    }
    http.end();//Cierra la conexión
  }
  else 
  {
    scpi_reset();
    debug("se ha ejecutado un reset\r por desconexión del WiFi!\r\n");
  }
  digitalWrite(LED_BUILTIN ,LOW);
}
/*******************************FIN***************************************/