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

ESP8266WiFiMulti wifiMulti;

// Create custom IOTSERVER object
IOTSERVER iotserver(SSID, PASSWORD, &webserver, &led_strip);


void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  iotserver.connectToWifi();

  // Start the server
  iotserver.begin();
  Serial.println("Server started");

  led_strip.clear();
}

void loop() {
  // Check if a client has connected
  iotserver.handleClient();
  delay(100);
}