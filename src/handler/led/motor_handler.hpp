#ifndef __MOTOR_HANDLER_HPP__
#define __MOTOR_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../../config.hpp"
#include "../../struct/packet.hpp"
#include "../../struct/handler/gpio_handler.hpp"

//motor message handler
class MotorHanlder : public MessageGpioControlHandler {
    public:
    MotorHanlder(gpio_num_t _motor) : MessageGpioControlHandler(_motor) {
        
    }

    MotorHanlder() : MotorHanlder(MOTOR_DEFAULT_PIN) {

    }
    
    //다른 기기에서 보내온 메세지를 수신
    void onMessage(const device_communication_message_t message){
        //보낸 메세지 타입이 모터 설정이면
        if(message.type == MESSAGE_TYPE::RUN_MOTOR){
            //설정됀 GPIO 상태 변화
            motor_message_t motor;
            motor.message = message.message;
            switch(motor.status){
                case MOTOR_STATUS::MOTOR_OFF : {
                    digitalWrite(this->pin, LOW);
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON :{
                    digitalWrite(this->pin, HIGH);
                    break;
                }
                default : break;
            }
        }
    }
};

#endif