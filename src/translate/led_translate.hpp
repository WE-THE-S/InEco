#ifndef __LED_TRANSLATE_HPP__
#define __LED_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/helper.hpp"
class LedTranslate : public Translate {
    protected:
    void broadcast(const device_communication_message_t message){
        Broadcast<device_communication_message_t>::getInstance()->broadcast(message);
    }

    public:
    LedTranslate() : Translate() {
    }

    void recv(){
        if(this->master->available() >= sizeof(device_communication_message_t)){
            auto message = new device_communication_message_t;
            this->master->readBytes(message->bytes, sizeof(device_communication_message_t)); 
            this->broadcast(*message);
            Helper helper;
            ESP_LOGI(typename(this), "Packet : %s", helper.bytesToHex(message->bytes, sizeof(device_communication_message_t)).c_str());
            switch(message->type){
                case MESSAGE_TYPE::RUN_MOTOR : 
                case MESSAGE_TYPE::MODULE_RESTART : {
                    send(message);
                    if(message->type == MESSAGE_TYPE::MODULE_RESTART){
                        delay(10);
                        ESP.restart();
                    }
                    break;
                }
                case MESSAGE_TYPE::SET_COLOR : {
                    auto led = reinterpret_cast<led_message_t*>(&(message->message));
                    if(led->col > 0){
                        led->col -= 1;
                        rightSend(message);
                        led->col += 1;
                    }
                    if(led->row > 0){
                        led->row -= 1;
                        bottomSend(message);
                    }
                    break;
                }
                default : break;
            }
            delete message;
        }
    }
};

#endif