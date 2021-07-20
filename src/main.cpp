#include <Arduino.h>
#include "js-countdown-clock-html.h"

#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"

char ssid[] = "Strandlund_IoT";     //  your network SSID (name)
char pass[] = "3066962933";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint32_t tRemain = 3000; // remaining time in milliseconds
uint32_t fromClient = 0;

DNSServer dnsServer;
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
    for(size_t i=0; i<len; i++){
      newDuration+=((data[i]-'0')*pow(10,(len-i-1)));
    }
    tRemain = newDuration;
    Serial.printf("\nnewDuration = %d\n",newDuration);
  }

  void handleRequest(AsyncWebServerRequest *request)
  {

    Serial.printf("url: [%s]\t",request->url().c_str());
/*
    //List all parameters
    int params = request->params();
    int args = request->args();
    int headers = request->headers();
    String type = request->contentType();
    int typelen = request->contentLength();

    Serial.printf("method: [%s]\t",request->methodToString());
    Serial.printf("#args: %i\t#headers: %i\t#params: %i\n",args,headers,params);
    Serial.printf("type: %s, len=%d\n",type.c_str(),typelen);

    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){ //p->isPost() is also true
        Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    for(int i=0;i<args;i++)
      Serial.printf("ARG[%s]: %s\n", request->argName(i).c_str(), request->arg(i).c_str());
*/    
    AsyncResponseStream *response;
    if(request->method()==HTTP_GET)
    {
      response = request->beginResponseStream("text/html");
      if(request->url() == "/tRemain")
        response->print(String(tRemain)); // reply with the current value of tRemain
      else
        response->print(FPSTR(clockhtml)); // reply with the main page

      request->send(response);
    }
    else if(request->method()==HTTP_POST)
    {
      Serial.println("handleRequest: POST");
    }
  }
};

bool filterOnPOST(AsyncWebServerRequest *request) { return request->method() == HTTP_POST; }

void setup(){
  Serial.begin(74880);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }

  dnsServer.start(53, "*", WiFi.localIP());

  // set up handler to handle the HTTP_GET for tRemain
  //server.addHandler(new CaptiveRequestHandler()).setFilter(filterOnGET);

  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_STA_FILTER);
  //more handlers...
  server.begin();
  Serial.print("server available at ");
  Serial.println(WiFi.localIP());
}

uint32_t prevDuration=tRemain;

void loop(){
  if(prevDuration!=tRemain)
  {
    Serial.printf("New Duration received: %d\n",tRemain);
    prevDuration = tRemain;
  }
  dnsServer.processNextRequest();
}