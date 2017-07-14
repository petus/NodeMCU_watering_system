/*
  ESP8266 (NodeMCU) water system by Petus (2017)
  Water system checks time and soil moisture

  https://chiptron.cz
  https://time4ee.com

  Idea:
  After starting of water system, ESP8266 (NodeMCU) checks NTP time
  If the time is ok (morning or night) then it measures soil moisture
  Water system turns on water pump for some time

*/

#include <ESP8266WiFi.h>
#include <time.h> //ntp time library for esp8266
#include <Time.h> //time library by arduino

#define SLEEP_DELAY_IN_SECONDS  300 //D0 Wake_Up pin

#define WATER_TIME_MIN  18 //start time
#define WATER_TIME_MAX  23 //end time
#define SOIL_MOISTURE_MIN 650 //min value of soil moisture
#define SOIL_MOISTURE_HYST 0 //hyteresis of soil moisture

//set SSID and password of WiFi network
const char* ssid = "SSID";
const char* password = "PASSWORD";
const int analogIn  = A0;
const int output1Delay = 10; //time of running - seconds
const int output1 = 14; //D5
const int output1LED = 13; //D7
const int output2 = 12; //D6
const int output2LED = 15; //D8

int timezone = 3;
int dst = 0;
int waiting = 0;
int soilMoisture = 0;
int airHumidity = 0;

void output1ON(void)
{
  digitalWrite(output1, HIGH); //turn on the output1
  digitalWrite(output1LED, LOW); //turn on LED indication
}

void output1OFF(void)
{
  digitalWrite(output1, LOW); //turn off the output1
  digitalWrite(output1LED, HIGH); //turn off LED indication
}

void output2ON(void)
{
  digitalWrite(output1, HIGH); //turn on the output2
  digitalWrite(output1LED, LOW); //turn on LED indication
}

void output2OFF(void)
{
  digitalWrite(output1, LOW); //turn off the output2
  digitalWrite(output1LED, HIGH); //turn off LED indication
}

void setup (void)
{
  Serial.begin(115200);
  //Serial.setDebugOutput(true);

  pinMode(output1, OUTPUT);
  pinMode(output1LED, OUTPUT);
  output1OFF();

  pinMode(output2, OUTPUT);
  pinMode(output2LED, OUTPUT);
  output2OFF();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  waiting = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
    waiting++;
    if (waiting > 10) //testing of connection
    {
      Serial.println("\nCan not connect to WiFi");
      ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    }
  }

  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  waiting = 0;
  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
    waiting++;
    if (waiting > 10) //waiting for time
    {
      Serial.println("\nTime is not available");
      ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
    }
  }
  Serial.println("");

  soilMoisture = analogRead(analogIn);
  Serial.println("\nSoil Moisture: ");
  Serial.println(soilMoisture);
}

void loop (void)
{
  time_t now = time(nullptr);
  int nowHour = hour(now);
  Serial.println(ctime(&now));
  Serial.println(nowHour);

  output1OFF();

  int soilMoisture_min = SOIL_MOISTURE_MIN - SOIL_MOISTURE_HYST;
  int soilMoisture_max = SOIL_MOISTURE_MIN + SOIL_MOISTURE_HYST;

  if ((nowHour > WATER_TIME_MIN) && (nowHour < WATER_TIME_MAX))
  {
    Serial.println("\nTime is OK");
    Serial.println(soilMoisture);
    if ((soilMoisture > soilMoisture_min)/* && (soilMoisture < soilMoisture_max)*/)
    {
      Serial.println("\nSoil Moisture is OK");
      output1ON();
      for (int i; i < output1Delay; i++)
      {
        Serial.print("!");
        delay(1000);
      }
      output1OFF();
    }
    else
    {
      Serial.println("Soil Moisture is NOT ok");
    }
  }
  else
  {
    Serial.println("Time is NOT ok");
  }

  /*-------------sleep-----------------*/
  output1OFF();
  //sleep
  Serial.println("Go to sleep");
  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
}
