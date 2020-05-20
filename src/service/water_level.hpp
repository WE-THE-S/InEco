#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

#include <Arduino.h>
#include <soc/rtc_cntl_reg.h>
#include <esp_adc_cal.h>
#include "../config.hpp"
#include "../struct/service.hpp"
#include "../struct/packet.hpp"

class WaterLevel : public Service {
    private:
        gpio_num_t pin;
    public:
    WaterLevel(gpio_num_t _pin) : pin(_pin) {
        adc_power_on();
    }
    WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN){}
    void execute(){
        int read_raw;
        ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_8, ADC_ATTEN_11db));
        ESP_ERROR_CHECK(adc2_get_raw(ADC2_CHANNEL_8, ADC_WIDTH_12Bit, &read_raw));
        adcAttachPin(pin);
        double bat = static_cast<double>(read_raw);
        if(read_raw > 400){
            
        }
    }

    void onMessage(const service_signal_t message){
    
    }
};

#endif