#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "./config.hpp"
#include "./struct/packet.hpp"
#include "./handler/led_handler.hpp"
#include "./handler/motor_handler.hpp"
#include "./service/alarm.hpp"
#include "./service/water_level.hpp"
#include "./utils/broadcast.hpp"
#include "./translate/led_translate.hpp"
#include "./translate/control_translate.hpp"
#include "./struct/translate.hpp"

#if LED_BOARD == 1
  LedHanlder led;
  MotorHanlder motor;
  LedTranslate translate;
#elif CONTROL_BOARD == 1
  WebServer server(80);
  ControlTranslate translate;
  Alarm ledAlarm;
  WaterLevel waterLevel;
#endif

void setup() {
  #if LED_BOARD == 1
    auto instance = Broadcast<device_communication_message_t>::getInstance();
    instance->add(&led);
    instance->add(&motor);
  #elif CONTROL_BOARD == 1
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
    }
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    MDNS.begin("ineco");

    server.on("/motor/on", []() {
      server.send(200, "text/plain", "hello from esp32!");
    });

    server.on("/motor/set/{}", []() {
      std::String arg = server.pathArg(0);
      long time = atol(arg.c_str());
      server.send(200, "text/plain", "User: '" + arg + "'");
    });
  
    server.begin();
    auto instance = Broadcast<service_signal_t>::getInstance();
    instance->add(&ledAlarm);
    instance->add(&waterLevel);
  #endif
}

void loop() {
  #if LED_BOARD == 1
    translate.recv();
  #elif CONTROL_BOARD == 1
    server.handleClient();
    waterLevel.execute();
  #endif
}