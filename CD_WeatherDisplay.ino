/*

  Weather display + maybe sunrise/sunset light.

  Thanks for the help, Tom!

  by Bianca Gan, 4/8/23
  updated: 4/28/23


*/

#include <WiFiNINA.h>   // for Nano
// #include <WiFi101.h>    // for MKR1000
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal.h>
#include "arduino_secrets.h"

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

WiFiSSLClient netSocket;
// WiFiClient netSocket;
const int port = 443;   // HTTPS standard port

// WeatherApi.com
const char serverAddress[] = "api.weatherapi.com";
String route = "/v1/current.json?key={key}&q={latlong}";
String location = "40.69805897384927,-73.98644816137055";

HttpClient http = HttpClient(netSocket, serverAddress, port);

long lastRequestTime = 0;
const long requestInterval = 60000 * 2;    // every 2 minutes

String jsonBuffer;

void setup() {
  Serial.begin(9600);

  // LCD's number of columns and rows:
  lcd.begin(20, 4);

  if(!Serial) delay(3000);

  // Initialize wifi, if not connected:
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to the network: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  // Print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());

  // First request:
  connectToServer();
}


void loop() {
  // Make an HTTP request once every interval:
  if(millis() - lastRequestTime > requestInterval){
    lcd.clear();

    connectToServer();

    lastRequestTime = millis();
  }
    
}


void connectToServer(){
  // Make HTTP call:
    Serial.println("Connecting to: ");
    route.replace("{key}", SECRET_API_KEY);
    route.replace("{latlong}", location);
    Serial.print(serverAddress);
    Serial.println(route);
    http.get(route);  // make a GET request

    int statusCode = http.responseStatusCode();
    String response = http.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);

    JSONVar myObject = JSON.parse(response);

    if(JSON.typeof(myObject) == "undefined"){
      Serial.println("Parsing input failed!");
      return;
    }

    Serial.print("JSON response: ");
    Serial.println(myObject);

    // Separate values from JSON:
    // Location:
    String currLoc = myObject["location"]["name"];
    // Current temperature in F:
    int currTemp = myObject["current"]["temp_f"];
    // Current conditions:
    String currCond = myObject["current"]["condition"]["text"];
    // Wind speed:
    int currWind = myObject["wind_mph"];

    Serial.print("Location: ");
    Serial.println(currLoc);
    Serial.print("Current temp: ");
    Serial.print(currTemp);
    Serial.println(" F");
    Serial.print("Conditions: ");
    Serial.println(currCond);
    Serial.print("Wind speed: ");
    Serial.print(currWind);
    Serial.println(" mph");

    
    // Write to LCD screen:
    lcd.print(currLoc);
    lcd.setCursor(0, 1);    // move to the second line
    lcd.print("Temp is ");
    lcd.print(currTemp);
    lcd.print(" F");
    lcd.setCursor(0, 2);    // move to the third line
    lcd.print("It is ");
    lcd.print(currCond);
    lcd.setCursor(0, 3);    // move to the fourth line
    lcd.print("Wind: ");
    lcd.print(currWind);
    lcd.print(" mph");


}
  


void connectToNetwork(){
  // Initialize wifi, if not connected:
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("Connecting to the network: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  // Print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());
  
}









