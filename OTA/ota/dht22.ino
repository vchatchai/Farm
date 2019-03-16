#include <DHT.h>
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

long now = 0; //in ms
long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
int min_timeout = 2000; //in ms

void dhtSetup() {
  dht.begin();
}

bool checkBound(float newValue, float prevValue, float maxDiff) {
  return (true);
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

void dhtLoop() {

  now = millis();
  if (now - lastMsg > min_timeout) {
    lastMsg = now;
    now = millis();
    //    float newTemp = temp + 2; //hdc.readTemperature();
    //    float newHum = hum + 0.5; //hdc.readHumidity();

    float newHum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float newTemp = dht.readTemperature();

    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("Sent ");
      Serial.print(String(temp).c_str());
      Serial.println(" to " + temperature_topic);
      mqtt_client.publish((temperature_topic).c_str(), String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("Sent ");
      Serial.print(String(hum).c_str());
      Serial.println(" to " +  humidity_topic );
      mqtt_client.publish((humidity_topic).c_str(), String(hum).c_str(), true);
    }
  }
}
