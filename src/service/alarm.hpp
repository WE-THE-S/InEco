#ifndef __ALARM_HPP__
#define __ALARM_HPP__

#include <Arduino.h>

#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"

class Alarm : public Service {
    private:
        gpio_num_t pin;
    public:
    Alarm(gpio_num_t _pin) : pin(_pin) {
        pinMode(pin, OUTPUT);
    }
    Alarm() : Alarm(ALARM_LED_DEFAULT_PIN){}

    void execute(){
        
    }

    void onMessage(const service_signal_t message){
        if(message.type == SERVICE_SIGNAL_TYPE::ALARM){
            ESP_LOGI(typename(this), "alarm status change : %d", message.value);
            if(message.value){
                digitalWrite(pin, HIGH);
            }else{
                digitalWrite(pin, LOW);
            }
        }
    }
};

#endif