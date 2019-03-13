#include <TransmissionResult.h>
#include <NetworkInfo.h>
#include <ESP8266WiFiMesh.h>
#include <TypeConversionFunctions.h>

#include <Hash.h>

#include <ESP8266WiFi.h>  //For ESP8266
#include <PubSubClient.h> //For MQTT
#include <ESP8266mDNS.h>  //For OTA
#include <WiFiUdp.h>      //For OTA
#include <ArduinoOTA.h>   //For OTA
#include <Bounce2.h>
#include <DHT.h>
//WIFI configuration
#define wifi_ssid "Home"
#define wifi_password "manager1"

//MQTT configuration
#define mqtt_server "m11.cloudmqtt.com"
#define mqtt_user "kysbvegn"
#define mqtt_password "Ww2mmbIEU9_e"
String mqtt_client_id = "ESP8266-"; //This text is concatenated with ChipId to get unique client_id
//MQTT Topic configuration
String mqtt_base_topic = "/sensor/" + mqtt_client_id + "/data";
#define humidity_topic "/humidity"
#define temperature_topic "/temperature"
#define valve_topic "/valve"

#define DHTTYPE DHT22
#define DHTPIN  4
//Motor Connections
//Change this if you wish to use another diagram
#define EnA  10
#define EnB  5
#define In1  9
#define In2  8
#define In3  7
#define In4  6

//LED PORT
#define LED_POWER 2
#define LED_STATUS 3

//PIN BUTTON
#define BUTTON_PIN 4
int ledState = LOW;
//MQTT client
WiFiClient espClient;
PubSubClient mqtt_client(espClient);

//Necesary to make Arduino Software autodetect OTA device
WiFiServer TelnetServer(8266);


Bounce debouncer = Bounce(); // สร้าง debouncer object

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    
    //LED_POWER ON
    digitalWrite(LED_POWER, LOW); 
    delay(500);
    //LED_POWER OFF
    digitalWrite(LED_POWER, HIGH); 
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.print("   IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nBooting...");

  Serial.println("\r\nSet Value PinMode.");
  // All motor control pins are outputs
//  pinMode(EnA, OUTPUT);
//  pinMode(EnB, OUTPUT);
//  pinMode(In1, OUTPUT);
//  pinMode(In2, OUTPUT);
//  pinMode(In3, OUTPUT);
//  pinMode(In4, OUTPUT);

  // initialize digital pin LED as an output.
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);

 debouncer.attach(BUTTON_PIN, INPUT_PULLUP);
 debouncer.interval(25); // กำหนดเวลาการเปลี่ยนสถานะให้กับ debouncer object ที่ 25 มิลลิวินาที

  setup_wifi();

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

  Serial.println("Configuring MQTT server...");
  mqtt_client_id = mqtt_client_id + ESP.getChipId();
  mqtt_base_topic = "/sensor/" + mqtt_client_id + "/data";
  mqtt_client.setServer(mqtt_server, 11621);
  
  mqtt_client.setCallback(callback);
  Serial.printf("   Server IP: %s\r\n", mqtt_server);
  Serial.printf("   Username:  %s\r\n", mqtt_user);
  Serial.println("   Cliend Id: " + mqtt_client_id);
  Serial.println("   MQTT configured!");

  Serial.println("Setup completed! Running app...");

  //Initial completed.
  //LED_POWER ON
  digitalWrite(LED_POWER, LOW); 
}


void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (mqtt_client.connect(mqtt_client_id.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
      
      mqtt_client.subscribe("/sensor/ESP8266-11648356/data/temperature");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
    
      // Wait 5 seconds before retrying
      
      //LED_POWER ON
      digitalWrite(LED_POWER, LOW); 
      delay(2500);
      //LED_POWER OFF
      digitalWrite(LED_POWER, HIGH); 
      delay(2500);
      
    }
  }
}


bool checkBound(float newValue, float prevValue, float maxDiff) {
  return (true);
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
//  if(topic=="room/lamp"){
//      Serial.print("Changing Room lamp to ");
//      if(messageTemp == "on"){
//          valveOn();
//        Serial.print("On");
//      }
//      else if(messageTemp == "off"){
//          valveOff();
//        Serial.print("Off");
//      }
//  }
  Serial.println();
}


void valveOn()   
{
  // turn on motor A
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  // set speed to 150 out 255
  //  analogWrite(EnA, 200);
  // turn on motor B
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
  // set speed to 150 out 255
  //  analogWrite(EnB, 200);
  delay(500);
  // now turn off motors
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  
  //LED_STATUS ON
  ledState = 1;
  digitalWrite(LED_STATUS, !ledState); 
}

void valveOff()   
{
  // turn on motor A
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  // set speed to 150 out 255
  //  analogWrite(EnA, 200);
  // turn on motor B
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
  // set speed to 150 out 255
  //  analogWrite(EnB, 200);
  delay(500);
  // now turn off motors
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  //LED_STATUS ON
  ledState = 0;
  digitalWrite(LED_STATUS, !ledState); 
}

void pushButton(){
     debouncer.update(); // อัปเดตสถานะให้กับ debouncer object
   
   // กำหนดเงื่อนไขให้โค้ดโปรแกรมในวงเล็บปีกกาทำงานเมื่อสถานะปุ่มกดเปลี่ยนจาก HIGH เป็น LOW โดยเช็คจากฟังก์ชั่น fell()
   // หากต้องการเช็คสถานะจาก LOW เป็น HIGH ให้แทนที่ฟังก์ชั่น fell() ด้วยฟังก์ชั่น rose()
   if ( debouncer.fell() ) { 
      ledState = !ledState; // สลับสถานะติด/ดับของ LED
      mqtt_client.publish((mqtt_base_topic + valve_topic).c_str(), String(ledState).c_str(), true);
   }
}


long now = 0; //in ms
long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
int min_timeout = 2000; //in ms

void loop() {

  ArduinoOTA.handle();

  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();

  now = millis();
  if (now - lastMsg > min_timeout) {
    lastMsg = now;
    now = millis();
    float newTemp = temp + 2; //hdc.readTemperature();
    float newHum = hum + 0.5; //hdc.readHumidity();

//    float newHum = dht.readHumidity();
    // Read temperature as Celsius (the default)
//    float newTemp = dht.readTemperature();

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("Sent ");
      Serial.print(String(temp).c_str());
      Serial.println(" to " + mqtt_base_topic + temperature_topic);
      mqtt_client.publish((mqtt_base_topic + temperature_topic).c_str(), String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("Sent ");
      Serial.print(String(hum).c_str());
      Serial.println(" to " + mqtt_base_topic + humidity_topic);
      mqtt_client.publish((mqtt_base_topic + humidity_topic).c_str(), String(hum).c_str(), true);
    }
  }
  pushButton();
}
