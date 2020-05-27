#ifndef __CONTROL_TRANSLATE_HPP__
#define __CONTROL_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/broadcast/service_signal_receiver.hpp"

class ControlTranslate : protected Translate, public ServiceSignalBroadcastReceiver {
    public:
        void recv(){
            if(this->bottom->available()){
                auto str = this->bottom->readString();
                ESP_LOGI(typename(this), "Bottom : %s", str.c_str());
            }
            if(this->right->available()){
                auto str = this->right->readString();
                ESP_LOGI(typename(this), "Right : %s", str.c_str());
            }
        }

        ControlTranslate() : Translate() {

        }

        void onMessage(const service_signal_t message){
            if(message.type == SERVICE_SIGNAL_TYPE::PACKET_SEND){
                communcation_service_signal_t signal;
                device_communication_message_t packet;
                
                signal.value = message.value;
                packet.message = *signal.message;
                packet.type = signal.type;
                packet.dir = signal.dir;
                ESP_LOGI(typename(this), "Hex : %s", bytesToHex(packet.bytes, sizeof(device_communication_message_t)).c_str());
                send(packet);
            }
        }
};

#endif