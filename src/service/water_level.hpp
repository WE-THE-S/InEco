#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

#include <Arduino.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_adc_cal.h>
#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"
#include "../utils/broadcast.hpp"

class WaterLevel : public Service {
    private:
        bool lastStatus;
        gpio_num_t pin;
    public:
    WaterLevel(gpio_num_t _pin) : pin(_pin) {
        pinMode(pin, INPUT_PULLUP);
    }
    WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN){

    }

    void execute(){
        int readRaw = digitalRead(this->pin);
        if(readRaw == WATER_LOW_THRESHOLD && !lastStatus){
            ESP_LOGI(typename(this), "raw : turn on");
            lastStatus = true;
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::ALARM;
            signal.value = 1ull;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }else if(readRaw != WATER_LOW_THRESHOLD && lastStatus){
            ESP_LOGI(typename(this), "raw : turn off");
            lastStatus = false;
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::ALARM;
            signal.value = 0ull;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }
    }

    void onMessage(const service_signal_t message){
    
    }
};

#endif