#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x)  Serial.println(x)
#define DEBUG_PRINT(x)  Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

//////////////////////////////////////////////////////
///////// Replace Wifi connexion information by //////
///////// your own values                       //////
//////////////////////////////////////////////////////
const char* Wifi_SSID = "SSID";
const char* Wifi_PASSWORD = "PASSWORD!";

//////////////////////////////////////////////////////
///////// if you want to push data on thingspeak /////
///////// Replace API keys by your own values ////////
//////////////////////////////////////////////////////
const String ThingSpeak_PM_APIKey = "Channel-PM-API-KEY";
const String ThingSpeak_Raw_APIKey = "Channel-Raw-API-KEY";
const char* ThingSpeak_API_Server = "api.thingspeak.com";

// I2C LCd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Thingspeak
#define USE_THINGSPEAK //comment to remove thinkspeak data feeding

//MQTT
#define USE_MQTT //comment to remove MQTT data feeding (not implemented yet)

//Wifi
#define USE_WIFI //Wifi connexion required for ThingSpeak feeding
const int MAX_WIFI_TRY=4;

//DHT11 / DHT21 / DHT22
#define USE_CELCIUS //define to use Celcius temperature, comment to use Farhenheit
const int PINDHT = D5;
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22  // DHT 22  (AM2302)
float humidity = 0;
float temperature = 0;
DHT dht(PINDHT, DHTTYPE);

//PMS5003
const int PINPMSGO = D0; // what pin we’re connected to activate measurement

#define MSG_LENGTH 31   //0x42 + 31 bytes equal to PMS5003 serial message packet lenght
#define HTTP_TIMEOUT 20000 //maximum http response wait period, sensor disconects if no response
#define MIN_WARM_TIME 35000 //warming-up period requred for sensor to enable fan and prepare air chamber
#define SLEEP_TIME 30000 // time between each loop
unsigned char buf[MSG_LENGTH];

int atmPM01Value = 0;  //define PM1.0 value of the air detector module
int atmPM25Value = 0;  //define pm2.5 value of the air detector module
int atmPM10Value = 0;  //define pm10 value of the air detector module
int CF1PM01Value = 0;  //define PM1.0 value of the air detector module
int CF1PM25Value = 0;  //define pm2.5 value of the air detector module
int CF1PM10Value = 0;  //define pm10 value of the air detector module
int Partcount0_3 = 0;
int Partcount0_5 = 0;
int Partcount1_0 = 0;
int Partcount2_5 = 0;
int Partcount5_0 = 0;
int Partcount10 = 0;

int airQualityIndex = 0;


