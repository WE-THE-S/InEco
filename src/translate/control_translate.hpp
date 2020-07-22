#ifndef __CONTROL_TRANSLATE_HPP__
#define __CONTROL_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/broadcast/service_signal_receiver.hpp"
#include "../struct/helper.hpp"

class ControlTranslate : public Translate, public ServiceSignalBroadcastReceiver {
    public:
        void recv(){
            if(this->bottom->available() != 0){
                auto len = this->bottom->available();
                auto str = static_cast<char*>(calloc(sizeof(char), len + 1));
                this->bottom->readBytes((uint8_t*)str, len);
                ESP_LOGI(typename(this), "Bottom : %d", len);
                ESP_LOGI(typename(this), "Bottom : %s", str);
                this->bottom->flush();
                free(str);
            }
            if(this->right->available() != 0){
                auto len = this->right->available();
                auto str = static_cast<char*>(calloc(sizeof(char), len + 1));
                this->right->readBytes((uint8_t*)str, len);
                ESP_LOGI(typename(this), "Right : %d", len);
                ESP_LOGI(typename(this), "Right : %s", str);
                this->right->flush();
                free(str);
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
                Helper helper;
                ESP_LOGI(typename(this), "Hex : %s", helper.bytesToHex(packet.bytes, sizeof(device_communication_message_t)).c_str());
                if(signal.type == MESSAGE_TYPE::SET_COLOR){
                    led_message_t led;
                    led.message = *signal.message;
                    ESP_LOGI(typename(this), "row : %u", led.row);
                    if(led.row > 1){
                        led.row -= 2;
                        packet.message = led.message;
                        bottomSend(packet);
                    }else{
                        rightSend(packet);                        
                    }
                }else{
                    send(packet);
                }
            }
        }
        /*
            col_ _
            |
        row |
        */
};

#endif