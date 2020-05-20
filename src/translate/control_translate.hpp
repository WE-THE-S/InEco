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
                device_communication_message_t packet;
                
                signal.value = message.value;
                packet.message = *signal.message;
                uint8_t type = static_cast<uint8_t>(packet.type);
                uint8_t dir = static_cast<uint8_t>(packet.type);
                
                packet.type = static_cast<MESSAGE_TYPE>(type | dir);   
                packet.crc = packet.getCrc();
                
                this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
                this->right->write(packet.bytes, sizeof(device_communication_message_t));
            }
        }
};

#endif