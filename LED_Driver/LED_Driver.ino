#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>  // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <ESP8266mDNS.h>       // Include the mDNS library
#include <Adafruit_NeoPixel.h>

#include "html_page.h"
#include "LED_driver.h"

#define LED_PIN 14
#define NUM_LEDS 5

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB);

LED led_strip(NUM_LEDS, LED_PIN, &strip);

#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303

String CURRENT_COLOR = "#FFFFFF";

String timeNotFormated = "";
String dateNotFormated = "";


ESP8266WiFiMulti wifiMulti;
// Create an instance of the server
ESP8266WebServer server(80);


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
void wifiINIT(String ssid, String password);

void setup() {
  Serial.begin(115200);
  delay(10);

  led_strip.setLEDStrip(0,0,0);
  wifiINIT("AndroidAP", "12345689");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/mode1", HTTP_GET, handleMode1);
  server.on("/mode2", HTTP_GET, handleMode2);
  server.on("/off", HTTP_GET, handleOff);
  server.on("/setStaticColor", handleStaticColor);
  server.on("/setAlarm", handleSetAlarm);
  server.on("/getColor", getCurrentColor);
  server.on("/getAlarmDateAndTime",getAlarmDateAndTime);
  server.on("/setWakeUpSong", HTTP_GET, handleSetWakeUpSong);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("Server started");

  led_strip.clear();
}

void loop() {
  // Check if a client has connected
  server.handleClient();
}

void wifiINIT(const char* ssid, const char* password) {
  // Connect to WiFi network
  Serial.println();
  wifiMulti.addAP(ssid, password);  // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP("<ssid2>", "<password>");

  Serial.println();
  Serial.print("Connecting ...");
  //WiFi.begin(ssid, password);

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected to ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("iot")) {  // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

/*void setColor(int r, int g, int b){
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}*/


/*void setColorHEX(long colorValue){
  analogWrite(redPin, colorValue>>16);
  analogWrite(greenPin, (colorValue & 0x00ff00)>>8);
  analogWrite(bluePin, (colorValue & 0x0000ff));
}*/

void handleMode1() {
  led_strip.setLEDStripHex(0xFF0000);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleMode2() {
  led_strip.setLEDStripHex(0x0000FF);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleOff() {
  led_strip.setLEDStripHex(0x000000);
  delay(40);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleStaticColor() {
  String incomingHex = server.arg("staticColor");
  CURRENT_COLOR = incomingHex;
  String staticColor = incomingHex.substring(1).c_str();
  long colorValue = hexToDec(staticColor);
  Serial.println(staticColor);
  Serial.println(colorValue, HEX);
  /*
  Serial.println(colorValue>>16);
  Serial.println((colorValue & 0x00ff00)>>8);
  Serial.println((colorValue & 0x0000ff));
  setColorHEX(colorValue);*/
  led_strip.setLEDStripHex(colorValue);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleSetAlarm(){
  timeNotFormated = server.arg("alarmTime");
  dateNotFormated = server.arg("alarmDate");
  Serial.println(dateNotFormated);
  Serial.println(timeNotFormated);
  int hour = timeNotFormated.substring(0,2).toInt();
  int minute = timeNotFormated.substring(3,5).toInt();
  int day = dateNotFormated.substring(8,10).toInt();
  int month = dateNotFormated.substring(5,7).toInt();
  int year = dateNotFormated.substring(0,4).toInt();
  Serial.println(year, DEC);
  Serial.println(month, DEC);
  Serial.println(day, DEC);
  Serial.println(hour, DEC);
  Serial.println(minute, DEC);
  server.sendHeader("Location","/");
  server.send(303);  
}

void getCurrentColor() {
  server.send(STATUSCODE_OK, "text/plain", String(CURRENT_COLOR));
}

void getAlarmDateAndTime(){
  server.send(STATUSCODE_OK, "text/plain", dateNotFormated + "#" + timeNotFormated);
  Serial.println(dateNotFormated + "#" + timeNotFormated);
}

void handleSetWakeUpSong(){
  int songID = server.arg("songID").toInt();
  Serial.println(songID);
  server.sendHeader("Location","/");
  server.send(303);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
