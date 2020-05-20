#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "./handler.hpp"

using namespace std;

class LedHanlder : protected Handler {
    private:
        gpio_num_t motorPin;
        gpio_num_t ledPin;
    
    public:
    LedHanlder(gpio_num_t _motor, gpio_num_t _led) : motorPin(_motor), ledPin(_led) {

    }
    LedHanlder() : LedHanlder(MOTOR_DEFAULT_PIN, LED_CONTROL_DEFAULT_PIN) {

    }
    void messageRecv(const device_communication_message_t const message){
        switch(message.type){
            case MESSAGE_TYPE::RUN_MOTOR : {
                auto motor = new motor_message_t;
                motor->message = message.message;
                switch(motor->status){
                    case MOTOR_STATUS::MOTOR_OFF : {
                        digitalWrite(motorPin, LOW);
                        break;
                    }
                    case MOTOR_STATUS::MOTOR_ON :{
                        digitalWrite(motorPin, HIGH);
                    }
                }
                delete motor;
                break;
            }
            case MESSAGE_TYPE::SET_COLOR : {
                auto led = new led_message_t;
                led->message = message.message;
                delete led;
                break;
            }
        }
    }
};

#endif