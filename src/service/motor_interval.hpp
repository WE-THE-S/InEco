#ifndef __MOTOR_INTERVAL_HPP__
#define __MOTOR_INTERVAL_HPP__

#include "../struct/service.hpp"
#include <Arduino.h>
class MotorInterval : public Service {
    private:
        //기간
        uint16_t intervalTime;

        //켜지는 시간
        uint16_t intervalSpan;
        
        //마지막으로 켜진 시간
        uint64_t lastTime;

        //마지막으로 타이머가 설정한 값
        bool lastIntervalStatus;

        //반복 실행 설정
        bool intervalEnable; 

        //onOff
        bool onOff;
    public:
    MotorInterval() {

    }
    void execute(){
        
    }

    void onMessage(const service_signal_t message){
        if(message.type == SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET){
        }
    }
};


#endif