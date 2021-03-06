/*
physical_button_wifi_print_starter_octoprint

Easily start 3D printss using a physical button connected to an ESP32

Switch connected to pin 13

Created 29-04-2020
By Mathew Varghese

https://github.com/mathewvarghesemanu/physical_button_wifi_print_starter_octoprint

*/

// #include <ESP8266WiFi.h>
// #include <ESP8266WiFiMulti.h>

// #include <ESP8266HTTPClient.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
// #include <String.h>
// #include <HTTPClient.h>
// #include <WiFiMulti.h>

const char* AP_SSID = "NETGEAR87"; //change ssid
const char* AP_PWD = "quaintsocks941"; //change password
const char* apiurl="http://10.0.0.30/api/job";
const char* authkey="A21E15F2F9864673A500081DE8F66AA6";
const int led_pin=2;
const int button_pin=D1;
const int buzzerpin=D2;

ESP8266WiFiMulti WiFiMulti;

void getDataFromServer()

  {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    // configure target server and url
    bool http.begin (apiurl);
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
        
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }


}



int postDataToServer() {
int response_code;
response_code=0;
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("wifi connected...");
    HTTPClient http;
    Serial.println("Posting JSON data to server...");
    bool begin(apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    StaticJsonDocument<200> doc;
    // Add values in the document
    
    doc["command"] = "start";

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody); //send command to start print to octopi server
    response_code=httpResponseCode;
    if (httpResponseCode > 0) {
      Serial.print(httpResponseCode);
      String response = http.getString();
       delay(1000);
      Serial.println(httpResponseCode);
      Serial.println(response);
        http.end();
        delay(9000);
        
      return (response_code);
    }
  }
}


void setup() {
  pinMode(button_pin, INPUT_PULLUP); 
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  pinMode(buzzerpin,OUTPUT);
  digitalWrite(buzzerpin,LOW);
  Serial.begin(115200);
  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
  Serial.println("Module started...");

  attachInterrupt(digitalPinToInterrupt(button_pin), button_press_function, FALLING);
}

void button_press_function(){
  digitalWrite(led_pin, 1); //internal LED is lit up untill the server returns a value
  postDataToServer();
  digitalWrite(led_pin, 0);
  tone(buzzerpin, 1000, 100);

}

void loop() {
  // getDataFromServer();
  delay(10000);
}
