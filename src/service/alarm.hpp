#ifndef __ALARM_HPP__
#define __ALARM_HPP__

#include <Arduino.h>

#include "../config.hpp"
#include "../struct/service.hpp"

class Alarm : public Service {
    private:
        gpio_num_t pin;
    public:
    Alarm(gpio_num_t _pin) : pin(_pin) {
        
    }
    Alarm() : Alarm(ALARM_LED_DEFAULT_PIN){}
    void execute(){
        
    }
};

#endif