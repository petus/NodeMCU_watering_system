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

#define WATER_TIME_MIN	20
#define WATER_TIME_MAX	23
#define GROUND_HUMIDITY_MIN 700
#define GROUND_HUMIDITY_HYSTERESIS 50

//set SSID and password of WiFi network
const char* ssid = "..........";
const char* password = "..........";
const int AnalogIn  = A0;
const int waterDelayS = 10;
const int waterPumpPin = 5;	//D1 - NodeMCU

int timezone = 3;
int dst = 0;
int waiting = 0;
int g_humidity = 0;

void setup (void)
{
	Serial.begin(115200);
	//Serial.setDebugOutput(true);
	
	pinMode(waterPumpPin, OUTPUT);
	digitalWrite(waterPumpPin, LOW);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    waiting = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(1000);
      waiting++;
      if(waiting > 10) //testing of connection
      {
	Serial.println("Can not connect to WiFi");
	ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
      }
    }

    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.println("Waiting for time");
    waiting = 0;
    while (!time(nullptr)) 
    {
    	Serial.print(".");
     	delay(1000);
	waiting++;
	if(waiting > 10) //waiting for time
      	{
    		Serial.println("Time is not available");
    		ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
      	}
    }
    Serial.println("");

	g_humidity = analogRead(AnalogIn);
	Serial.println("Humidity of ground: ");
 	Serial.println(g_humidity);
}

void loop (void)
{	time_t now = time(nullptr);
	int nowHour = hour(now);
 	Serial.println(ctime(&now));
 	
 	digitalWrite(waterPumpPin, LOW);
 	
 	int g_humidity_min = GROUND_HUMIDITY_MIN - GROUND_HUMIDITY_HYSTERESIS;
 	int g_humidity_max = GROUND_HUMIDITY_MIN + GROUND_HUMIDITY_HYSTERESIS;

	if((nowHour > WATER_TIME_MIN) && (nowHour < WATER_TIME_MAX)) 
	{
		Serial.println("Time IS ok");
		if((g_humidity > g_humidity_min) /*&& (g_humidity < g_humidity_max)*/)
		{
			Serial.println("Ground humidity IS ok");
			digitalWrite(waterPumpPin, HIGH);
			for(int i; i < waterDelayS; i++)
			{
				Serial.print(">");
				delay(1000);
			}
			digitalWrite(waterPumpPin, LOW);
		}
		else
		{
			Serial.println("Humidity is NOT ok");
		}
	}
 	else
 	{
	 	Serial.println("Time is NOT ok");
 	}
	
	digitalWrite(waterPumpPin, LOW);
	//sleep
 	Serial.println("Go to sleep");
	ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
}
