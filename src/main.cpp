#include <Arduino.h>
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
    auto instance = Broadcast<service_signal_t>::getInstance();
    instance->add(&ledAlarm);
    instance->add(&waterLevel);
  #endif
}

void loop() {
  #if LED_BOARD == 1
    translate.recv();
  #elif CONTROL_BOARD == 1
    waterLevel.execute();
  #endif
}