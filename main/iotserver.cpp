#include "iotserver.h"

// Define DHT pin and LDR pin (light dependend resistor)
int* DHTPIN;
int* LDRPIN;

DHTsensor* sensor; // Define sensor as a pointer to a DHTsensor object.

myTM* CurrentTime; // Define currentTime as a pointer to a custom time struct.
myTM* CurrentAlarm;

long* ColorValue;

extern bool itr;

data* SensorData;

ESP8266WebServer* SERVER; // Define SERVER as a pointer to a WebServer object.

LED* LED_STRIP; // Define LED_STRIP as a pointer an LED object.

SegmentDriver* SEGMENT;// Define SEGMENT as a pointer to a SegmentDriver object.

NTPClient* TimeClient; // Define TimeClient as a pointer to a NTP client.

String CURRENT_COLOR = "#000000"; // Global variable that is used for storing the current LED color. Default is black.

// Global variables for storing the date and time
String timeNotFormated = "";
String dateNotFormated = "";

String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int* CurrentSong; // Set up variable for selecting what song to play when the alarm rings. Default is song = 0.

int* TimeInterval;

/*
Initializes all needed global variables and server handles
Author: JKT, AC, AW
*/
void initServer(ESP8266WebServer* server, LED* strip, SegmentDriver* display, DHTsensor* SENSOR, data* sensorData) {
  SensorData = sensorData;
  
  sensor = SENSOR;
  
  SERVER = server;

  LED_STRIP = strip;

  SEGMENT = display;

  // Define endpoints and how to handle them
  SERVER->on("/", HTTP_GET, handleRoot);

  SERVER->on("/mode1", HTTP_GET, handleMode1);

  SERVER->on("/mode2", HTTP_GET, handleMode2);

  SERVER->on("/off", HTTP_GET, handleOff);

  SERVER->on("/setStaticColor", handleStaticColor);

  SERVER->on("/setAlarm", handleSetAlarm);

  SERVER->on("/getColor", getCurrentColor);

  SERVER->on("/getAlarmDateAndTime", getAlarmDateAndTime);

  SERVER->on("/setWakeUpSong", HTTP_GET, handleSetWakeUpSong);

  SERVER->on("/getCurrentWakeUpSong", getCurrentSong);

  SERVER->on("/updatePage", updatePage);

  SERVER->onNotFound(handleNotFound); //code 404
}

/*
This function initialises the NTP client, and sets CurrentTime to the provided time struct pointer.
Author: AC, AW
*/
void initNTP(NTPClient* timeClient, myTM* currentTime, myTM* currentAlarm){
  TimeClient = timeClient;
  CurrentTime = currentTime;
  CurrentAlarm = currentAlarm;
  TimeClient->begin();
  TimeClient->setTimeOffset(3600);
}

/*
Initialise variables for later use
Author: AC
*/
void initVars(long* colorValue, int* currentSong, int* timeInterval){
  ColorValue = colorValue;
  CurrentSong = currentSong;
  TimeInterval = timeInterval;
}

/*
Begin the server
Author: JKT
*/
void startServer() {
  SERVER->begin();
}

/*
Handle incoming requests
From exercises 14 and 15
*/
void handleClients() {
  SERVER->handleClient();
}

