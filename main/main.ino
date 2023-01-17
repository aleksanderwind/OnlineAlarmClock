#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <DHT.h>

#include "html_page.h"
#include "LED_driver.h"
#include "iotserver.h"
#include "DHTsensor.h"
#include "interface.h"
#include "themes.h"

struct myTM currentTime;

#define LED_PIN 4 //D2 on ESP8266
#define NUM_LEDS 5

#define DIN 13  //D7 on ESP8266
#define CLK 14  //D5 on ESP8266
#define CS 15   //D8 on ESP8266

#define DHTPIN 12 //D6 on ESP8266 
#define LDRPIN A0

#define BUZZER_PIN 999;

initBuzzer(BUZZER_PIN);

SegmentDriver display = SegmentDriver(DIN, CLK, CS);

//declare NTP objects
const char* ntpServer = "pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);

DHTsensor sens(DHTPIN, LDRPIN);

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

String SSID = "Jesper";
String PASSWORD = "12345677";

// Create an instance of the server
ESP8266WebServer webserver(80);

// Create an instance of the wifiMulti
ESP8266WiFiMulti wifiMulti;

void setup() {
  attachInterrupt(digitalPinToInterrupt(D1), ISR, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.println();

  // init NTP to get time from a server
  initNTP(&timeClient, &currentTime);

  // Initialize the IoT server by parsing pointers to the webserver and led_strip object.
  initServer(&webserver, &led_strip, &display, &sens);

  // Connect to the wifi using the set SSID and PASSWORD
  connectToWifi(SSID, PASSWORD, &wifiMulti);

  // Start the IoT server
  startServer();
  Serial.println("Server started");
  
  if (display.setClock(0, 0) != 0 ){
    Serial.println("Failed to display clock.");
  }
}

void ISR()
{
  interrupt();
}

void loop() {
  // Check if a client has connected
  handleClients();
  updateTime();
}
