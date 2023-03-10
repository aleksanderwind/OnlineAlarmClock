#ifndef iotserver_h
#define iotserver_h

/*
Header file created by: JKT
Authors for individual functions can be found in the iotserver.cpp file.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>  // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <ESP8266mDNS.h>       // Include the mDNS library
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "html_page.h"
#include "LED_driver.h"
#include "interface.h"
#include "DHTsensor.h"

// Two of the status codes that are commonly used throughout this project
#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303

void connectToWifi(String ssid, String password, ESP8266WiFiMulti* wifiMulti);

void initServer(ESP8266WebServer* server, LED* strip, SegmentDriver* display, DHTsensor* SENSOR, data* SensorData);

void initNTP(NTPClient* timeClient, myTM* currentTime, myTM* currentAlarm);

void initVars(long* colorValue, int* currentSong, int* timeInterval);

void startServer();

void handleClients();

void handleRoot();

void handleMode1();

void handleMode2();

void handleOff();

void handleStaticColor();

void handleSetAlarm();

void handleSetWakeUpSong();

void handleNotFound();

void getCurrentColor();

void getAlarmDateAndTime();

void getCurrentSong();

void updatePage();

void updateTime();

#endif  // iotserver
