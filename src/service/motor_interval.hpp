#ifndef __MOTOR_INTERVAL_HPP__
#define __MOTOR_INTERVAL_HPP__

#include <Arduino.h>
#include <cmath>
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

        MOTOR_STATUS sendMessage(MOTOR_STATUS status){
            service_signal_t signal;
            communcation_service_signal_t com;
            motor_message_t* motorMessage = new motor_message_t;
            auto broadcast = Broadcast<service_signal_t>::getInstance();
            
            motorMessage->status = MOTOR_STATUS::MOTOR_ON;

            com.dir = MESSAGE_DIRECTION::TO_SLAVE;
            com.header |= static_cast<uint8_t>(MESSAGE_TYPE::RUN_MOTOR);
            com.message = &motorMessage->message;
            
            signal.value = com.value;
            signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;

            broadcast->broadcast(signal);

            delete motorMessage;
            return status;
        }
    public:

    MotorInterval() {
        intervalEnable = false;
        onOff = false;
        lastIntervalStatus = MOTOR_STATUS::MOTOR_OFF;
        lastTime = 0;
        intervalSpan = MOTOR_DEFAULT_SPAN;
        intervalTime = MOTOR_DEFAULT_TIME;
    }

    void execute(){
        if(this->intervalEnable){
            const uint64_t now = millis();
            const auto requireOffTime = this->lastTime + this->intervalSpan;
            const auto requireOnTime = this->lastTime + this->intervalTime;

            switch(this->lastIntervalStatus){
                case MOTOR_STATUS::MOTOR_OFF : {
                    if(requireOnTime <= now){
                        ESP_LOGI(typename(this), "motor interval on %u", now);
                        this->lastTime = now;
                        this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_ON);
                    }
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON : {
                    if(requireOffTime <= now){
                        ESP_LOGI(typename(this), "motor interval off %u", now);
                        this->lastTime = now;
                        this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_OFF);
                    }
                    break;
                }
            }

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
                    this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_OFF);
                    this->lastTime = min(millis() - signal.intervalTime, 0ul);
                }
                this->intervalEnable = signal.intervalEnable;
                ESP_LOGI(typename(this), "Enable %u", this->intervalEnable);
                ESP_LOGI(typename(this), "Span %u", this->intervalSpan);
                ESP_LOGI(typename(this), "Time %u", this->intervalTime);
            }else{
                this->onOff = signal.onOff;
                ESP_LOGI(typename(this), "Motor %s", this->onOff ? "ON" : "OFF");
                this->sendMessage(this->onOff ? MOTOR_STATUS::MOTOR_ON : MOTOR_STATUS::MOTOR_OFF);
            }
        }
    }
};


#endif