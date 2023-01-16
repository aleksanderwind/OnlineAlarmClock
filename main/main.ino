#include <Adafruit_NeoPixel.h>
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <DHT.h>

#include "html_page.h"
#include "LED_driver.h"
#include "iotserver.h"
#include "sensors.h"
#include "interface.h"

#define LED_PIN 4 //D2 on ESP8266
#define NUM_LEDS 5

#define DIN 13  //D7 on ESP8266
#define CLK 14  //D5 on ESP8266
#define CS 15   //D8 on ESP8266

#define DHTPIN 12 //D6 on ESP8266 
#define DHTTYPE DHT11
#define photosens A0

SegmentDriver display = SegmentDriver(DIN, CLK, CS);

DHT dht(DHTPIN, DHTTYPE);

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

String SSID = "Jesper";
String PASSWORD = "12345677";

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

  // Initialize the IoT server by parsing pointers to the webserver and led_strip object.
  initServer(&webserver, &led_strip, &display);

  // Connect to the wifi using the set SSID and PASSWORD
  connectToWifi(SSID, PASSWORD, &wifiMulti);

  // Start the IoT server
  startServer();
  Serial.println("Server started");
  
  if (display.setClock(0, 0) != 0 ){
    Serial.println("Failed to display clock.");
  }
}

void loop() {
  // Check if a client has connected
  handleClients();
}
