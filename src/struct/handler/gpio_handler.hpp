#ifndef __MESSAGE_GPIO_CONTROL_HANDLER_HPP__
#define __MESSAGE_GPIO_CONTROL_HANDLER_HPP__

#include <Arduino.h>
#include "../packet.hpp"
#include "../broadcast/message_broadcast_receiver.hpp"

//다른 기기에서 메세지가 왔을때 핀 설정을 하는 기본 클래스
class MessageGpioControlHandler : public MessageBroadcastReceiver{
    protected:
        gpio_num_t pin;
    
    public:
        //pin 입출력 방향 설정
        MessageGpioControlHandler(const gpio_num_t _pin) : pin(_pin) {
            pinMode(_pin, OUTPUT);
            //기본은 LOW상태
            digitalWrite(_pin, LOW);
        }
        
        //만약 핸들러를 별도로 정의하지 않은 경우, LOG 출력
        void onMessage(const device_communication_message_t message){
            ESP_LOGE(typename(this), "No configure message receiver");    
        }
};

#endif