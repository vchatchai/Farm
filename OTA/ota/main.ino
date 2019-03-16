

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nBooting...");

  Serial.println("\r\nSet Value PinMode.");
  valveSetup();
  // initialize digital pin LED as an output.
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);

  dhtSetup();

  pushButtonSetup();
  setup_wifi();


OTASetup();

  Serial.println("Configuring MQTT server...");
  mqtt_client_id = ESP.getChipId();
  humidity_topic = humidity_topic + ESP.getChipId();
  temperature_topic = temperature_topic + ESP.getChipId();
  mqttSetup();
  Serial.printf("   Server IP: %s\r\n", mqtt_server);
  Serial.printf("   Username:  %s\r\n", mqtt_user);
  Serial.println("   Cliend Id: " + mqtt_client_id);
  Serial.println("   MQTT configured!");

  Serial.println("Setup completed! Running app...");
  //Initial completed.
  //LED_POWER ON
  digitalWrite(LED_POWER, LOW);
}

void loop() {

  ArduinoOTA.handle();

  mqttLoop();
  dhtLoop();
  pushButtonLoop();
}

