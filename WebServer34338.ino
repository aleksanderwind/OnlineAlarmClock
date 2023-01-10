#include <JsonListener.h>
#include <JsonStreamingParser.h>
#include <ArduinoJson.h>
#include <SimpleDHT.h>
#include <DHT.h>
#define DHTTYPE DHT11
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h> // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h> // Include the WebServer library


ESP8266WiFiMulti wifiMulti;

// Create an instance of the server
ESP8266WebServer server(80);

const int DHTPin = 5;
DHT dht(DHTPin, DHTTYPE);

const int led = 2;
const int led2 = 16;

WiFiClient client;

void handleRoot();
void handleLED();
void handleNotFound();

void setup()
{
  dht.begin();
  Serial.begin(115200);
  delay(10);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, 1);

  // Connect to WiFi network
  Serial.println();
  wifiMulti.addAP("IoTFotonik", ""); // add Wi-Fi networks youwant to connect to
  wifiMulti.addAP("Hot MILF nearby", "14151415");
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

  server.on("/", HTTP_GET, handleRoot);
  server.on("/LED", HTTP_POST, handleLED);
  server.on("/TEMP", HTTP_GET, handleTEMP);
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
{ // When URI / is  requested, send a web page with a button to toggle the LED
  char * htmlcode = "<html><head><title>Internet of Things - Demonstration</title><meta charset=\"utf-8\" \/> \</head><body><h1>Webserver i 34302 Introduktion til Cyberteknologi</h1> \<p>Internet of Things (IoT) er \"tingenes Internet\" - dagligdags ting kommer på nettet og får ny værdi. Det kan løse mange udfordringer.</p> \<p>Her kommunikerer du med en webserver på en lille microcontroller af typen Arduino, som i dette tilfælde styrer en digital udgang, som du så igen kan bruge til at styre en lampe, en ventilator, tænde for varmen eller hvad du lyster</p> \<p>Klik på nedenstående knap for at tænde eller slukke LED på port D2</p> \<form action=\"/LED\" method=\"POST\" ><input type=\"submit\" value=\"Skift tilstand på LED\" style=\"width:500px; height:100px; font-size:24px\"></form> \<p>Med en Arduino ESP8266 kan du lave et hav af sjove projekter</p> \<p>Nu skal du udvide denne webserver med mere funktionalitet</p> \<p>F.eks. skal du opbygge funktionalitet således at nedenstående knap fører dig til en side som viser temperaturer, målte og hentede.</p> \<form action=\"/TEMP\" method=\"GET\" ><input type=\"submit\" value=\"Temperaturinformation\" style=\"width:500px; height:100px; font-size:24px\"></form> \</body></html>";
  server.send(200, "text/html", htmlcode);
}

void handleLED()
{ // If a POST request  is made to URI / LED
  digitalWrite(led, !digitalRead(led)); // Change the state  of the LED
  digitalWrite(led2, !digitalRead(led2));
  server.sendHeader("Location", "/"); // Add a header to  respond with a new location for the browser to go to the home  page again
  server.send(303); // Send it back to  the browser with an HTTP status 303 (See Other) to redirect
}

void handleTEMP()
{
  //Char array that will be sent to the client is declared
  char str[500];
  const size_t capacity = 2048;
  const String endpoint =
    "http://api.openweathermap.org/data/2.5/weather?q=Kongens%20Lyngby,dk";
  DynamicJsonDocument doc(capacity);
  HTTPClient http;
  http.begin(client, endpoint +
             "&APPID=42c732ff5116bc7cbcd147c4b007dfc2&units=metric");
  int httpCode = http.GET(); //Make the request
  String payload = http.getString();
  deserializeJson(doc, payload);
  JsonObject main = doc["main"];
  float main_temp = main["temp"];
  float main_humid = main["humidity"];

  //The three variables for temperature and humidity are  declared, and initialized as being values of the input received  by the DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //The values of the floats are inserted into the HTML code by use of sprintf
  sprintf(str,"<html><head><title>Temperaturinformation</title><meta charset=\"utf-8\" \/> \</head><body><h1>Vejrinformation:</h1> \<p>Temperatur indenfor: %.2f og Fugtighed indenfor: %.2f</p>\<p>Temperatur udenfor: %.2f og Fugtighed udenfor: %.2f</p>\</body></html>", t, h, main_temp, main_humid);
  server.send(200, "text/html", str);
}

void handleNotFound()
{
  char * code404 = "<html><head><title>Error 404</title><meta charset=\"utf-8\" \/> \</head><body><h1>Error 404</h1> \<p>Alcohol not found.</p> \<p>Du er DTU student for helvede... Find noget sprut!</p> \</body></html>";
  server.send(404, "text/html", code404); // Send HTTP status  404 (Not Found) when there's no handler for the URI in the  request
}
