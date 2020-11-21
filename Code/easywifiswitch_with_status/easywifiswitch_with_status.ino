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

const char *AP_SSID = "NETGEAR87"; //change ssid
const char *AP_PWD = "quaintsocks941"; //change password
String apiurl="http://10.0.0.30/api/job";
String authkey="401D82B1BD4B47D6817DBD5C8A6AD297";
int led_pin=2;
int button_pin=15;
WiFiMulti wifiMulti;
void postDataToServer();
void getDataFromServer();

void setup() {
  pinMode(button_pin, INPUT_PULLUP); 
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, 0);
  Serial.begin(9600);

  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
  Serial.println("started...");

}

void loop() {
  getDataFromServer();
  if (digitalRead(button_pin) != 1)//if switch is pressed, led lits up, command sent and delay for 1.5 minutes
  {
    digitalWrite(led_pin, 1); //internal LED is lit up untill the server returns a value
    postDataToServer();
    digitalWrite(led_pin, 0);
    
    
  }
  else
  {
    Serial.print("off\n");//if switch idle, led turned off. minimal delay and loop starts
    digitalWrite(led_pin, 0);
  }
  delay(500);
}


void getDataFromServer()

  {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url


    http.begin(client, apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    //    http.setAuthorization(authkey);
    //    http.setAuthorization("X-Api-Key", authkey);


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        //Serial.println(payload);
        if (payload.indexOf("Printing") == -1)
        {
          digitalWrite(led_pin , 0);
          for (int i=0;i<20;i++)
          {
            digitalWrite(led_pin,1);
            delay(200);
            digitalWrite(led_pin,0);
            delay(200);
          }
        }
        else
        {
          digitalWrite(led_pin , 0);
          Serial.print("Printing");
        }
        delay(5000);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(500);
}















void postDataToServer() {


  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("wifi connected...");
    HTTPClient http;
    Serial.println("Posting JSON data to server...");
    http.begin(apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    StaticJsonDocument<200> doc;
    // Add values in the document
    
    doc["command"] = "start";

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody); //send command to start print to octopi server

    if (httpResponseCode > 0) {
      Serial.print(httpResponseCode);
      String response = http.getString();
       delay(1000);
      Serial.println(httpResponseCode);
      Serial.println(response);
        http.end();
    }

  }

}
