#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
*  DHT
*/
#define DHTPIN 2       /* sensor pin */
#define DHTTYPE DHT22  /* type of sensor used */

/* 
*   Sleep Time 
*/
#define HOUR_TO_SLEEP 4ULL                        /* Sleep time between each measurement  */
#define uS_TO_S_FACTOR 1000000                    /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SECOND (HOUR_TO_SLEEP * 60 * 60)  /* Time ESP32 will go to sleep (in seconds) */


/* 
*   WIFI Setting
*/
#define WIFI_SSID "YOUR_WIFI_SSID"     /* Replace with your wifi ssid. */
#define WIFI_PWD "YOUR_WIFI_PASSWORD"  /* Replace with your wifi password */

/* 
*   Adafruit IO settings 
*/
#define IO_USERNAME "YOUR ADAFRUIT USERNAME"     /* Replace with your adafruit username. */
#define IO_KEY "YOUR ADAFRUIT KEY"               /* Replace with your adafruit key.      */
#define IO_FEED_HUMIDITY "HUMIDITY FEED NAME"    /* Replace with your adafruit humidity feed name. */
#define IO_FEED_TEMP "TEMPERATURE FEED NAME"     /* Replace with your adafruit humidity feed name. */

#endif 