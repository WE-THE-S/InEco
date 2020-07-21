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
//const uint32_t intervalTimeSet[] = {600, 1800, 7200, 21600, 43200};
const uint32_t intervalTimeSet[] = {10, 30, 60, 120, 240};
class Button : public Service {
    protected:
        uint8_t intervalTimePos;
        pair<gpio_num_t, int> interval;
        pair<gpio_num_t, int> runTime;
        motor_interval_service_signal_t* message;
        void send(const motor_interval_service_signal_t* message){
			ESP_LOGI(typename(this), "interval time %u", message->intervalTime);
            ESP_LOGI(typename(this), "interval span %u", message->intervalSpan);
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET;
            signal.value = message->value;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }
    public:
    Button(const gpio_num_t _run_pin, const gpio_num_t _inter_pin) {
        runTime = make_pair(_run_pin, 0);
        intervalTimePos = 0u;
        interval = make_pair(_inter_pin, 0); 
        this->message = new motor_interval_service_signal_t;
        this->message->intervalEnable = true;
        this->message->isIntervalSet = true;
        this->message->intervalSpan = S_TO_MS_FACTOR;
        this->message->intervalTime = intervalTimeSet[intervalTimePos] * S_TO_MS_FACTOR;
    }

    Button() : Button(RUNTIME_BUTTON_DEFAULT_PIN, INTERVAL_BUTTON_DEFAULT_PIN){
        
    }

    ~Button(){
        delete this->message;
    }

    void execute(){
        pinMode(interval.first, INPUT_PULLUP);
        pinMode(runTime.first, INPUT_PULLUP);
        bool changed = false;
        auto intervalStatus = digitalRead(interval.first);
        auto runStatus = digitalRead(runTime.first);
        if(intervalStatus != interval.second){
            ESP_LOGI(typename(this), "Interval Status : %d, %d", interval.first, intervalStatus);
            if(!intervalStatus){
                changed = true;
                intervalTimePos = (intervalTimePos + 1) % MAX_MOTOR_INTERVAL;
                auto nextTime = intervalTimeSet[intervalTimePos] * S_TO_MS_FACTOR;
                this->message->intervalTime = nextTime;
            }
        }
        
        if(runStatus != runTime.second){
            if(!runStatus){
                ESP_LOGI(typename(this), "Run Status : %d", runStatus);
                changed = true;
                auto nextTime = (this->message->intervalSpan / S_TO_MS_FACTOR);
                nextTime = (nextTime % MAX_MOTOR_SPAN) + 1;
                this->message->intervalSpan = (nextTime * S_TO_MS_FACTOR);
            }
        }
    
        interval.second = intervalStatus;
        runTime.second = runStatus;
        if(changed){
            this->send(message);
            delay(100);
        }
    }

    void onMessage(const service_signal_t message){
    
    }
};

#endif