void setupWIFI() {
#ifdef USE_WIFI
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(Wifi_SSID, Wifi_PASSWORD);
  int i = 0;
  DEBUG_PRINTLN("MAC address: ");
  DEBUG_PRINTLN(WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    i++;
    if (i > MAX_WIFI_TRY) {
          i = 0;
          DEBUG_PRINTLN("WiFi failed, retry");
          Serial.printf("WiFi status:%d\n",  WiFi.status());
          //ESP.reset();
          
          WiFi.disconnect();
          WiFi.persistent(false);
          WiFi.mode(WIFI_OFF);
          WiFi.mode(WIFI_STA);
          WiFi.begin(Wifi_SSID, Wifi_PASSWORD);
      }
    
    DEBUG_PRINT(".");
  }

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("WiFi connected");
  DEBUG_PRINTLN("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
#endif
}

// ThingSpeak Channel definition

// PM Channel:
// Field1= CF1 PM1
// Field2= CF1 PM2.5
// Field3= CF1 PM10
// Field4= Atm PM1
// Field5= Atm PM2.5
// Field6= Atm PM10
// Field7= Humidity
// Field8= Temperature

// Raw Dust Channel:
// Field1= Particule count 0.3 µm
// Field2= Particule count 0.5 µm
// Field3= Particule count 1.0 µm
// Field4= Particule count 2.5 µm
// Field5= Particule count 5.0 µm
// Field6= Particule count 10 µm
// Field7= AQI

#ifdef USE_THINGSPEAK

void sendPMDataToCloud() {
  DEBUG_PRINTLN("sendPMDataToCloud start");

  // Enclosure-PM Channel
    String postStr = ThingSpeak_PM_APIKey;
    postStr += "&field1=";
    postStr += String(CF1PM01Value);
    postStr += "&field2=";
    postStr += String(CF1PM25Value);
    postStr += "&field3=";
    postStr += String(CF1PM10Value);
    postStr += "&field4=";
    postStr += String(atmPM01Value);
    postStr += "&field5=";
    postStr += String(atmPM25Value);
    postStr += "&field6=";
    postStr += String(atmPM10Value);
    if (!isnan(temperature)) {
        postStr += "&field7=";
        postStr += String(temperature);
    }
    if (!isnan(humidity)) {
        postStr += "&field8=";
        postStr += String(humidity);
    }
    postStr += "\r\n\r\n";

    sendDataToCloud(ThingSpeak_PM_APIKey, postStr);
}

void sendRawDataToCloud() {
  DEBUG_PRINTLN("sendRawDataToCloud start");

  // Enclosure-RawDust Channel
    String postStr = ThingSpeak_Raw_APIKey;
    postStr += "&field1=";
    postStr += String(Partcount0_3);
    postStr += "&field2=";
    postStr += String(Partcount0_5);
    postStr += "&field3=";
    postStr += String(Partcount1_0);
    postStr += "&field4=";
    postStr += String(Partcount2_5);
    postStr += "&field5=";
    postStr += String(Partcount5_0);
    postStr += "&field6=";
    postStr += String(Partcount10);
    postStr += "&field7=";
    postStr += String(airQualityIndex);
    postStr += "\r\n\r\n";

    sendDataToCloud(ThingSpeak_Raw_APIKey, postStr);
}

void sendDataToCloud(String apikey, String strfield) {
  DEBUG_PRINTLN("sendDataToCloud start");
  // Use WiFiClient class to create TCP connections
  //setupWIFI();
  WiFiClient client;
  if (!client.connect(ThingSpeak_API_Server, 80)) {
    DEBUG_PRINT("connection failed to:");
    DEBUG_PRINTLN(ThingSpeak_API_Server);
    return;
  }

  DEBUG_PRINTLN("Sending:" + strfield);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apikey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(strfield.length());
    client.print("\n\n");
    client.print(strfield);

  client.flush();
  delay(10);
  DEBUG_PRINTLN("wait for response");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > HTTP_TIMEOUT) {
      DEBUG_PRINTLN(">>> Client Timeout !");
      client.stop();
      DEBUG_PRINTLN("closing connection by timeout");
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    DEBUG_PRINT(line);
  }

  client.stop();
  DEBUG_PRINTLN("closing connection");
}
#endif

boolean validateMsg() {
  int receiveSum = 0;
  for (int i = 0; i < (MSG_LENGTH - 2); i++) {
    receiveSum = receiveSum + buf[i];
  }
  receiveSum = receiveSum + 0x42;
  return receiveSum == ((buf[MSG_LENGTH - 2] << 8) + buf[MSG_LENGTH - 1]);
}

//Decode frame values
int decodeAtmosphericPM01(unsigned char *thebuf) {
  return ((thebuf[3] << 8) + thebuf[4]); //count PM1.0 atmospheric value of the air detector module
}

int decodeCF1PM01(unsigned char *thebuf) {
  return ((thebuf[9] << 8) + thebuf[10]); //count PM1.0 atmospheric value of the air detector module
}

int decodeAtmosphericPM25(unsigned char *thebuf) {
  return ((thebuf[11] << 8) + thebuf[12]); //count pm2.5 atmospheric value of the air detector module
}

int decodeCF1PM25(unsigned char *thebuf) {
  return ((thebuf[5] << 8) + thebuf[6]); //count pm2.5 atmospheric value of the air detector module
}
  
int decodeAtmosphericPM10(unsigned char *thebuf) {
  return ((thebuf[13] << 8) + thebuf[14]); //count pm10 atmospheric value of the air detector module
}

int decodeCF1PM10(unsigned char *thebuf) {
  return ((thebuf[7] << 8) + thebuf[8]); //count pm10 atmospheric value of the air detector module
}

int decodeCount0_3(unsigned char *thebuf) {
  return ((thebuf[15] << 8) + thebuf[16]); //count pm10 atmospheric value of the air detector module
}

int decodeCount0_5(unsigned char *thebuf) {
  return ((thebuf[17] << 8) + thebuf[18]); //count pm10 atmospheric value of the air detector module
}

int decodeCount1_0(unsigned char *thebuf) {
  return ((thebuf[19] << 8) + thebuf[20]); //count pm10 atmospheric value of the air detector module
}

