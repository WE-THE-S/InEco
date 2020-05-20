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

        }
};

#endif