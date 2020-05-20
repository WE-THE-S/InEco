#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/handler/gpio_handler.hpp"

using namespace std;

class LedHanlder : public MessageGpioControlHandler {
    public:
    LedHanlder(gpio_num_t _led) : MessageGpioControlHandler(_led) {

    }

    LedHanlder() : LedHanlder(LED_CONTROL_DEFAULT_PIN) {

    }

    void onMessage(const device_communication_message_t message){
        if(message.type == MESSAGE_TYPE::SET_COLOR){
            led_message_t led;
            led.message = message.message;
        }
    }
};

#endif