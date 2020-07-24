#ifndef __ALARM_HPP__
#define __ALARM_HPP__

#include <Arduino.h>

#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"

class Alarm : public Service {
    private:
        gpio_num_t pin;
        bool last;
    public:
    //물 부족 알림을 위한 gpio 설정
    Alarm(gpio_num_t _pin) : pin(_pin) {
        pinMode(pin, OUTPUT);
        //마지막 경고 상태는 false 상태
        last = false;
    }

    //생성자에 인자를 넣지 않으면 기본 핀으로 시작
    Alarm() : Alarm(ALARM_LED_DEFAULT_PIN){}

    void execute(){
        
    }

    //다른 서비스에서 메세지가 날아올 경우 실행
    void onMessage(const service_signal_t message){
        //메세지 타입이 알람 신호 설정이면 아래 코드 실행
        if(message.type == SERVICE_SIGNAL_TYPE::ALARM){
            water_level_service_signal_t signal;
            signal.value = message.value;
            //보내진 메세지의 onOff 데이터가 기존 상태와 다르면 변경됀 onOff 데이터로 설정
            if(signal.onOff != last){
                ESP_LOGI(typename(this), "alarm status change : %d", message.value);
                if(signal.onOff){
                    digitalWrite(pin, HIGH);
                }else{
                    digitalWrite(pin, LOW);
                }
                last = signal.onOff;
            }
        }
    }
};

#endif