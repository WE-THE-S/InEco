#ifndef __CONTROL_TRANSLATE_HPP__
#define __CONTROL_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/broadcast/service_signal_receiver.hpp"

class ControlTranslate : protected Translate, public ServiceSignalBroadcastReceiver {
    protected:
        void recv(){
            
        }

        void send(){

        }
    public:
        ControlTranslate(HardwareSerial* _master, HardwareSerial* _right, HardwareSerial* _bottom) : 
            Translate(_master, _right, _bottom) {
        }
        ControlTranslate() : Translate() {
        }

        void onMessage(const service_signal_t message){
            if(message.type == SERVICE_SIGNAL_TYPE::PACKET_SEND){
                communcation_service_signal_t signal;
                device_communication_message_t message;
                message.dir = MESSAGE_DIRECTION::TO_SLAVE;
                message.type = MESSAGE_TYPE::RUN_MOTOR;

                
                this->bottom->write(message->bytes, sizeof(device_communication_message_t));
                this->right->write(message->bytes, sizeof(device_communication_message_t));
                if(message.value){
                    digitalWrite(pin, HIGH);
                }else{
                    digitalWrite(pin, LOW);
                }
            }
        }
};

#endif