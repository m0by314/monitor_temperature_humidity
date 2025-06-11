# Monitoring temperature and humidity with esp32 and Thinger.io

## Summary
 - [How it works](#How-it-works)
 - [Connection](#Connection)
 - [Thinger.io ](#Thinger.io)
 - [Setup](#Setup)
 - [Built with](#Built-with)


## How it works

The tool measures temperature and humidity every 1 hours by default. 
Data is stored on a Thinger.io data bucket and visualized from a dashboard.

Between each measurement, the ESP goes into deep sleep mode.  

## Connection

The dht22 is connected to pin 13 of the ESP. 

## Thinger.io

The following elements must be created on Thinger.io:  
* Device (define a device credential, which is only visible when the device is created).
* Data bucket ( select “From Device Write Call” for the data source) 
* Dashboards to view data the way you like it 


## Setup 


* First copy the template:  
`cp arduino_secret.h.template arduino_secret.h`

* Configure the following variables with your secrets:  
```
/* ------- Thinger.io ------- */
#define THINGER_USERNAME "your Thinger.io username"
#define THINGER_DEVICE_ID "your device id"
#define THINGER_DEVICE_CREDENTIAL "your device credential"

#define THINGER_BUCKET_ID "your bucket id"
#define THINGER_ENDPOINT_IDENTIFIER "your endpoint identifier"

/* ------- WIFI ------- */
#define WIFI_SSID "your wifi name"
#define WIFI_PASSWORD "your wifi password"
```

## Built with

* Kit AZ-Delivery Dev Kit C  
* Sensor DHT22
