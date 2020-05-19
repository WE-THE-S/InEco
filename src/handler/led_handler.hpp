#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "./handler.hpp"

using namespace std;

class LedHanlder : protected Handler {
    private:
        gpio_num_t motor;
        gpio_num_t led;

    public:
    LedHanlder(gpio_num_t _motor, gpio_num_t _led) : motor(_motor), led(_led), Handler() {

    }
    LedHanlder() : LedHanlder(MOTOR_DEFAULT_PIN, LED_CONTROL_DEFAULT_PIN) {

    }

    void execute(){
        while(this->commandQueue.size() > 0){
            auto command = this->commandQueue.front();
            switch(command.first){
                case MESSAGE_TYPE::RUN_MOTOR : {
                    motor_message_t motor = dynamic_cast<motor_message_t>(command.second);
                    break;
                }
                case MESSAGE_TYPE::SET_COLOR : {
                    led_message_t led = reinterpret_cast<led_message_t>(command.second);
                    
                    break;
                }
            }
            this->commandQueue.pop();
        }
    }
};

#endif