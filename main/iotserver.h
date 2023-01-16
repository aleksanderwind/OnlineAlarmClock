#ifndef iotserver_h
#define iotserver_h

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

#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303

struct myTM {
  int hour;
  int minute;
  int day;
  int month;
  long year;
};

void connectToWifi(String ssid, String password, ESP8266WiFiMulti* wifiMulti);

void initServer(ESP8266WebServer* server, LED* strip, SegmentDriver* display);

void initNTP(NTPClient* timeClient, myTM* currentTime);

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

void getSensorData();

void updatePage();

void updateTime();

#endif  // iotserver
