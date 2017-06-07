/*
* ESP8266 (NodeMCU) water system by Petus (2017)
* Water system checks time and humidity of ground
*
* https://chiptron.cz
* https://time4ee.com
*
* Idea:
* After starting of water system, ESP8266 (NodeMCU) checks NTP time
* If the time is ok (morning or night) then it measures humidity of ground
* Water system turns on water pump for some time
*
*/

#include <ESP8266WiFi.h>
#include <time.h> //ntp time library for esp8266
#include <Time.h> //time library by arduino

#define SLEEP_DELAY_IN_SECONDS  300

#define WATER_TIME_MIN	5
#define WATER_TIME_MAX	7
#define GROUND_HUMIDITY_MIN 500
#define GROUND_HUMIDITY_HYSTERESIS 50

//set SSID and password of WiFi network
const char* ssid = "..........";
const char* password = "..........";
const int AnalogIn  = A0;
const int waterDelayS = 10;
const int waterPumpPin = 5;

int timezone = 3;
int dst = 0;
int waiting = 0;
int g_humidity = 0;

void setup (void)
{
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	
	pinMode(waterPumpPin, OUTPUT);
	pinMode(waterPumpPin, LOW);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(1000);
      waiting++;
      if(waiting > 10) //testing of connection
      {
    	  Serial.println("\nCan not connect to WiFi");
		  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
      }
    }

    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) 
    {
    	Serial.print(".");
     	delay(1000);
		if(waiting > 10) //waiting for time
      	{
    		Serial.println("\nTime is not available");
    		ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
      	}
    }
    Serial.println("");

	g_humidity = analogRead(AnalogIn);
	Serial.println("\nHumidity of ground");
 	Serial.println(g_humidity);
}

void loop (void)
{	time_t now = time(nullptr);
	int nowHour = hour(now);
 	Serial.println(ctime(&now));
 	
 	pinMode(waterPumpPin, LOW);
 	
 	int g_humidity_min = GROUND_HUMIDITY_MIN - GROUND_HUMIDITY_HYSTERESIS;
 	int g_humidity_max = GROUND_HUMIDITY_MIN + GROUND_HUMIDITY_HYSTERESIS;

	if((nowHour > WATER_TIME_MIN) && (now < WATER_TIME_MAX)) 
	{
		Serial.println("\nTime is OK");
		if((g_humidity > g_humidity_min) && (g_humidity < g_humidity_max))
		{
			Serial.println("\nGround humidity is OK");
			pinMode(waterPumpPin, HIGH);
			for(int i; i < waterDelayS; i++)
			{
				Serial.print("!");
				delay(1000);
			}
			pinMode(waterPumpPin, LOW);
		}
	}
	
	pinMode(waterPumpPin, LOW);
	//sleep
	ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
}
