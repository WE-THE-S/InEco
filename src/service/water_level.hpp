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
        adc_power_on();
    }
    WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN){

    }

    void execute(){
        int readRaw;
        ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_8, ADC_ATTEN_11db));
        ESP_ERROR_CHECK(adc2_get_raw(ADC2_CHANNEL_8, ADC_WIDTH_12Bit, &readRaw));
        adcAttachPin(pin);
        ESP_LOGI(typename(this), "adc raw : %d", readRaw);
        if(readRaw > WATER_LOW_THRESHOLD && !lastStatus){
            lastStatus = true;
            service_signal_t signal;
            signal.type = SERVICE_SIGNAL_TYPE::ALARM;
            signal.value = 1ull;
            Broadcast<service_signal_t>::getInstance()->broadcast(signal);
        }else if(readRaw <= WATER_LOW_THRESHOLD && lastStatus){
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