#ifndef __COMMUNCATION_HPP__
#define __COMMUNCATION_HPP__

#include <Arduino.h>
#include <queue>
#include "../config.hpp"
#include "../struct/packet.hpp"
using namespace std;
class Translate{
    protected:
        HardwareSerial* master;
        HardwareSerial* right;
        HardwareSerial* bottom;
        queue<device_communication_message_t>* recvQueue;
        queue<pair<PacketDirection, device_communication_message_t>>* sendQueue;

    public:
    Translate(HardwareSerial* _master, HardwareSerial* _right, HardwareSerial* _bottom) 
        : master(_master), right(_right), bottom(_bottom) {
        this->master->begin(UART_BAUD_RATE);
        this->bottom->begin(UART_BAUD_RATE, UART_SERIAL_MODE, BOTTOM_UART_RX, BOTTOM_UART_TX);
        this->right->begin(UART_BAUD_RATE, UART_SERIAL_MODE, RIGHT_UART_RX, RIGHT_UART_TX);
        this->sendQueue = new queue<pair<PacketDirection, device_communication_message_t>>;
        this->recvQueue = new queue<device_communication_message_t>;
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