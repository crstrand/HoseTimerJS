#include <Arduino.h>
#include "js-countdown-timer-html.h"

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#define ON true
#define OFF false
#define RELAY_PIN 2

char ssid[] = "Strandlund_IoT";     //  your network SSID (name)
char pass[] = "3066962933";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint32_t tDuration = 3000; // remaining time in milliseconds
uint32_t tRemain = 0;
uint32_t fromClient = 0;

AsyncWebServer server(80);

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    request->addInterestingHeader("Access-Control-Allow-Origin: *");
    return true;
  }

  void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    // the number is sent as an array of characters ie: 1200 = '1','2','0','0'
    uint32_t newDuration=0;
    if(request->method()==HTTP_POST)
    {
      for(size_t i=0; i<len; i++){
        newDuration+=((data[i]-'0')*pow(10,(len-i-1)));
      }
      tDuration = newDuration;
      Serial.printf("handleRequest: POST\nnewDuration = %d\n",newDuration);
    }
  }

  void handleRequest(AsyncWebServerRequest *request)
  {

    //Serial.printf("url: [%s]\t",request->url().c_str());
    AsyncResponseStream *response;
    response = request->beginResponseStream("text/html");
    if(request->method()==HTTP_GET)
    {
      if(request->url() == "/tRemain")
        response->print(String(tRemain)); // reply with the current value of tRemain
      else
        response->print(FPSTR(clockhtml)); // reply with the main page

      request->send(response);
    }
    if(request->method()==HTTP_POST)
    {
      response->print(String(tRemain)); // reply with the current value of tRemain
      request->send(response);
    }
  }
};

void relay_state(bool state)
{
  digitalWrite(RELAY_PIN,state);
  Serial.printf("Relay: %s\n",(state==true?"ON":"OFF"));
}

void setup(){
  pinMode(RELAY_PIN,OUTPUT);
  relay_state(OFF);

  Serial.begin(74880);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("hosetimer");
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  
  // use one handler for everything as this is a very simple webserver
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_STA_FILTER);
  //more handlers...
  server.begin();

  Serial.print("server available at ");
  Serial.println(WiFi.localIP());
}

uint32_t prevDuration=tDuration;
unsigned long msecStart;
unsigned long msecInterval=1000;
unsigned long tStop=0;
bool timerState=OFF;
bool prevState=OFF;

void loop(){
  if(millis()-msecStart>=msecInterval)
  {
    msecStart = millis();
    if(tRemain<=0) // we're done
      timerState = OFF;
    else
    {
      // update tRemain
      tRemain = tStop-millis();
      if(tRemain<msecInterval)
        tRemain=0;
    }
    if(timerState!=prevState)
    {
      prevState=timerState;
      relay_state(timerState);
    }
    //Serial.printf("tRemain = %d\n",tRemain);
  }
  if(prevDuration!=tDuration)
  {
    Serial.printf("New Duration received: %d\n",tDuration);
    prevDuration = tDuration;
    tStop = tDuration + millis();
    tRemain = tDuration;
    timerState = (tDuration>0);
  }
}