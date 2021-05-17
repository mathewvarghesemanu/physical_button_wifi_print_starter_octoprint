/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "constants.h"
#include <ArduinoJson.h>
#include "constants.h"


// #define apiurl "http://10.0.0.30/api/job"
// #define STASSID "NETGEAR87"
// #define STAPSK "quaintsocks941"
// #define authkey "A21E15F2F9864673A500081DE8F66AA6"

const int led_pin=D4;
const int button_pin=D1;
const int buzzerpin=D5;
int postDataToServer();

void ICACHE_RAM_ATTR button_press_function ();

void button_press_function(){
  // digitalWrite(led_pin, 1); //internal LED is lit up untill the server returns a value
  postDataToServer();
  // digitalWrite(led_pin, 0);
  

}

void getDataFromServer()

  {
  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    // configure target server and url
    http.begin(client,apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    //    http.setAuthorization(authkey);
    //    http.setAuthorization("X-Api-Key", authkey);


    Serial.println("[HTTP] GET...\n");
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
          Serial.println("Printing");
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
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    Serial.println("Posting JSON data to server...");
    http.begin(client,  apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    StaticJsonDocument<200> doc;
    // Add values in the document
    
    doc["command"] = "start";

    String requestBody;
    serializeJson(doc, requestBody);

    int httpResponseCode = http.POST(requestBody); //send command to start print to octopi server
    response_code=httpResponseCode;
    if (httpResponseCode ==201 || httpResponseCode==204){
      tone(buzzerpin, 1000, 1000);
    }

    if (httpResponseCode > 0) {
      Serial.println(httpResponseCode);
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

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Module started...");

  // attachInterrupt(digitalPinToInterrupt(button_pin), button_press_function, FALLING);
}


void loop() {
  if (digitalRead(button_pin)==LOW){
    button_press_function();
  }
    delay(1000);
}
