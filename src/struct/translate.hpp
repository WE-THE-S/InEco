#ifndef __TRANSLATE_HPP__
#define __TRANSLATE_HPP__

#include "../config.hpp"
#include "../utils/broadcast.hpp"
#include "./packet.hpp"
#include <Arduino.h>
#include "./helper.hpp"
class Translate : protected Helper {
protected:
	HardwareSerial *master;
	HardwareSerial *right;
	HardwareSerial *bottom;

    inline void rightSend(device_communication_message_t* packet){
        rightSend(*packet);
    }

    inline void rightSend(device_communication_message_t packet){
        this->right->write(packet.bytes, sizeof(device_communication_message_t));
    }

    inline void bottomSend(device_communication_message_t* packet){
        bottomSend(*packet);
    }

    inline void bottomSend(device_communication_message_t packet){
        this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
    }
    
    inline void send(device_communication_message_t* packet){
        send(*packet);
	}
	
    void send(device_communication_message_t packet){
        rightSend(packet);
        bottomSend(packet);
    }
	
public:
	Translate() {
		this->master = &Serial;
		this->bottom = &Serial1;
		this->right = &Serial2;
	}

	~Translate() {
		master->flush();
		right->flush();
		bottom->flush();
	}

	void begin(){
		this->right->begin(HARDWARE_UART_BAUDRATE, HARDWARE_UART_SERIAL_MODE, RIGHT_UART_RX, RIGHT_UART_TX);
		this->bottom->begin(HARDWARE_UART_BAUDRATE, HARDWARE_UART_SERIAL_MODE, BOTTOM_UART_RX, BOTTOM_UART_TX);
		this->master->begin(HARDWARE_UART_BAUDRATE);
	}
	
	virtual void recv() {
		ESP_LOGE(typename(this), "No configure recv");
	}
};
#endif