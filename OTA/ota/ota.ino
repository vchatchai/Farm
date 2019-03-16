#include <TransmissionResult.h>
#include <NetworkInfo.h>
#include <ESP8266WiFiMesh.h>
#include <TypeConversionFunctions.h>

#include <Hash.h>

#include <ESP8266WiFi.h>  //For ESP8266
#include <ESP8266mDNS.h>  //For OTA
#include <WiFiUdp.h>      //For OTA
#include <ArduinoOTA.h>   //For OTA
//WIFI configuration
#define wifi_ssid "Chatchai"
#define wifi_password "manager1"

//MQTT configuration
#define mqtt_server "m11.cloudmqtt.com"
#define mqtt_port 11621
#define mqtt_user "kysbvegn"
#define mqtt_password "Ww2mmbIEU9_e"
String mqtt_client_id = "ESP8266-"; //This text is concatenated with ChipId to get unique client_id
//MQTT Topic configuration

String root = "/";
String farm = root+"farm_home";
String humidity_topic = farm + "/sensor/humidity/" ;
String temperature_topic = farm + "/sensor/temperature/" ;
String valve_topic = farm + "/valve/";
String pump_topic = farm + "/pump/";



//PIN BUTTON
#define BUTTON_PIN 5
//DHT SENSOR
#define DHTPIN  4

//LED PORT
#define LED_STATUS 0
#define LED_POWER 2


int ledState = LOW;

//Necesary to make Arduino Software autodetect OTA device
WiFiServer TelnetServer(8266);

void OTASetup(){

  Serial.print("Configuring OTA device...");
  TelnetServer.begin();   //Necesary to make Arduino Software autodetect OTA device
  ArduinoOTA.onStart([]() {
    Serial.println("OTA starting...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA update finished!");
    Serial.println("Rebooting...");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA in progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OK");

}
