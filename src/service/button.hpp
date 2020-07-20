#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <Arduino.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_adc_cal.h>
#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"
#include "../utils/broadcast.hpp"

using namespace std;

//초 단위임
const uint32_t intervalTimeSet[] = {600, 1800, 7200, 21600, 43200};

class Button : public Service {
    protected:
        uint8_t intervalTimePos;
        pair<gpio_num_t, int> interval;
        pair<gpio_num_t, int> runTime;
        motor_interval_service_signal_t* message;
        void send(const motor_interval_service_signal_t* message){
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
            signal.value = message->value;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }
    public:
    Button(gpio_num_t _run_pin, gpio_num_t _inter_pin) {
        runTime = make_pair(_run_pin, 0);
        intervalTimePos = 0u;
        interval = make_pair(_inter_pin, 0); 
        this->message = new motor_interval_service_signal_t;
        this->message->intervalEnable = true;
        this->message->isIntervalSet = true;
        this->message->intervalSpan = intervalTimeSet[0] * S_TO_MS_FACTOR;
        this->message->intervalTime = 1 * S_TO_MS_FACTOR;
        pinMode(_run_pin, INPUT_PULLUP);
        pinMode(_inter_pin, INPUT_PULLUP);
    }
    
    Button() : Button(RUNTIME_BUTTON_DEFAULT_PIN, INTERVAL_BUTTON_DEFAULT_PIN){
        
    }

    ~Button(){
        delete this->message;
    }

    void execute(){
        bool changed = false;
        auto intervalStatus = digitalRead(interval.first);
        auto runStatus = digitalRead(runTime.first);

        if(intervalStatus != interval.second){
            changed = true;
            auto nextTime = intervalTimeSet[(intervalTimePos + 1) % MAX_MOTOR_INTERVAL] * S_TO_MS_FACTOR;
            this->message->intervalTime = nextTime;
        }
        
        if(runStatus != runTime.second){
            changed = true;
            auto nextTime = (this->message->intervalSpan / S_TO_MS_FACTOR);
            nextTime = ((nextTime + 1) % MAX_MOTOR_SPAN) + 1;
            this->message->intervalSpan = (nextTime * S_TO_MS_FACTOR);
        }

        interval.second = intervalStatus;
        runTime.second = runStatus;
        if(changed){
            this->send(message);
        }
    }

    void onMessage(const service_signal_t message){
    
    }
};

#endif