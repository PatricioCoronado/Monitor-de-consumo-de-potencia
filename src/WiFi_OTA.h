/*******************************************************************
 * Fichero donde definir los parámetros WiFi y OTA
********************************************************************/
#ifndef WIFI_OTA_CONFIG
#define WIFI_OTA_CONFIG
//#define pueblo
/**********************************************  
 * wifi client
 **********************************************/
#ifdef pueblo
  const char* ssid = "SSID1";
  const char* password = "password1"; 
#else
  const char* ssid = "SSID2";
  const char* password = "password2"; 
#endif

String IPasignada ="255.255.255.255";

/**********************************************  
 * Parámetros personalizados de OTA 
 **********************************************/
#define HOSTNAME "Monitir_de_red_01"
//the password for OTA upgrade, can set it in any char you want
#define PASSWORD "OTApassword" 
/*************************************************
 * Por si se quiere usar una IP estática  
 *************************************************/
#define USE_STATIC_IP false
#if USE_STATIC_IP
  IPAddress staticIP(192,168,1,22);
  IPAddress gateway(192,168,1,9);
  IPAddress subnet(255,255,255,0);
  IPAddress dns1(8, 8, 8, 8);
  IPAddress dns2(114,114,114,114);
#endif
/*****************************************************
 * setupOTA()
 ******************************************************/
void setupOTA()
{
  //Port defaults to 8266
  //ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(HOSTNAME);
  //Authentication
  ArduinoOTA.setPassword(PASSWORD);

  //Password can be set with it's md5 value as well
  //MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
  {
    Serial.printf("Start Updating....Type:%s\n", (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem");
  });

  ArduinoOTA.onEnd([]()
  {
    Serial.println("Update Complete!");
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    String info = "Error Info:";
    switch(error)
    {
      case OTA_AUTH_ERROR:
        info += "Auth Failed";
        Serial.println("Auth Failed");
        break;

      case OTA_BEGIN_ERROR:
        info += "Begin Failed";
        Serial.println("Begin Failed");
        break;

      case OTA_CONNECT_ERROR:
        info += "Connect Failed";
        Serial.println("Connect Failed");
        break;

      case OTA_RECEIVE_ERROR:
        info += "Receive Failed";
        Serial.println("Receive Failed");
        break;

      case OTA_END_ERROR:
        info += "End Failed";
        Serial.println("End Failed");
        break;
    }
    ESP.restart();
  });
  ArduinoOTA.begin();
}
/******************************************************
 * setup wifi
 ******************************************************/
void setupWIFI()
{
  Serial.println(String(ssid));
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  //WiFi.onEvent(WiFiEvent);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.setHostname(HOSTNAME);
  WiFi.begin(ssid, password);
  #if USE_STATIC_IP
    WiFi.config(staticIP, gateway, subnet);
  #endif
  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 20)
  {
    count ++;
    delay(1000);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected");
    IPasignada = String(WiFi.localIP());
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Connect False");
    IPasignada = "No IP";
  }
}
/******************************************************
 * [Función general] ESP32 WiFi Kit 32 
 * procesamiento de eventos
 ******************************************************/
void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event)
    {
        case SYSTEM_EVENT_WIFI_READY:               /**< ESP32 WiFi ready */
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP:
            break;
        case SYSTEM_EVENT_SCAN_DONE:                /**< ESP32 finish scanning AP */
            break;

        case SYSTEM_EVENT_STA_START:                /**< ESP32 station start */
            break;
        case SYSTEM_EVENT_STA_STOP:                 /**< ESP32 station stop */
            break;

        case SYSTEM_EVENT_STA_CONNECTED:            /**< ESP32 station connected to AP */
            break;

        case SYSTEM_EVENT_STA_DISCONNECTED:         /**< ESP32 station disconnected from AP */
            break;

        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:      /**< the auth mode of AP connected by ESP32 station changed */
            break;

        case SYSTEM_EVENT_STA_GOT_IP:               /**< ESP32 station got IP from connected AP */
        case SYSTEM_EVENT_STA_LOST_IP:              /**< ESP32 station lost IP and the IP is reset to 0 */
            break;

        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:       /**< ESP32 station wps succeeds in enrollee mode */
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:        /**< ESP32 station wps fails in enrollee mode */
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:       /**< ESP32 station wps timeout in enrollee mode */
        case SYSTEM_EVENT_STA_WPS_ER_PIN:           /**< ESP32 station wps pin code in enrollee mode */
            break;

        case SYSTEM_EVENT_AP_START:                 /**< ESP32 soft-AP start */
        case SYSTEM_EVENT_AP_STOP:                  /**< ESP32 soft-AP stop */
        case SYSTEM_EVENT_AP_STACONNECTED:          /**< a station connected to ESP32 soft-AP */
        case SYSTEM_EVENT_AP_STADISCONNECTED:       /**< a station disconnected from ESP32 soft-AP */
        case SYSTEM_EVENT_AP_PROBEREQRECVED:        /**< Receive probe request packet in soft-AP interface */
        case SYSTEM_EVENT_AP_STA_GOT_IP6:           /**< ESP32 station or ap interface v6IP addr is preferred */
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            break;

        case SYSTEM_EVENT_ETH_START:                /**< ESP32 ethernet start */
        case SYSTEM_EVENT_ETH_STOP:                 /**< ESP32 ethernet stop */
        case SYSTEM_EVENT_ETH_CONNECTED:            /**< ESP32 ethernet phy link up */
        case SYSTEM_EVENT_ETH_DISCONNECTED:         /**< ESP32 ethernet phy link down */
        case SYSTEM_EVENT_ETH_GOT_IP:               /**< ESP32 ethernet got IP from connected AP */
        case SYSTEM_EVENT_MAX:
            break;
    }
}
#endif //#ifndef WIFI_OTA_CONFIG
