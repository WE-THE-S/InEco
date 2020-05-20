#ifndef __GPIO_HANDLER_HPP__
#define __GPIO_HANDLER_HPP__

#include <Arduino.h>
#include <queue>
#include <cstdint>
#include "../packet.hpp"
#include "../message_broadcast_receiver.hpp"

using namespace std;

class GpioHandler : public MessageBroadcastReceiver{
    protected:
        gpio_num_t pin;
    
    public:
        GpioHandler(gpio_num_t _pin) : pin(_pin) {
            
        }
        
        void messageRecv(const device_communication_message_t message){
            ESP_LOGE(typename(this), "No configure message receiver");    
        }
};

#endif