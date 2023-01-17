#include "iotserver.h"

// Define DHT pin and LDR pin (light dependend resistor)
int* DHTPIN;
int* LDRPIN;

DHTsensor* sensor; // Define sensor as a pointer to a DHTsensor object.

myTM* CurrentTime; // Define currentTime as a pointer to a custom time struct.

ESP8266WebServer* SERVER; // Define SERVER as a pointer to a WebServer object.

LED* LED_STRIP; // Define LED_STRIP as a pointer an LED object.

SegmentDriver* SEGMENT;// Define SEGMENT as a pointer to a SegmentDriver object.

NTPClient* TimeClient; // Define TimeClient as a pointer to a NTP client.

String CURRENT_COLOR = "#FFFFFF"; // Global variable that is used for storing the current LED color. Default is white.

// Global variables for storing the date and time
String timeNotFormated = "";
String dateNotFormated = "";

String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int currentSong = 0; // Set up variable for selecting what song to play when the alarm rings. Default is song = 0.

//Initializes all needed global variables and server handles
void initServer(ESP8266WebServer* server, LED* strip, SegmentDriver* display, DHTsensor* SENSOR) {
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

  SERVER->on("/getSensorReading", getSensorReading);

  SERVER->onNotFound(handleNotFound); //code 404
}

/*
* This function initialises the NTP client, and sets CurrentTime to the provided time struct pointer.
*/
void initNTP(NTPClient* timeClient, myTM* currentTime){
  TimeClient = timeClient;
  CurrentTime = currentTime;
  TimeClient->begin();
  TimeClient->setTimeOffset(3600);
}

void startServer() {
  SERVER->begin();
}

void handleClients() {
  SERVER->handleClient();
}

/*
* Function to connect to wifi. 
* INPUT: String ssid, String password and a WifiMulti pointer.
* No outputs.
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

//root/index page
void handleRoot() {
  SERVER->send(200, "text/html", index_html);
}

// Mode1 sets the LED to a static, predefined color.
void handleMode1() {
  LED_STRIP->setLEDStripHex(0xFF0000); // Set color to full red
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

// Mode2 sets the LED to a static, predefined color.
void handleMode2() {
  LED_STRIP->setLEDStripHex(0x0000FF); // Set color to full red
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

void handleOff() {
  LED_STRIP->setLEDStripHex(0x000000); // Set all color to black, effectively turning off the strip
  delay(40);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

// Display custom color 
void handleStaticColor() {
  String incomingHex = SERVER->arg("staticColor"); // Store the incoming argument in a string
  CURRENT_COLOR = incomingHex; // Set the CURRENT_COLOR variable to the incoming color
  String staticColor = incomingHex.substring(1).c_str(); // Remove the # from the string
  long colorValue = hexToDec(staticColor); // Convert from string hex to a long value
  Serial.println(staticColor); 
  Serial.println(colorValue, HEX);
  LED_STRIP->setLEDStripHex(colorValue); // Set the LED strip
  delay(10);
  SERVER->sendHeader("Location", "/"); // Redirect to root
  SERVER->send(STATUSCODE_SEEOTHER); // Send redirect code
}

//Sets alarm to user defined time
void handleSetAlarm() {
  timeNotFormated = SERVER->arg("alarmTime");
  dateNotFormated = SERVER->arg("alarmDate");
  Serial.println(dateNotFormated);
  Serial.println(timeNotFormated);
  int hour = timeNotFormated.substring(0, 2).toInt();
  int minute = timeNotFormated.substring(3, 5).toInt();
  int day = dateNotFormated.substring(8, 10).toInt();
  int month = dateNotFormated.substring(5, 7).toInt();
  int year = dateNotFormated.substring(0, 4).toInt();
  Serial.println(year, DEC);
  Serial.println(month, DEC);
  Serial.println(day, DEC);
  Serial.println(hour, DEC);
  Serial.println(minute, DEC);
  SERVER->sendHeader("Location", "/");
  SERVER->send(303);
}

//returns currently displayed color
void getCurrentColor() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(CURRENT_COLOR));
}

//returns time and date of currently set alarm
void getAlarmDateAndTime() {
  SERVER->send(STATUSCODE_OK, "text/plain", dateNotFormated + "#" + timeNotFormated);
  //Serial.println(dateNotFormated + "#" + timeNotFormated);
}

//returns currently playing melody
void getCurrentSong() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(currentSong));
}

//sets melody to be played to the one defined by the user
void handleSetWakeUpSong() {
  currentSong = SERVER->arg("songID").toInt();
  Serial.println(currentSong);
  SERVER->sendHeader("Location", "/");
  SERVER->send(303);
}

//Updates time displayed on the page
void updatePage(){
  //Serial.println(weekDays[day] + "#" + String(hour) + "#" + String(minute));
  SERVER->send(STATUSCODE_OK, "text/plain", weekDays[CurrentTime->day] + "#" + String(CurrentTime->hour) + "#" + String(CurrentTime->minute));
}

//Updates current time values
void updateTime(){
  TimeClient->update();
  CurrentTime->hour = TimeClient->getHours();
  CurrentTime->minute = TimeClient->getMinutes();
  CurrentTime->day = TimeClient->getDay();

  SEGMENT->setClock(CurrentTime->hour, CurrentTime->minute);
}

//returns current readings from the DHT and photo sensors
void getSensorReading()
{
  char tmpT[5];
  char tmpH[5];
  float tmpTF, tmpHF;
  String lumen = String(sensor->smoothLumen());
  tmpTF = sensor->smoothTempDHT();
  tmpHF = sensor->smoothHumiDHT();
  
  dtostrf(tmpTF, 4, 1, tmpT);
  dtostrf(tmpHF, 4, 1, tmpH);
  
  Serial.print(tmpT);
  Serial.print("#");
  Serial.print(tmpH);
  Serial.print("#");
  Serial.println(lumen);
  
  SERVER->send(STATUSCODE_OK, "text/plain", String(tmpT) + "#" + String(tmpH) + "#" + lumen);
  /* Units for sensor data:
   * Temperature (tmpT): Celcius
   * Humidity (tmpH): Percent
   * Light (lumen): Lumen
   */
}

//404 error when no mathing handle is found
void handleNotFound() {
  SERVER->send(404, "text/plain", "404: Not found");  // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
