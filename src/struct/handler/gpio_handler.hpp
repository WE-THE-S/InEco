#ifndef __HANDLER_HPP__
#define __HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../packet.hpp"
#include "../broadcast_receiver.hpp"

using namespace std;

class GpioHandler : public BroadcastReceiver{
    protected:
        gpio_num_t pin;
    
    public:
        GpioHandler(gpio_num_t _pin) : pin(_pin) {
            
        }
        
        void messageRecv(const device_communication_message_t const message){
            ESP_LOGE(typename(this), "No configure message receiver");    
        }
};

#endif