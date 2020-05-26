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
        char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};

        std::string bytesToHex(uint8_t* bytes, size_t size) const {
            std::string str;
            for (size_t i = 0; i < size; ++i) {
                const uint8_t ch = bytes[i];
                str.append(&hex[(ch  & 0xF0) >> 4], 1);
                str.append(&hex[ch & 0xF], 1);
            }
            return str;
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
                packet.type = signal.type;
                packet.header |= static_cast<uint8_t>(signal.dir);
                packet.crc = packet.getCrc();
                ESP_LOGI(typename(this), "Hex : %s", bytesToHex(packet.bytes, sizeof(device_communication_message_t)).c_str());
                this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
                this->right->write(packet.bytes, sizeof(device_communication_message_t));
            }
        }
};

#endif