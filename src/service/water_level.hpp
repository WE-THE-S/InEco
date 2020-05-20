#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

#include <Arduino.h>
#include "../config.hpp"
#include "../struct/service.hpp"

class WaterLevel : public Service {
    private:
        gpio_num_t pin;
    public:
    WaterLevel(gpio_num_t _pin) : pin(_pin) {
        
    }
    WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN){}
    void execute(){
        
    }
};

#endif