#ifndef __LED_HPP__
#define __LED_HPP__

#include <Arduino.h>
#include <cstdint>
#include "../struct/packet.hpp"
#include "../config.hpp"

using namespace std;

class Motor {
    private:
        gpio_num_t pin;

    public:
    Motor(gpio_num_t _pin) : pin(_pin) {
        pinMode(pin, OUTPUT);
    }
    Motor() : Motor(MOTOR_DEFAULT_PIN) {
    }

    void set(MotorStatus motor) {
        if(MotorStatus::MOTOR_OFF != motor){
            digitalWrite(pin, HIGH);
        }else{
            digitalWrite(pin, LOW);
        }
    }


};
#endif