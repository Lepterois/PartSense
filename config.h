#define DEBUG //Comment to remove all debug & trace instructions


//////////////////////////////////////////////////////
///////// Replace Wifi connexion information by //////
///////// your own values                       //////
//////////////////////////////////////////////////////
const char* Wifi_SSID = "SSID";
const char* Wifi_PASSWORD = "PASSWORD";

//Thingspeak
//#define USE_THINGSPEAK //comment to remove thinkspeak data feeding
#ifdef USE_THINGSPEAK
  //////////////////////////////////////////////////////
  ///////// if you want to push data on thingspeak /////
  ///////// Replace API keys by your own values ////////
  //////////////////////////////////////////////////////
  const String ThingSpeak_PM_APIKey = "XXXXXXXXXXXXXXXX";
  const String ThingSpeak_Raw_APIKey = "XXXXXXXXXXXXXXXX";
  const char* ThingSpeak_API_Server = "api.thingspeak.com";
#endif

//MQTT
#define USE_MQTT //comment to remove MQTT data feeding
#define USE_MQTT_WITH_DOMOTICZ //comment to remove MQTT data format required for Domoticz integration for graphing
#ifdef USE_MQTT
  /*
  * if you want to push data on a MQTT broker
  * Replace settings by your own values
  * Best template to install Mosquitto on Debian: https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-8
  */
  #define MQTT_Server "MQTT server adress"
  // #define MQTT_USE_AUTH // comment to use unauthenticated MQTT connection (not yet implemented)
  #define MQTT_Username "toto"
  #define MQTT_Password "toto"
  #define MQTT_Port 1883
  #define MQTT_ClientName "PartSense-"
  
  #ifdef USE_MQTT_WITH_DOMOTICZ
  /*
   * IDX value are assigned by domoticz during dummy devices creation
   * Follow instructions on page https://www.domoticz.com/wiki/MQTT
   */
    #define MQTT_topic "domoticz/in"
    #define MQTT_temperature_humidity_idx 111 // Create a Temperature+Humidity virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_atmPM01Value_idx 0 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_atmPM25Value_idx 0 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_atmPM10Value_idx 0 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_CF1PM01Value_idx 113 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_CF1PM25Value_idx 114 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_CF1PM10Value_idx 115 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount0_3_idx 116 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount0_5_idx 117 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount1_0_idx 118 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount2_5_idx 119 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount5_0_idx 120 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_Partcount10_idx 121 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
    #define MQTT_airQualityIndex_idx 112 // Create a Custom virtual Sensor on Domoticz and get it's idx from Domoticz device list
  #else
    #define MQTT_temperature_topic "3DPrint/Enclosure1/temperature"
    #define MQTT_humidity_topic "3DPrint/Enclosure1/humidity"
    #define MQTT_atmPM01Value_topic "3DPrint/Enclosure1/atmPM01Value"
    #define MQTT_atmPM25Value_topic "3DPrint/Enclosure1/atmPM25Value"
    #define MQTT_atmPM10Value_topic "3DPrint/Enclosure1/atmPM10Value"
    #define MQTT_CF1PM01Value_topic "3DPrint/Enclosure1/CF1PM01Value"
    #define MQTT_CF1PM25Value_topic "3DPrint/Enclosure1/CF1PM25Value"
    #define MQTT_CF1PM10Value_topic "3DPrint/Enclosure1/CF1PM10Value"
    #define MQTT_Partcount0_3_topic "3DPrint/Enclosure1/Partcount0_3"
    #define MQTT_Partcount0_5_topic "3DPrint/Enclosure1/Partcount0_5"
    #define MQTT_Partcount1_0_topic "3DPrint/Enclosure1/Partcount1_0"
    #define MQTT_Partcount2_5_topic "3DPrint/Enclosure1/Partcount2_5"
    #define MQTT_Partcount5_0_topic "3DPrint/Enclosure1/Partcount5_0"
    #define MQTT_Partcount10_topic "3DPrint/Enclosure1/Partcount10"
    #define MQTT_airQualityIndex_topic "3DPrint/Enclosure1/airQualityIndex"
  #endif
#endif





