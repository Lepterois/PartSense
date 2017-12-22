#define DEBUG //Comment to remove all debug & trace instructions


//////////////////////////////////////////////////////
///////// Replace Wifi connexion information by //////
///////// your own values                       //////
//////////////////////////////////////////////////////
const char* Wifi_SSID = "SSID";
const char* Wifi_PASSWORD = "PASSWORD";

// Ajustement TO et humidité
float adjustT0 = -1.9;// remplacer par la valeur de correction
float adjustHumidity = +11.3;// remplacer par la valeur de correction

//Thingspeak
#define USE_THINGSPEAK //comment to remove thingspeak data feeding
#ifdef USE_THINGSPEAK
  //////////////////////////////////////////////////////
  ///////// if you want to push data on thingspeak /////
  ///////// Replace API keys by your own values ////////
  //////////////////////////////////////////////////////
  const char* ThingSpeak_API_Server = "api.thingspeak.com";
  /*  Create or use a ThingSpeak account. Data are sent only every minute, so the free plan must be enough.
   *  Create two channels with the following fields collection
   *  The names of the field are not important, but the order of the fields should be defined as below

  // PM Channel:
  // Field1= CF1 PM 1.0 (μg/m3)
  // Field2= CF1 PM 2.5 (μg/m3)
  // Field3= CF1 PM 10 (μg/m3)
  // Field4= Atm PM 1.0 (μg/m3)
  // Field5= Atm PM 2.5 (μg/m3)
  // Field6= Atm PM 10 (μg/m3)
  // Field7= Temperature (°C or F)*/
  // Field8= Humidity (%)
  const String ThingSpeak_PM_APIKey = "APIKEY1";

  /*
  // Raw Dust Channel:
  // Field1= Particule count 0.3 µm per 0.1 L
  // Field2= Particule count 0.5 µm per 0.1 L
  // Field3= Particule count 1.0 µm per 0.1 L
  // Field4= Particule count 2.5 µm per 0.1 L
  // Field5= Particule count 5.0 µm per 0.1 L
  // Field6= Particule count 10 µm per 0.1 L
  // Field7= US AQI */
  const String ThingSpeak_Raw_APIKey = "APIKEY2";
#endif

//MQTT
#define USE_MQTT //comment to remove MQTT data feeding
#define USE_MQTT_WITH_DOMOTICZ  //comment to remove MQTT data format required for Domoticz integration for graphing
#define USE_MQTT_WITH_FLATTOPIC //comment to remove MQTT data format required for generic data on flat topic
//                                it's possible to send MQTT data to both Domoticz structure and flat structure
#ifdef USE_MQTT
  /*
  * if you want to push data on a MQTT broker
  * Replace settings by your own values
  * Best template to install Mosquitto on Debian: https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-debian-8
  */
  #define MQTT_Server "MQTT-IP"
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
  #endif

  #ifdef USE_MQTT_WITH_FLATTOPIC
    #define MQTT_flattopic "3DPrint/Enclosure1"
    #define MQTT_temperature_Channel "temperature"
    #define MQTT_humidity_Channel "humidity"
    #define MQTT_atmPM01Value_Channel "atmPM01Value"
    #define MQTT_atmPM25Value_Channel "atmPM25Value"
    #define MQTT_atmPM10Value_Channel "atmPM10Value"
    #define MQTT_CF1PM01Value_Channel "CF1PM01Value"
    #define MQTT_CF1PM25Value_Channel "CF1PM25Value"
    #define MQTT_CF1PM10Value_Channel "CF1PM10Value"
    #define MQTT_Partcount0_3_Channel "Partcount0_3"
    #define MQTT_Partcount0_5_Channel "Partcount0_5"
    #define MQTT_Partcount1_0_Channel "Partcount1_0"
    #define MQTT_Partcount2_5_Channel "Partcount2_5"
    #define MQTT_Partcount5_0_Channel "Partcount5_0"
    #define MQTT_Partcount10_Channel "Partcount10"
    #define MQTT_airQualityIndex_Channel "airQualityIndex"
  #endif
#endif
