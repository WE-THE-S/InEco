#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include "./config.hpp"
#include "./handler/led/led_handler.hpp"
#include "./handler/led/motor_handler.hpp"
#include "./service/alarm.hpp"
#include "./service/water_level.hpp"
#include "./service/motor_interval.hpp"
#include "./utils/broadcast.hpp"
#include "./translate/led_translate.hpp"
#include "./translate/control_translate.hpp"
#include "./struct/packet.hpp"
#include "./struct/translate.hpp"

#define CONTROL_BOARD 1
#define LED_BOARD 0

#if LED_BOARD == 1
  LedHanlder led;
  MotorHanlder motor;
  LedTranslate translate;
#elif CONTROL_BOARD == 1
  AsyncWebServer server(80);
  ControlTranslate translate;
  Alarm ledAlarm;
  MotorInterval motorInterval;
  WaterLevel waterLevel;
  void notFound(AsyncWebServerRequest* request){
    request->send(404, "text/html", "not found");
  }
#endif

void setup() {
  #if LED_BOARD == 1
    auto instance = Broadcast<device_communication_message_t>::getInstance();
    instance->add(&led);
    instance->add(&motor);
  #elif CONTROL_BOARD == 1
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    MDNS.begin("ineco");

    server.onNotFound(notFound);

    server.on("^\\/led$", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", LED_SET_HTML);
    });

    server.on("^\\/led\\/power\\/([a-zA-Z]+)$", HTTP_GET, [](AsyncWebServerRequest* request) {
      const String onOff = request->pathArg(0);
      request->send(200, "text/plain", onOff);
    });

    server.on("^\\/motor$", HTTP_GET, [](AsyncWebServerRequest* request) {
      request->send(200, "text/html", MOTOR_SET_HTML);
    });

    server.on("^\\/motor\\/power\\/([a-zA-Z]+)$", HTTP_GET, [](AsyncWebServerRequest* request) {
      const String onOff = request->pathArg(0);
      service_signal_t signal;
      motor_interval_service_signal_t message;
      message.isIntervalSet = false;
      message.onOff = onOff.equalsIgnoreCase("on");
      signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
      signal.value = message.value;
      Broadcast<service_signal_t>::getInstance()->broadcast(signal);
      request->send(200, "text/html", String(message.onOff));
    });

    server.on("^\\/motor\\/set\\/enable\\/([a-zA-Z]+)\\/time\\/([0-9]+)\\/span\\/([0-9]+)$", HTTP_GET, [](AsyncWebServerRequest* request) {
      const String enable = request->pathArg(0);
      const String time = request->pathArg(1);
      const String span = request->pathArg(2);
      service_signal_t signal;
      motor_interval_service_signal_t message;
      message.isIntervalSet = true;
      message.intervalEnable = enable.equalsIgnoreCase("on");
      message.intervalSpan = static_cast<uint16_t>(atoi(span.c_str()));
      message.intervalTime = static_cast<uint16_t>(atoi(time.c_str()));
      signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
      signal.value = message.value;
      Broadcast<service_signal_t>::getInstance()->broadcast(signal);
      request->send(200, "text/plain", enable);
    });
  
    server.begin();

    auto instance = Broadcast<service_signal_t>::getInstance();
    instance->add(&ledAlarm);
    instance->add(&waterLevel);
    instance->add(&translate);
    instance->add(&motorInterval);
  #endif
}

void loop() {
  #if LED_BOARD == 1
    translate.recv();
  #elif CONTROL_BOARD == 1
    waterLevel.execute();
    motorInterval.execute();
  #endif
}