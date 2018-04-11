#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
//const char* ssid = "Chatchai";
const char *password = "manager1";

// Config MQTT Server
#define mqtt_server "test.mosquitto.org"
#define mqtt_port 1883
#define mqtt_user "TEST"
#define mqtt_password "12345"

#define LED_PIN 2

byte mac[6];
char str_mac[6];
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  listNetworks();

  String ssidString = WiFi.SSID(4);
  char ssid[ssidString.length() + 1];
  ssidString.toCharArray(ssid, ssidString.length() + 1);

  //  Serial.println();
  //  Serial.print("Connecting to ");
  //  Serial.println(ssid);
  //
  //  WiFi.begin(ssid, password);
  //
  //  while (WiFi.status() != WL_CONNECTED) {
  //    delay(500);
  //    Serial.print(".");
  //  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("MAC address: ");
  WiFi.macAddress(mac);

  array_to_string(mac, 6, str_mac);

  Serial.println(str_mac);
  Serial.println(WiFi.macAddress());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
  {

    listNetworks();
    delay(5000);

    Serial.println("Attempting MQTT connection...");

    //    array_to_string(str_mac,6, mac);
    if (client.connect(str_mac))
    {
      Serial.println("connected");
      client.subscribe("/ESP/LED");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  client.loop();
}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
    buffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
  }
  buffer[len * 2] = '\0';
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  int i = 0;
  while (i < length)
    msg += (char)payload[i++];
  if (msg == "GET")
  {
    client.publish("/ESP/LED", (digitalRead(LED_PIN) ? "LEDON" : "LEDOFF"));
    Serial.println("Send !");
    return;
  }

  // put your setup code here, to run once:
  //char json[] = "{\"address\":\"18FE34DF0F31\",\"LED\":\"ON\"}";
  //msg must be {"address":"18FE34DF0F31","LED":"ON"}

  Serial.print("\nJSONMessage");
  Serial.println(msg);
  StaticJsonBuffer<512> jsonBuffer;

  JsonObject &root = jsonBuffer.parseObject(msg);

  const char *address = root["address"];
  //const char* led   root["LED"];
  //double latitude    = root["data"][0];
  //double longitude   = root["data"][1];

  Serial.print("\naddress:");
  Serial.println(address);
  String s = String(address);
  if (s.equals(str_mac))
  {
    Serial.println("address is equal");

    const char *led = root["LED"];
    String statusLed = String(led);
    if (statusLed.equals("ON"))
    {
      digitalWrite(LED_PIN, LOW);
    }
    else if (statusLed.equals("OFF"))
    {
      digitalWrite(LED_PIN, HIGH);
    }

    //  digitalWrite(LED_PIN, (msg == "LEDON" ? HIGH : LOW));
  }
  else
  {
    Serial.println("address is not equal");
  }
  Serial.println(address);
  Serial.println(str_mac);
  Serial.println(msg);
}

void listNetworks()
{
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println("Couldn't get a wifi connection");
    while (true)
      ;
  }

  // print the list of networks seen:
  Serial.print("number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++)
  {

    if (WiFi.status() == WL_CONNECTED)
    {
      return;
    }

    Serial.print(thisNet);
    Serial.print(") ");
    String ssidString = WiFi.SSID(thisNet);
    char ssid[ssidString.length() + 1];
    ssidString.toCharArray(ssid, ssidString.length() + 1);
    Serial.print(ssid);
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.println("\tEncryption: ");

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    delay(10000);
    Serial.print(".");

    //    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}
