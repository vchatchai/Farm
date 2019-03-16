
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