#ifndef __LED_HPP__
#define __LED_HPP__

#include <cstdint>
#include "../struct/packet.hpp"

using namespace std;

class Led {
    Led(){

    }

    void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        
    }

    void set(uint8_t r, uint8_t g, uint8_t b){
        this->set(r, g, b, 255u);
        
    }

    void set(color_t color) {
        this->set(color.r, color.g, color.b, color.bright);
    }
};
#endif