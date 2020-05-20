#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__

#include <Arduino.h>
#include "./broadcast/service_signal_receiver.hpp"
#include "./packet.hpp"

class Service : public ServiceSignalBroadcastReceiver {
    public:
    virtual void execute(){
        ESP_LOGE(typename(this), "Service can't execute");
    }
};

#endif