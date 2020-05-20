#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/handler/gpio_handler.hpp"

using namespace std;

class MotorHanlder : public GpioHandler {
    public:
    MotorHanlder(gpio_num_t _motor) : GpioHandler(_motor) {

    }

    MotorHanlder() : MotorHanlder(MOTOR_DEFAULT_PIN) {

    }
    
    void messageRecv(const device_communication_message_t const message){
        if(message.type == MESSAGE_TYPE::RUN_MOTOR){
            auto motor = new motor_message_t;
            motor->message = message.message;
            switch(motor->status){
                case MOTOR_STATUS::MOTOR_OFF : {
                    digitalWrite(pin, LOW);
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON :{
                    digitalWrite(pin, HIGH);
                }
            }
        }
    }
};

#endif