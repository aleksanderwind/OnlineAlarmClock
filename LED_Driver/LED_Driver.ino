#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>  // Include the WebServer library

#include "html_page.h"
#include "LED_driver.h"
#include "iotserver.h"

#define LED_PIN 14
#define NUM_LEDS 5

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

String SSID = "ASUS7";
String PASSWORD = "Kniv7holt";

// Create an instance of the server
ESP8266WebServer webserver(80);

// Create an instance of the wifiMulti
ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

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
}