int decodeCount2_5(unsigned char *thebuf) {
  return ((thebuf[21] << 8) + thebuf[22]); //count pm10 atmospheric value of the air detector module
}

int decodeCount5_0(unsigned char *thebuf) {
  return ((thebuf[23] << 8) + thebuf[24]); //count pm10 atmospheric value of the air detector module
}

int decodeCount10(unsigned char *thebuf) {
  return ((thebuf[25] << 8) + thebuf[26]); //count pm10 atmospheric value of the air detector module
}


// AQI formula: https://en.wikipedia.org/wiki/Air_Quality_Index#United_States
int toAQI(int I_high, int I_low, int C_high, int C_low, int C) {
  return (I_high - I_low) * (C - C_low) / (C_high - C_low) + I_low;
}

//thanks to https://gist.github.com/nfjinjing/8d63012c18feea3ed04e
int calculateAQI25(float density) {

  int d10 = (int)(density * 10);
  if (d10 <= 0) {
    return 0;
  } else if (d10 <= 120) {
    return toAQI(50, 0, 120, 0, d10);
  } else if (d10 <= 354) {
    return toAQI(100, 51, 354, 121, d10);
  } else if (d10 <= 554) {
    return toAQI(150, 101, 554, 355, d10);
  } else if (d10 <= 1504) {
    return toAQI(200, 151, 1504, 555, d10);
  } else if (d10 <= 2504) {
    return toAQI(300, 201, 2504, 1505, d10);
  } else if (d10 <= 3504) {
    return toAQI(400, 301, 3504, 2505, d10);
  } else if (d10 <= 5004) {
    return toAQI(500, 401, 5004, 3505, d10);
  } else if (d10 <= 10000) {
    return toAQI(1000, 501, 10000, 5005, d10);
  } else {
    return 1001;
  }
}

void powerOnSensor() {
  //Switch on PMS sensor
  digitalWrite(PINPMSGO, HIGH);
  //Warm-up
  unsigned long timeout = millis();
  timeout = MIN_WARM_TIME - (millis() - timeout);
  if (timeout > 0) {
    DEBUG_PRINT("sensor warm-up: ");
    DEBUG_PRINTLN(timeout);
    delay(timeout);
  }
}

void powerOffSensor() {
#ifdef USE_WIFI
  WiFi.disconnect();
#endif
  //Switch off PMS sensor
  digitalWrite(PINPMSGO, LOW);
}

void ShowDataonLCD() {
    char buffer[35]; 
    lcd.clear();

    if (!isnan(temperature)) {
      lcd.setCursor(0,0);
      float f=temperature;
#ifdef USE_CELCIUS
      sprintf(buffer, "T:%d.%01dC ",(int)f, (int)(f*10)%10);
#else
      sprintf(buffer, "T:%d.%01dF ",(int)f, (int)(f*10)%10);
#endif
      lcd.print(buffer);
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("T: NaN");
    }
    
    if (!isnan(humidity)) {
      lcd.setCursor(8,0);
      float f=humidity;
      sprintf(buffer, "H:%d.%01d%%",(int)f, (int)(f*10)%10);
      lcd.print(buffer);
    }  
    else {
      lcd.setCursor(7,0);
      lcd.print("H: NaN");
    }

    lcd.setCursor(0,1);
    sprintf(buffer, "AQ:%d ",(int)airQualityIndex);
    lcd.print(buffer);

    lcd.setCursor(7,1);
    sprintf(buffer, "PM:%d ",(int)CF1PM01Value);
    lcd.print(buffer);

}

// Setup block
void setup() {
  Serial.begin(9600);

  DEBUG_PRINTLN("Initialize LCD");
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.print("Initialize ...");
  DEBUG_PRINTLN("LCD Initialized");
  
  delay(10);
  dht.begin();

  DEBUG_PRINTLN(" Init started: DEBUG MODE");
  Serial.setTimeout(1500);//set the Timeout to 1500ms, longer than the data transmission time of the sensor
  pinMode(PINPMSGO, OUTPUT); // Define PMS 5003 pinout
  lcd.setCursor(0,1); 
  lcd.print("Warming up ...");


  DEBUG_PRINTLN("Initialization finished");
}

