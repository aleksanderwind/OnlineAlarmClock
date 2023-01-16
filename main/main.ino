#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <DHT.h>

#include "html_page.h"
#include "LED_driver.h"
#include "iotserver.h"
#include "sensors.h"

struct myTM currentTime;

#define LED_PIN 14
#define NUM_LEDS 5

const int DHTPIN = 13;
const int DHTTYPE = DHT11;
const int photosens = A0;

//declare NTP objects
const char* ntpServer = "pool.ntp.org";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer);

DHT dht(DHTPIN, DHTTYPE);

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

String SSID = "AndroidAP";
String PASSWORD = "12345689";

// Create an instance of the server
ESP8266WebServer webserver(80);

// Create an instance of the wifiMulti
ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  // init sensors (DHT11 and LDR)
  initSensors(&dht, photosens);

  // init NTP to get time from a server
  initNTP(&timeClient, &currentTime);

  // Initialize the IoT server by parsing pointers to the webserver and led_strip object.
  initServer(&webserver, &led_strip);

  // Connect to the wifi using the set SSID and PASSWORD
  connectToWifi(SSID, PASSWORD, &wifiMulti);

  // Start the IoT server
  startServer();
  Serial.println("Server started");

  led_strip.clear();
}

void loop() {
  // Check if a client has connected
  handleClients();
  updateTime();
}
