#include "iotserver.h"

int* DHTPIN;
int* LDRPIN; //find pin

DHTsensor* sensor;

myTM* CurrentTime;

ESP8266WebServer* SERVER;

LED* LED_STRIP;

SegmentDriver* SEGMENT;
NTPClient* TimeClient;

String CURRENT_COLOR = "#FFFFFF";

String timeNotFormated = "";
String dateNotFormated = "";

String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int currentSong = 0;

void initServer(ESP8266WebServer* server, LED* strip, SegmentDriver* display, DHTsensor* SENSOR) {
  sensor = SENSOR;
  
  SERVER = server;

  LED_STRIP = strip;

  SEGMENT = display;

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

  SERVER->onNotFound(handleNotFound);
}

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

void connectToWifi(String SSID, String PASSWORD, ESP8266WiFiMulti* wifiMulti) {

  // Connect to WiFi network
  Serial.println();
  wifiMulti->addAP(SSID.c_str(), PASSWORD.c_str());  // add Wi-Fi networks you want to connect to

  Serial.println();
  Serial.print("Connecting ...");

  while (wifiMulti->run() != WL_CONNECTED) {
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
    delay(10);
  } else {
    Serial.println("Error setting up MDNS responder!");
    delay(10);
  }
}

void handleRoot() {
  SERVER->send(200, "text/html", index_html);
}

void handleMode1() {
  LED_STRIP->setLEDStripHex(0xFF0000);
  delay(10);
  //FastLED.show();
  SERVER->sendHeader("Location", "/");
  SERVER->send(STATUSCODE_SEEOTHER);
}

void handleMode2() {
  LED_STRIP->setLEDStripHex(0x0000FF);
  delay(10);
  //FastLED.show();
  SERVER->sendHeader("Location", "/");
  SERVER->send(STATUSCODE_SEEOTHER);
}

void handleOff() {
  LED_STRIP->setLEDStripHex(0x000000);
  delay(40);
  //FastLED.show();
  SERVER->sendHeader("Location", "/");
  SERVER->send(STATUSCODE_SEEOTHER);
}

void handleStaticColor() {
  String incomingHex = SERVER->arg("staticColor");
  CURRENT_COLOR = incomingHex;
  String staticColor = incomingHex.substring(1).c_str();
  long colorValue = hexToDec(staticColor);
  Serial.println(staticColor);
  Serial.println(colorValue, HEX);
  LED_STRIP->setLEDStripHex(colorValue);
  delay(10);
  //FastLED.show();
  SERVER->sendHeader("Location", "/");
  SERVER->send(STATUSCODE_SEEOTHER);
}

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

void getCurrentColor() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(CURRENT_COLOR));
}

void getAlarmDateAndTime() {
  SERVER->send(STATUSCODE_OK, "text/plain", dateNotFormated + "#" + timeNotFormated);
  //Serial.println(dateNotFormated + "#" + timeNotFormated);
}

void getCurrentSong() {
  SERVER->send(STATUSCODE_OK, "text/plain", String(currentSong));
}

void handleSetWakeUpSong() {
  currentSong = SERVER->arg("songID").toInt();
  Serial.println(currentSong);
  SERVER->sendHeader("Location", "/");
  SERVER->send(303);
}

void updatePage(){
  //Serial.println(weekDays[day] + "#" + String(hour) + "#" + String(minute));
  SERVER->send(STATUSCODE_OK, "text/plain", weekDays[CurrentTime->day] + "#" + String(CurrentTime->hour) + "#" + String(CurrentTime->minute));
}

void updateTime(){
  TimeClient->update();
  CurrentTime->hour = TimeClient->getHours();
  CurrentTime->minute = TimeClient->getMinutes();
  CurrentTime->day = TimeClient->getDay();

  SEGMENT->setClock(CurrentTime->hour, CurrentTime->minute);
}

// Skal implementeres i HTML
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

void handleNotFound() {
  SERVER->send(404, "text/plain", "404: Not found");  // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
