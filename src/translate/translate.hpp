#ifndef __COMMUNCATION_HPP__
#define __COMMUNCATION_HPP__

#include <Arduino.h>
#include <queue>
#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../utils/broadcast.hpp"

using namespace std;

class Translate{
    protected:
        HardwareSerial* master;
        HardwareSerial* right;
        HardwareSerial* bottom;
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