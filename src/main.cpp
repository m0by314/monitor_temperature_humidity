#include <DHT_U.h>
#include <ThingerESP32.h>

#include "../arduino_secret.h"

/* Sleep Time */
#define HOUR_TO_SLEEP 1ULL                        /* Sleep time between each measurement  */
#define uS_TO_S_FACTOR 1000000                    /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP (HOUR_TO_SLEEP * 60 * 60)   /* Time ESP32 will go to sleep (in seconds) */

/* DHT */
#define DHT_PIN 13
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

ThingerESP32 thing(THINGER_USERNAME, THINGER_DEVICE_ID, THINGER_DEVICE_CREDENTIAL);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(1500);
  
  // Connect WIFI
  thing.add_wifi(WIFI_SSID, WIFI_PASSWORD);
  delay(2000);
  
  // Start DHT22
  dht.begin();
  
  // Create data structure
  thing["data"] >> [](pson& in){ 
    in["temperature"] = dht.readTemperature();
    in["humidity"] = dht.readHumidity();
    in["device_id"] = THINGER_DEVICE_ID;
  };

  // Write Data
  thing.handle();
  thing.write_bucket(THINGER_BUCKET_ID, "data");
  delay(500);
  thing.stop();
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // Timer for the wakeup of the ESP
  esp_deep_sleep_start();
  
}

void loop() {}
