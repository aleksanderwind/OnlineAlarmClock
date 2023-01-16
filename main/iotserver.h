#ifndef iotserver_h
#define iotserver_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>  // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <ESP8266mDNS.h>       // Include the mDNS library
#include "html_page.h"
#include "LED_driver.h"
#include "DHTsensor.h"

#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303

void connectToWifi(String ssid, String password, ESP8266WiFiMulti* wifiMulti);

void initServer(ESP8266WebServer* server, LED* strip, DHTsensor* sensor);

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

void getSensorReading();

#endif  // iotserver
