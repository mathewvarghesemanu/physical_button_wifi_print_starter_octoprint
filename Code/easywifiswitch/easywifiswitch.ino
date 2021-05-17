/*
physical_button_wifi_print_starter_octoprint

Easily start 3D printss using a physical button connected to an ESP32

Switch connected to pin 13

Created 29-04-2020
By Mathew Varghese

https://github.com/mathewvarghesemanu/physical_button_wifi_print_starter_octoprint

*/


#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

const char *AP_SSID = "NETGEAR87"; //change ssid
const char *AP_PWD = "quaintsocks941"; //change password
int led_pin=2;
int button_pin=15;
WiFiMulti wifiMulti;
void postDataToServer();
void setup() {
  pinMode(button_pin, INPUT_PULLUP); 
  pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, 1);
  Serial.begin(9600);

  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
  Serial.println("started...");

}

void loop() {

  if (digitalRead(button_pin) != 1)//if switch is pressed, led lits up, command sent and delay for 1.5 minutes
  {
    Serial.print("on\n");
    digitalWrite(led_pin, 1); //internal LED is lit up untill the server returns a value
    postDataToServer();
    digitalWrite(led_pin, 0);
    delay(500);
  }
  else
  {
    Serial.print("off\n");//if switch idle, led turned off. minimal delay and loop starts
    digitalWrite(led_pin, 0);
  }
  delay(500);
}

void postDataToServer() {


  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("wifi connected...");
    HTTPClient http;
    Serial.println("Posting JSON data to server...");
    http.begin("http://192.168.1.30/api/job");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", "401D82B1BD4B47D6817DBD5C8A6AD297");
    StaticJsonDocument<200> doc;
    // Add values in the document
    
    doc["command"] = "start";

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody); //send command to start print to octopi server

    if (httpResponseCode > 0) {
      Serial.print(httpResponseCode);
      String response = http.getString();
      
      Serial.println(httpResponseCode);
      Serial.println(response);

    }

  }
  
}
