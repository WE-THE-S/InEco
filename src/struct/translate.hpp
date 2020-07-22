#ifndef __TRANSLATE_HPP__
#define __TRANSLATE_HPP__

#include "../config.hpp"
#include "../utils/broadcast.hpp"
#include "./packet.hpp"
#include <Arduino.h>
#include "./helper.hpp"
class Translate {
protected:
	HardwareSerial *master;
	HardwareSerial *right;
	HardwareSerial *bottom;

    inline void rightSend(device_communication_message_t* packet){
        rightSend(*packet);
    }

    inline void rightSend(device_communication_message_t packet){
		#if CONTROL_BOARD == 1
			if(packet.type == MESSAGE_TYPE::SET_COLOR){
				for(auto i = 0;i<PACKET_RETRY_COUNT;i++){
		#endif
        this->right->write(packet.bytes, sizeof(device_communication_message_t));
		#if CONTROL_BOARD == 1	
				}
			}else{
				this->right->write(packet.bytes, sizeof(device_communication_message_t));
			}
		#endif
    }

    inline void bottomSend(device_communication_message_t* packet){
        bottomSend(*packet);
    }

    inline void bottomSend(device_communication_message_t packet){
		#if CONTROL_BOARD == 1
			if(packet.type == MESSAGE_TYPE::SET_COLOR){
				for(auto i = 0;i<PACKET_RETRY_COUNT;i++){
		#endif
        this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
		#if CONTROL_BOARD == 1	
				}
			}else{
				this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
			}
		#endif
    }
    
    inline void send(device_communication_message_t* packet){
        send(*packet);
	}
	
    void send(device_communication_message_t packet){
		ESP_LOGD(typename(this), "All Send");
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