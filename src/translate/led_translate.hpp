#ifndef __SLAVE_HPP__
#define __SLAVE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "./translate.hpp"

class LedTranslate : protected Translate {
    public:
    LedTranslate(HardwareSerial* _master, HardwareSerial* _right, HardwareSerial* _bottom) : 
        Translate(_master, _right, _bottom) {
    }

    LedTranslate() : Translate() {

    }

    void recv(){
        if(this->master->available() >= sizeof(device_communication_message_t)){
        auto message = new device_communication_message_t;
        this->master->readBytes(message->bytes, sizeof(device_communication_message_t)); 
        switch(message->type){
            case MessageType::RUN_MOTOR : {
                this->bottom->write(message->bytes, sizeof(device_communication_message_t));
                this->right->write(message->bytes, sizeof(device_communication_message_t));
                break;
            }
            case MessageType::SET_COLOR : {
            auto led = reinterpret_cast<led_message_t*>(&(message->message));
            if(led->row == 0 & led->col == 0){
                
            }else{
                led->col -= 1;
                this->right->write(message->bytes, sizeof(device_communication_message_t));
                led->col += 1;
                led->row -= 1;
                this->bottom->write(message->bytes, sizeof(device_communication_message_t));
            }
            break;
            }
            default : break;
        }
        delete message;
        }
    }

    void send(){
        
    }
};

#endif