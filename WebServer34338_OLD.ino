#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h> // Include the WebServer library
#include <NTPClient.h>
#include <WiFiUdp.h>

ESP8266WiFiMulti wifiMulti;

// Create an instance of the server
ESP8266WebServer server(80);

const int led = 2;
const int led2 = 16;

WiFiClient client;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void handleRoot();
void handleLED();
void handleTime();
void handleNotFound();

void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, 1);

  // Connect to WiFi network
  Serial.println(); 
  wifiMulti.addAP("Hot MILF nearby", "14151415"); // add Wi-Fi networks youwant to connect to
  Serial.println();
  Serial.print("Connecting ...");
  //WiFi.begin(ssid, password);
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected to ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(0);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/TIME", HTTP_POST, handleTime);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop()
{
  // Check if a client has connected
  server.handleClient();
}

void handleRoot()
{ 
  // When URI / is  requested, send a web page with a button to toggle the LED
  char * htmlcode = "<html><head><title>Website - Generic</title><meta charset=\"utf-8\" \/> \</head><body><h1>Generic Website</h1> \<p>Press the button to turn the LED's ON/OFF</p> \<form action=\"/LED\" method=\"POST\" ><input type=\"submit\" value=\"LED ON/OFF\" style=\"width:500px; height:100px; font-size:24px\"></form> \<form action=\"/TIME\" method=\"POST\" ><input type=\"submit\" value=\"Get current time and date\" style=\"width:500px; height:100px; font-size:24px\"></form> \</body></html>";
  server.send(200, "text/html", htmlcode);
}

void handleLED()
{ 
  // If a POST request  is made to URI / LED
  digitalWrite(led, !digitalRead(led)); // Change the state  of the LED
  digitalWrite(led2, !digitalRead(led2));
  server.sendHeader("Location", "/"); // Add a header to  respond with a new location for the browser to go to the home  page again
  server.send(303); // Send it back to  the browser with an HTTP status 303 (See Other) to redirect
}

void handleTime()
{
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  timeClient.setTimeOffset(3600);

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds(); 
  String wkDy = weekDays[timeClient.getDay()]; 

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;

  char weekDay[10];
  wkDy.toCharArray(weekDay, wkDy.length()+1);

  char str[256];
  sprintf(str,"<html><head><title>Time Information</title><meta charset=\"utf-8\" \/> \</head><body><h1>Date/time:</h1> \<p>Time: %d:%d:%d</p>\<p>Date: %s, %d/%d/%d</p>\<p>Time retrieved from <a href=\"pool.ntp.org\">pool.ntp.org</a></p>\<img src=\"https://media.tenor.com/iRRAJt3llV4AAAAC/its-time-to-stop-stop.gif\">\</body></html>", currentHour, currentMinute, currentSecond, weekDay, monthDay, currentMonth, currentYear);
  server.send(200, "text/html", str);
}

void handleNotFound()
{
  char * code404 = "<html><head><title>Error 404</title><meta charset=\"utf-8\" \/> \</head><body><h1>Error 404</h1> \<p>Alcohol not found.</p> \<p>Du er DTU student for helvede... Find noget sprut!</p> \</body></html>";
  server.send(404, "text/html", code404); // Send HTTP status  404 (Not Found) when there's no handler for the URI in the  request
}
