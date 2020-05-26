#ifndef __TRANSLATE_HPP__
#define __TRANSLATE_HPP__

#include <Arduino.h>
#include "../config.hpp"
#include "../utils/broadcast.hpp"
#include "./packet.hpp"

class Translate{
    protected:
        HardwareSerial* master;
        HardwareSerial* right;
        HardwareSerial* bottom;

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
    Translate(HardwareSerial* _master, HardwareSerial* _right, HardwareSerial* _bottom) 
        : master(_master), right(_right), bottom(_bottom) {
        this->master->begin(UART_BAUD_RATE);
        this->bottom->begin(UART_BAUD_RATE, UART_SERIAL_MODE, BOTTOM_UART_RX, BOTTOM_UART_TX);
        this->right->begin(UART_BAUD_RATE, UART_SERIAL_MODE, RIGHT_UART_RX, RIGHT_UART_TX);
    }

    Translate() : Translate(&Serial, &RightSerial, &BottomSerial) {
    }

    ~Translate(){
        master->flush();
        right->flush();
        bottom->flush();
    }

    virtual void recv(){
        ESP_LOGE(typename(this), "No configure recv");
    }

    virtual void send(){
        ESP_LOGE(typename(this), "No configure send");
    }
};
#endif