/*
Function to connect to wifi. 
INPUT: String ssid, String password and a WifiMulti pointer.
No outputs.

Based on sample code provided in exercises 14 and 15.
Author: JKT
*/
void connectToWifi(String SSID, String PASSWORD, ESP8266WiFiMulti* wifiMulti) {

  // Connect to WiFi network
  Serial.println();
  wifiMulti->addAP(SSID.c_str(), PASSWORD.c_str());  // add Wi-Fi to the wifiMulti object.

  Serial.println();
  Serial.print("Connecting ...");

  // Try to connect to the given Access Point (AP). 
  // When the ESP has connected to the AP, the while loop is exited.
  while (wifiMulti->run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Some debug prints
  Serial.println("");
  Serial.println("WiFi connected to ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // The IP address is needed to access the web client

  if (MDNS.begin("iot")) {  // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    delay(10);
  } else {
    Serial.println("Error setting up MDNS responder!");
    delay(10);
  }
}

/* 
root/index page
From exercises 14 and 15.
*/
void handleRoot() {
  SERVER->send(200, "text/html", index_html);
}

/*
Mode1 sets the LED to a static, predefined color.
Author: AC
*/
void handleMode1() {
  LED_STRIP->setLEDStripHex(0xFF0000); // Set color to full red
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

/*
Mode2 sets the LED to a static, predefined color.
Author: AC
*/
void handleMode2() {
  LED_STRIP->setLEDStripHex(0x0000FF); // Set color to full red
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

/*
Set all LEDs to a black color.
Author: AC
*/
void handleOff() {
  LED_STRIP->setLEDStripHex(0x000000); // Set all color to black, effectively turning off the strip
  delay(40);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

/*
Display custom color 
Author: AC
*/
void handleStaticColor() {
  String incomingHex = SERVER->arg("staticColor"); // Store the incoming argument in a string
  CURRENT_COLOR = incomingHex; // Set the CURRENT_COLOR variable to the incoming color
  String staticColor = incomingHex.substring(1).c_str(); // Remove the # from the string
  *ColorValue = hexToDec(staticColor);// Convert from string hex to a long value

  /*
  Serial.println(staticColor);
  Serial.println(*ColorValue, HEX);
  */
  
  LED_STRIP->setLEDStripHex(*ColorValue); // Set the LED strip
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

/*
Sets alarm to user defined time
Author: AC
*/
void handleSetAlarm() {
  itr = false;
  timeNotFormated = SERVER->arg("alarmTime");
  dateNotFormated = SERVER->arg("alarmDate");
  *TimeInterval = SERVER->arg("timeInterval").toInt();
    
  /* DEBUG
  Serial.println(dateNotFormated);
  Serial.println(timeNotFormated);
  */
  
  CurrentAlarm->hour = timeNotFormated.substring(0, 2).toInt();
  CurrentAlarm->minute = timeNotFormated.substring(3, 5).toInt();
  CurrentAlarm->day = dateNotFormated.substring(8, 10).toInt();
  CurrentAlarm->month = dateNotFormated.substring(5, 7).toInt();
  CurrentAlarm->year = dateNotFormated.substring(0, 4).toInt();

  CurrentAlarm->inEpoch = toEpochTime(CurrentAlarm->year,CurrentAlarm->month,CurrentAlarm->day,CurrentAlarm->hour,CurrentAlarm->minute);
  
  /* DEBUG
  Serial.println(year, DEC);
  Serial.println(month, DEC);
  Serial.println(day, DEC);
  Serial.println(hour, DEC);
  Serial.println(minute, DEC);
  */
  
  SERVER->sendHeader("Location", "/");
  SERVER->send(303);
}

/*
returns currently displayed color
Author: AC
*/
void getCurrentColor() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(CURRENT_COLOR));
}

/*
returns time and date of currently set alarm
Author: AC
*/
void getAlarmDateAndTime() {
  SERVER->send(STATUSCODE_OK, "text/plain", dateNotFormated + "#" + timeNotFormated + "#" + String(*TimeInterval));
  //Serial.println(dateNotFormated + "#" + timeNotFormated);
}

/*
returns currently playing melody
Author: AC
*/
void getCurrentSong() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(*CurrentSong));
}

/*
sets melody to be played to the one defined by the user
Author: AC
*/
void handleSetWakeUpSong() {
  *CurrentSong = SERVER->arg("songID").toInt();
  Serial.println(*CurrentSong);
  SERVER->sendHeader("Location", "/");
  SERVER->send(303);
}

/*
Updates time displayed on the page
Author: AC
*/
void updatePage(){
  readSensors(SensorData, sensor);
  //Serial.println(weekDays[CurrentTime->day] + "#" + String(CurrentTime->hour) + "#" + String(CurrentTime->minute) + "#" + String(SensorData->temperature) + "#" + String(SensorData->humidity) + "#" + String(SensorData->lightLevel));
  SERVER->send(STATUSCODE_OK, "text/plain", weekDays[CurrentTime->day] + "#" + String(CurrentTime->hour) + "#" + String(CurrentTime->minute) + "#" + String(SensorData->temperature) + "#" + String(SensorData->humidity) + "#" + String(SensorData->lightLevel));
}

/*
Updates current time values
Author: AC
*/
void updateTime(){
  TimeClient->update();
  CurrentTime->hour = TimeClient->getHours();
  CurrentTime->minute = TimeClient->getMinutes();
  CurrentTime->day = TimeClient->getDay();
  CurrentTime->inEpoch = TimeClient->getEpochTime();

  SEGMENT->setClock(CurrentTime->hour, CurrentTime->minute);
}

/*
404 error when no mathing handle is found
From exercises 14 and 15.
*/
void handleNotFound() {
  SERVER->send(404, "text/plain", "404: Not found");  // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
