#ifndef iotserver_h
#define iotserver_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>  // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <ESP8266mDNS.h>       // Include the mDNS library
#include "html_page.h"
#include "LED_driver.h"

#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303


class IOTSERVER {
private:


public:
  String SSID;

  String PASSWORD;

  ESP8266WiFiMulti wifiMulti;

  IOTSERVER(String ssid, String password, ESP8266WebServer* server, LED* strip);

  void begin();

  void handleClient();

  void connectToWifi();
};

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
#endif  // iotserver