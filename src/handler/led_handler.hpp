#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/handler/gpio_handler.hpp"

using namespace std;

class LedHanlder : public GpioHandler {
    public:
    LedHanlder(gpio_num_t _led) : GpioHandler(_led) {

    }

    LedHanlder() : LedHanlder(LED_CONTROL_DEFAULT_PIN) {

    }
    
    void messageRecv(const device_communication_message_t const message){
        if(message.type == MESSAGE_TYPE::SET_COLOR){
            auto led = new led_message_t;
            led->message = message.message;
            delete led;
        }
    }
};

#endif