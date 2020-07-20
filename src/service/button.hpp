#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

#include <Arduino.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_adc_cal.h>
#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"
#include "../utils/broadcast.hpp"

using namespace std;

class Button : public Service {
    protected:
        pair<gpio_num_t, int> interval;
        pair<gpio_num_t, int> runTime;
        motor_interval_service_signal_t* message;
        void sendAlarm(const motor_interval_service_signal_t message){
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
            signal.value = message.value;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }
    public:
    Button(gpio_num_t _run_pin, gpio_num_t _inter_pin) : 
        interval(make_pair(_inter_pin, 0)), runTime(make_pair(_run_pin, 0)) {
        this->message = new motor_interval_service_signal_t;
        pinMode(_run_pin, INPUT_PULLUP);
        pinMode(_inter_pin, INPUT_PULLUP);
    }
    
    Button() : Button(RUNTIME_BUTTON_DEFAULT_PIN, INTERVAL_BUTTON_DEFAULT_PIN){
        
    }

    ~Button(){
        delete this->message;
    }

    void execute(){

    }

    void onMessage(const service_signal_t message){
    
    }
};

#endif