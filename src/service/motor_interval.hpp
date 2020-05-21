#ifndef __MOTOR_INTERVAL_HPP__
#define __MOTOR_INTERVAL_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/service.hpp"
#include "../utils/broadcast.hpp"

class MotorInterval : public Service {
    private:
        //기간
        uint16_t intervalTime;

        //켜지는 시간
        uint16_t intervalSpan;
        
        //마지막으로 켜진 시간
        uint64_t lastTime;

        //마지막으로 타이머가 설정한 값
        MOTOR_STATUS lastIntervalStatus;

        //반복 실행 설정
        bool intervalEnable; 

        //onOff
        bool onOff;
    public:

    MotorInterval() {

    }

    void execute(){
        if(this->intervalEnable){
            const uint64_t const now = millis();
            const auto requireOffTime = this->lastTime + this->intervalSpan;
            const auto requireOnTime = this->lastTime + this->intervalTime;
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;
            motor_message_t* motorMessage = new motor_message_t;
            switch(this->lastIntervalStatus){
                case MOTOR_STATUS::MOTOR_OFF : {
                    if(requireOnTime > now){
                        motorMessage->status = MOTOR_STATUS::MOTOR_ON;
                        communcation_service_signal_t communicationSignal;
                        Broadcast<service_signal_t>::getInstance()->broadcast(signal);
                    }
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON : {
                    
                    break;
                }
            }
            delete motorMessage;
        }
    }

    void onMessage(const service_signal_t message){
        if(message.type == SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET){
            motor_interval_service_signal_t signal;
            signal.value = message.value;
            if(signal.isIntervalSet){
                this->intervalSpan = signal.intervalSpan;
                this->intervalTime = signal.intervalTime;
                if(signal.intervalEnable != this->intervalEnable){
                    this->lastIntervalStatus = MOTOR_STATUS::MOTOR_OFF;
                    this->lastTime = (millis() - signal.intervalTime);
                }
                this->intervalEnable = signal.intervalEnable;
            }else{
                this->onOff = signal.onOff;
            }
        }
    }
};


#endif