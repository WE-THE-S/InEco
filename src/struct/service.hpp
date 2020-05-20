#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

#include <Arduino.h>

class Service {
    virtual void execute(){
        ESP_LOGE(typename(this), "Service can't execute");
    }
};

#endif