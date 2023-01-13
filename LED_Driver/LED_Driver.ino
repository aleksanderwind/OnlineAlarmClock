#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>  // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>  // Include the WebServer library
#include <ESP8266mDNS.h>       // Include the mDNS library
#include <Adafruit_NeoPixel.h>

#define LED_PIN 14
#define NUM_LEDS 5
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define STATUSCODE_OK 200
#define STATUSCODE_SEEOTHER 303

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB);

ESP8266WiFiMulti wifiMulti;
// Create an instance of the server
ESP8266WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<html>

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>Online Alarm Clock</title>
    <meta name="final project" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <h1> Online Alarm Clock</h1>
</head>
   

<style> 
    input.mode1[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #ff0000;
        color: white;
        text-align: center;
        margin-right: 10px;
    }

    input.mode2[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #0000ff;
        color: white;
        text-align: center;
        margin-right: 10px;
    }

    input.off[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #000000;
        color: white;
        text-align: center;
    }

    input.set[type="submit"]{
        float: left;
        height: 25px;
        width: fit-content;
        background-color: #000000;
        color: white;
        text-align: center;
        margin-top: 4px;
    }

    input[type="color"] {
        opacity: 0;
        display: block;
        width: 32px;
        height: 32px;
        border: none;
    }
    #color-picker-wrapper {
        float: left;
        border: 10px;
        border-radius: 50%;
        margin-right: 10px;
    }

</style>

<body> 
    <p> Select mode or turn LED strip off: </p>
    <div>
        <form action="/mode1">
            <!--<label for = "mode1"> Activate mode 1 </label><br> -->
            <input class = mode1 type="submit" value = "Mode 1">
        </form>
    
        <form action="/mode2">
            <!--<label for = "mode2"> Activate mode 2 </label><br> -->
            <input class = mode2 type="submit" value = "Mode 2">
        </form>
    
        <form action="/off">
            <!--<label for = "off"> Turn off </label><br> -->
            <input class = off type="submit" value = "Off"><br><br>
        </form>
    </div>
    
    <label for="staticColor">Select color of LED strip:</label>
    <form action="/setStaticColor">
        <div id="color-picker-wrapper">
            <input type="color" value="#ff0000" id="staticColor" name="staticColor">
        </div>
        <input class = set type="submit" value="Set"> 
    </form>

    <script>
        var color_picker = document.getElementById("staticColor");
        var color_picker_wrapper = document.getElementById("color-picker-wrapper");
        color_picker.onchange = function () {
            color_picker_wrapper.style.backgroundColor = color_picker.value;
        }
        color_picker_wrapper.style.backgroundColor = color_picker.value;
    </script>
    
</body>

</html>   )rawliteral";

void handleRoot();
void handleMode1();
void handleMode2();
void handleOff();
void handleStaticColor();
void handleNotFound();
void wifiINIT(String ssid, String password);
void setLEDStrip(int r, int g, int b);
void setLEDStripHex(long hex);

void setup() {
  Serial.begin(115200);
  delay(10);

  pixels.begin();
  setLEDStrip(0,0,0);
  wifiINIT("Jesper", "12345677");

  server.on("/", HTTP_GET, handleRoot);
  server.on("/mode1", HTTP_GET, handleMode1);
  server.on("/mode2", HTTP_GET, handleMode2);
  server.on("/off", HTTP_GET, handleOff);
  server.on("/setStaticColor", handleStaticColor);
  server.onNotFound(handleNotFound);

  // Start the server
  server.begin();
  Serial.println("Server started");
  pixels.clear();
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

long hexToDec(String hexString) {
  long decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt -= 48;
    if (nextInt >= 65 && nextInt <= 70) nextInt -= 45;
    if (nextInt >= 97 && nextInt <= 102) nextInt -= 87;
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}

void setLEDStrip(int r, int g, int b) {
  /*
   * Description
   * Takes three integers r, g and b and sets the led strip to the color specified by that three numbers.
   * 
   * INPUT: r,g,b number between 0 and 255
   * OUTPUT: none
   * 
   * Example:
   * setLEDStrip(255,0,0);
   * 
   * Sets the color of the LED strip to red.
  */

  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
    pixels.show();
}

void setLEDStripHEX(long colorValue) {
  /*
   * Description
   * Takes a long value at sets the RGB strip to that color.
   * 
   * INPUT: long colorValue
   * OUTPUT: none
   * 
   * Example:
   * setLEDStrip(0xFF0000);
   * 
   * Sets the color of the LED strip to red.
  */

  int r, g, b;
  r = colorValue >> 16;
  g = (colorValue & 0x00FF00) >> 8;
  b = colorValue & 0xFF;
  setLEDStrip(r,g,b);
}

void handleMode1() {
  setLEDStripHEX(0xFF0000);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleMode2() {
  setLEDStripHEX(0x0000FF);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleOff() {
  setLEDStripHEX(0x000000);
  delay(40);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleStaticColor() {
  String staticColor = server.arg("staticColor").substring(1).c_str();
  long colorValue = hexToDec(staticColor);
  Serial.println(staticColor);
  Serial.println(colorValue, HEX);
  /*
  Serial.println(colorValue>>16);
  Serial.println((colorValue & 0x00ff00)>>8);
  Serial.println((colorValue & 0x0000ff));
  setColorHEX(colorValue);*/
  setLEDStripHEX(colorValue);
  delay(10);
  //FastLED.show();
  server.sendHeader("Location", "/");
  server.send(STATUSCODE_SEEOTHER);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");  // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
