#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <AdafruitIO_WiFi.h>
#include <DHT.h>
#include <DHT_U.h>
#include "../config.h"


// Declaration of objects
DHT_Unified dht(DHTPIN, DHTTYPE);
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PWD);  // Set up the adafruit WiFi client.
AdafruitIO_Feed *humidity = io.feed(IO_FEED_HUMIDITY); // Set up the feed for humidity.
AdafruitIO_Feed *temperature = io.feed(IO_FEED_TEMP); // Set up the feed for temperature.


void setup() {
 
  Serial.begin(115200);

  esp_sleep_enable_timer_wakeup(TIME_TO_SECOND * uS_TO_S_FACTOR);  // Timer for the wakeup of the ESP

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  int timeout = 10;
  while(io.status() < AIO_CONNECTED && (timeout-- > 0)) {
    Serial.print(".");
    sleep(1);
  }
  if (io.status() < AIO_CONNECTED) {
    Serial.println();
    Serial.println("ERROR: Adafruit IO fail connection");
    esp_deep_sleep_start();
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  
  // start dht sensor
  dht.begin();

  // Initialize sensor for temperature and humidity
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  
  delay(2000); 
}
 
void loop() {

  sensors_event_t event;

  // Get temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Send data to the feed.
  temperature->save(event.temperature);

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
  // Send data to the feed.
  humidity->save(event.relative_humidity);
 

  Serial.println("Going to sleep ...\n");
  
  delay(2000);

  esp_deep_sleep_start();
}