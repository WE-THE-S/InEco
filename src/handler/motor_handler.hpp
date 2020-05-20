#ifndef __MOTOR_HANDLER_HPP__
#define __MOTOR_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/handler/gpio_handler.hpp"

class MotorHanlder : public GpioHandler {
    public:
    MotorHanlder(gpio_num_t _motor) : GpioHandler(_motor) {

    }

    MotorHanlder() : MotorHanlder(MOTOR_DEFAULT_PIN) {

    }

    void messageRecv(const device_communication_message_t const message){
        if(message.type == MESSAGE_TYPE::RUN_MOTOR){
            motor_message_t motor;
            motor.message = message.message;
            switch(motor.status){
                case MOTOR_STATUS::MOTOR_OFF : {
                    digitalWrite(this->pin, LOW);
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON :{
                    digitalWrite(this->pin, HIGH);
                }
            }
        }
    }
};

#endif