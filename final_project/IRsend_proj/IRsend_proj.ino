/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <IRremote.h>

const char* hardwareid = "hw1";
const char* ssid = "winnone";
const char* password = "winnone12";
const char* urlget = "https://ecourse.cpe.ku.ac.th/exceed11/api/hardware/commands/";
const int sizee = 2*JSON_OBJECT_SIZE(4);

char commandid[50],str[50],value[50];

StaticJsonDocument<sizee> JSONPost;
StaticJsonDocument<sizee> JSONGet;

//const char* irsig="0xa9016899";

IRsend irsend; 

void wificonnect(){
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to the WiFi network");
}

void _get(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(urlget);
    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization","Bearer hw1");
    int httpCode = http.GET();
    
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      DeserializationError err = deserializeJson(JSONGet,payload);
      if(err){
        Serial.print(F("deserializeJson() failed with code"));
        Serial.println(err.c_str());
      }

      else{
        Serial.print("GET ");
        Serial.println(httpCode);
        strcpy(commandid,JSONGet["commandId"]);
        strcpy(value,JSONGet["value"]);
        unsigned long long a=strtoul(value,NULL,16);
        Serial.print("value ");
        Serial.println(value);
        for (int i = 0; i < 3; i++) {
            irsend.sendNEC(a, 32);
            delay(20);
        }
        _post(commandid);
        //strcpy(hwid,JSONGet["hardwareId"]);
      }
    }
    
   else{
      Serial.println(httpCode);
      //wificonnect();
   }
 }
  delay(100);
}

void _post (const char* commamdid){
  if(WiFi.status() == WL_CONNECTED){
    String urlpost = String("https://ecourse.cpe.ku.ac.th/exceed11/api/hardware/command/"+String(commandid)+"/ack/");
    HTTPClient http;
    http.begin(urlpost);
    http.addHeader("Content-Type","application/json");
    http.addHeader("Authorization","Bearer hw1");

    JSONPost["commandId"]=commandid;
    serializeJson(JSONPost,str);
    int httpCode = http.POST(str);
    
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      Serial.print("POST ");
      Serial.println(httpCode);
      Serial.println(payload);
    }
   else{
      Serial.println(httpCode);
      //wificonnect();
   }
 }
  delay(100);
}

void setup()
{
  Serial.begin(9600);
  wificonnect();
  delay(1000);
}

void loop() {
  
	_get();
  delay(500);
}
