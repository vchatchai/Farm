#include <PubSubClient.h> //For MQTT



//MQTT client
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
void mqttSetup(){

  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(callback);
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
      String subscribe_path = valve_topic +  ESP.getChipId();
      // Length (with one extra character for the null terminator)
      int str_len = subscribe_path.length() + 1;

      // Prepare the character array (the buffer)
      char char_array[str_len];

      // Copy it over
      subscribe_path.toCharArray(char_array, str_len);

      Serial.print("subscribe: ");
      Serial.println(char_array);
      mqtt_client.subscribe(char_array);
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

void mqttLoop(){ 

  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();
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
  if (topic == valve_topic + ESP.getChipId()) {
    Serial.print("Changing Valve to ");
    if (messageTemp == "on") {
      valveOn();
    } else if (messageTemp == "off") {
      valveOff();
    }
  }else if (topic == pump_topic + ESP.getChipId()) {
    Serial.print("Changing Valve to ");
    if (messageTemp == "on") {
      pumpOn();
    } else if (messageTemp == "off") {
      pumpOff();
    }
  }
  Serial.println();
}
