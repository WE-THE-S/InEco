#ifndef __CONTROL_HPP__
#define __CONTROL_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "./translate.hpp"

class ControlTranslate : protected Translate {
    public:
    ControlTranslate(HardwareSerial* _master, HardwareSerial* _right, HardwareSerial* _bottom) : 
        Translate(_master, _right, _bottom) {
    }
    ControlTranslate() : Translate() {
    }

    void recv(){
        
    }

    void send(){

    }
};

#endif