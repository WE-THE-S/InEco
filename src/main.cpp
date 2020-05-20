#include <Arduino.h>
#include "./config.hpp"
#include "./struct/packet.hpp"
#include "./handler/led_handler.hpp"
#include "./handler/motor_handler.hpp"
#include "./utils/broadcast.hpp"
#include "./translate/led_translate.hpp"
#include "./translate/control_translate.hpp"
#include "./struct/translate.hpp"

#if defined(LED_BOARD)
  LedHanlder led;
  MotorHanlder motor;
  LedTranslate translate;
#elif defined(CONTROL_BOARD)
  ControlTranslate translate;
#endif

void setup() {
  #if defined(LED_BOARD)
    Broadcast* instance = Broadcast::getInstance();
    instance->add(&led);
    instance->add(&motor);
  #elif defined(CONTROL_BOARD)
    
  #endif
}

void loop() {
  translate.recv();
  #if defined(CONTROL_BOARD)
    
  #endif
}