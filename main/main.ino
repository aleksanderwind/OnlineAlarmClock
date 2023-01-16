#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <DHT.h>

#include "html_page.h"
#include "LED_driver.h"
#include "iotserver.h"
#include "DHTsensor.h"

#define LED_PIN 14
#define NUM_LEDS 5

const int DHTPIN = 12;
const int LDRPIN = A0; //find pin

//const int photosens = A0;

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

DHTsensor sens(DHTPIN, LDRPIN);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

String SSID = "Hot MILF nearby";
String PASSWORD = "14151415";

// Create an instance of the server
ESP8266WebServer webserver(80);

// Create an instance of the wifiMulti
ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  // init sensors (DHT11 and LDR)
  //initSensors(&dht, photosens);

  // Initialize the IoT server by parsing pointers to the webserver and led_strip object.
  initServer(&webserver, &led_strip, &sens);

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
