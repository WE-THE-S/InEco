#ifndef __TRANSLATE_HPP__
#define __TRANSLATE_HPP__

#include "../config.hpp"
#include "../utils/broadcast.hpp"
#include "./packet.hpp"
#include <Arduino.h>
#include <SoftwareSerial.h>

class Translate {
protected:
	HardwareSerial *master;
	HardwareSerial *right;
	HardwareSerial *bottom;

	char const hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	std::string bytesToHex(uint8_t *bytes, size_t size) const {
		std::string str;
		for (size_t i = 0; i < size; ++i) {
			const uint8_t ch = bytes[i];
			str.append(&hex[(ch & 0xF0) >> 4], 1);
			str.append(&hex[ch & 0xF], 1);
		}
		return str;
	}

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