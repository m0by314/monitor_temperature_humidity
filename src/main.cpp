#include <DHT_U.h>
#include <ThingerESP32.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "../arduino_secret.h"

/* Sleep Time */
#define HOUR_TO_SLEEP 2ULL                        /* Sleep time between each measurement  */
#define uS_TO_S_FACTOR 1000000                    /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP (HOUR_TO_SLEEP * 60 * 60)   /* Time ESP32 will go to sleep (in seconds) */

/* DHT */
#define DHT_PIN 13
#define DHT_TYPE DHT22

/*  ----------- ADC ------------ 
    ADC1_CHANNEL_0 = 0,  ADC1 channel 0 is GPIO36 
    ADC1_CHANNEL_1,      ADC1 channel 1 is GPIO37 
    ADC1_CHANNEL_2,      ADC1 channel 2 is GPIO38 
    ADC1_CHANNEL_3,      ADC1 channel 3 is GPIO39 
    ADC1_CHANNEL_4,      ADC1 channel 4 is GPIO32 
    ADC1_CHANNEL_5,      ADC1 channel 5 is GPIO33 
    ADC1_CHANNEL_6,      ADC1 channel 6 is GPIO34 
    ADC1_CHANNEL_7,      ADC1 channel 7 is GPIO35 
*/
#define ADC_PIN 32
#define ADC_CHANNEL ADC1_CHANNEL_4
#define ADC_BITS  12                     // 12 bits for esp32
#define ADC_RAW_MAX  (pow(2,ADC_BITS) - 1)  // Maximum of the output ADC raw digital reading result, which is 4095 under Single Read mode, 4095 under Continuous Read mode.

/* ----- Battery threshold --------- */
#define T_BATTERY_MAX  3.3
#define T_BATTERY_MIN  2.9

// Convert battery threshold to digital value
#define BATTERY_MAX_RAW (T_BATTERY_MAX / (T_BATTERY_MAX / ADC_RAW_MAX)) 
#define BATTERY_MIN_RAW (T_BATTERY_MIN / (T_BATTERY_MAX / ADC_RAW_MAX))

/* ----- Battery endpoint threshold -------------- */
#define BATTERY_ENDPOINT_THRESOLD 20.0


DHT dht(DHT_PIN, DHT_TYPE);
ThingerESP32 thing(THINGER_USERNAME, THINGER_DEVICE_ID, THINGER_DEVICE_CREDENTIAL);

float battery_percent;
int endpointUpperFlag = 1;

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(1500);
  
  // Connect WIFI
  thing.add_wifi(WIFI_SSID, WIFI_PASSWORD);
  delay(2000);
  
  // Start DHT22
  dht.begin();
  
  // ADC Read 
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);  // ADC Attenuation ADC_ATTEN_DB_11 --> 150 ~ 2450 (mv) the higher the attenuation is, the higher the measurable input voltage could be
  int adc_val = analogRead(ADC_PIN);

  // Calculate battery percent
  battery_percent = map(adc_val, BATTERY_MIN_RAW , BATTERY_MAX_RAW, 0 ,100);

  // Create data structure
  thing["data"] >> [](pson& in){ 
    in["temperature"] = dht.readTemperature();
    in["humidity"] = dht.readHumidity();
    in["battery"] = battery_percent;
    in["device_id"] = THINGER_DEVICE_ID;
  };

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);  // Timer for the wakeup of the ESP
  
}

void loop() {
  thing.handle();
  thing.write_bucket(THINGER_BUCKET_ID, "data");
  
  if(battery_percent <= BATTERY_ENDPOINT_THRESOLD && endpointUpperFlag){  
    thing.call_endpoint(THINGER_ENDPOINT_IDENTIFIER, thing["data"]);
    endpointUpperFlag=0;
  }

  Serial.println(battery_percent);
  delay(500);
  esp_deep_sleep_start();
}
