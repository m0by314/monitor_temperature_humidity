# Monitoring temperature and humidity with esp32 and Thinger.io

## Summary
 - [How it works](#How-it-works)
 - [Connection](#Connection)
 - [Battery charge calculation](#Battery-charge-calculation)
 - [Thinger.io ](#Thinger.io)
 - [Setup](#Setup)
 - [Built with](#Built-with)


## How it works

The tool measures temperature and humidity every 1 hours by default. 
Data is stored on a Thinger.io data bucket and visualized from a dashboard.
An endpoint is used to send an e-mail when the battery is low 

Between each measurement, the ESP goes into deep sleep mode.  

## Connection

The dht22 is connected to pin 13 of the ESP. 
The battery holder is connected to pin 32 to calculate the battery charge.

## Battery charge calculation
A cr123 lithium battery is used. 
The maximum charge threshold is set at 3.3V and the discharge threshold at 2.9V. 
The thresholds are converted to digital values for comparison with those obtained from the adc.
The value read from the adc is converted to a percentage using the map() function.
If the percentage is less than 20%, the endpoint is triggered.

## Thinger.io

The following elements must be created on Thinger.io:  
* Device (define a device credential, which is only visible when the device is created).
* Data bucket ( select “From Device Write Call” for the data source) 
* Endpoint ( select “Email” for the endpoint type. Add custom placeholders in the destination address, reply to, subject, or email body from the endpoint call data by using {{field}} or {{}} ) 
*ESP-supplied fields: “temperature”, “humidity”, “battery” , “device_id”.*
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
* cr123 lithium battery 
* Battery holder cr123
