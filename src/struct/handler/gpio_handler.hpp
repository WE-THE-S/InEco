#ifndef __MESSAGE_GPIO_CONTROL_HANDLER_HPP__
#define __MESSAGE_GPIO_CONTROL_HANDLER_HPP__

#include <Arduino.h>
#include "../packet.hpp"
#include "../broadcast/message_broadcast_receiver.hpp"

class MessageGpioControlHandler : public MessageBroadcastReceiver{
    protected:
        gpio_num_t pin;
    
    public:
        MessageGpioControlHandler(gpio_num_t _pin) : pin(_pin) {
            
        }
        
        void onMessage(const device_communication_message_t message){
            ESP_LOGE(typename(this), "No configure message receiver");    
        }
};

#endif