// Loop block
void loop() {
  DEBUG_PRINTLN("loop start");

  powerOnSensor();
  #ifdef USE_WIFI
    setupWIFI();
  #endif

  //Get Temperature & Humidity from DHT
  humidity = dht.readHumidity();
  #ifdef USE_CELCIUS
    temperature = dht.readTemperature();
  #else
    temperature = dht.readTemperature(true);
  #endif
  if (isnan(humidity) || isnan(temperature)) {
    DEBUG_PRINTLN("DHTXX not ready, skipped");
  }


  for (int i = 0; i < 30; i++) {
  Serial.swap(); //Use UART2 for PMS5003 communication (Allow sketch upload without having to unplug GPIO3 !!!!)
    if (Serial.find(0x42)) {  //start to read when detect 0x42
      Serial.readBytes(buf, MSG_LENGTH);
      Serial.swap();

      if (buf[0] == 0x4d && validateMsg()) {
        CF1PM01Value = decodeCF1PM01(buf); //count PM1.0 CF1 value of the air detector module
        CF1PM25Value = decodeCF1PM25(buf);//count pm2.5 CF1 value of the air detector module
        CF1PM10Value = decodeCF1PM10(buf); //count pm10 CF1 value of the air detector module

        atmPM01Value = decodeAtmosphericPM01(buf); //count PM1.0 atmospheric value of the air detector module
        atmPM25Value = decodeAtmosphericPM25(buf);//count pm2.5 atmospheric value of the air detector module
        atmPM10Value = decodeAtmosphericPM10(buf); //count pm10 atmospheric value of the air detector module
    
        Partcount0_3 = decodeCount0_3(buf); //count 0.3 µm particulates count
        Partcount0_5 = decodeCount0_5(buf); //count 0.5 µm particulates count
        Partcount1_0 = decodeCount1_0(buf); //count 1.0 µm particulates count
        Partcount2_5 = decodeCount2_5(buf); //count 2.5 µm particulates count
        Partcount5_0 = decodeCount5_0(buf); //count 5.0 µm particulates count
        Partcount10 = decodeCount10(buf); //count 10 µm particulates count

        /*if (atmPM01Value == atmPM25Value || atmPM25Value == atmPM10Value) {
          //it is very rarely happened that different particles have same value, better to read again
          DEBUG_PRINT("skip loop:");
          DEBUG_PRINTLN(i);
          continue;
        }
        */
        airQualityIndex = calculateAQI25(atmPM25Value);


#ifdef DEBUG //debug info only for serial connection debugging
        DEBUG_PRINTLN("Temperature: " + String(temperature));
        DEBUG_PRINTLN("Humidity: " + String(humidity));
        DEBUG_PRINTLN("Atm PM 1.0: " + String(atmPM01Value));
        DEBUG_PRINTLN("Atm PM 2.5: " + String(atmPM25Value));
        DEBUG_PRINTLN("Atm PM  10: " + String(atmPM10Value));
        DEBUG_PRINTLN("CF1 PM 1.0: " + String(CF1PM01Value));
        DEBUG_PRINTLN("CF1 PM 2.5: " + String(CF1PM25Value));
        DEBUG_PRINTLN("CF1 PM  10: " + String(CF1PM10Value));
        DEBUG_PRINTLN("AQI   : " + String(airQualityIndex));
        DEBUG_PRINTLN("Count 0.3 : " + String(Partcount0_3));
        DEBUG_PRINTLN("Count 0.5 : " + String(Partcount0_5));
        DEBUG_PRINTLN("Count 1.0 : " + String(Partcount1_0));
        DEBUG_PRINTLN("Count 2.5 : " + String(Partcount2_5));
        DEBUG_PRINTLN("Count 5.0 : " + String(Partcount5_0));
        DEBUG_PRINTLN("Count  10 : " + String(Partcount10));
        DEBUG_PRINTLN("--------------");
#endif

        ShowDataonLCD();

#ifdef USE_THINGSPEAK
        sendPMDataToCloud();
        sendRawDataToCloud();
#endif
#ifdef USE_THINGSPEAK
    //Placeholder for MQTT call
#endif     

        DEBUG_PRINTLN("Waiting for next loop");
        delay(SLEEP_TIME); // Wait more than 15 seconds to send additionnal data

    } else {
        DEBUG_PRINTLN("message validation error");
      }
    } else {
    Serial.swap();
    DEBUG_PRINTLN("sensor msg start not found");
    }
  }

  powerOffSensor();
}
