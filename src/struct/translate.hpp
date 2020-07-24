#ifndef __TRANSLATE_HPP__
#define __TRANSLATE_HPP__

#include "../config.hpp"
#include "../utils/broadcast.hpp"
#include "./packet.hpp"
#include <Arduino.h>
#include "./helper.hpp"

//기기간 통신을 위한 함수를 모아둔 클래스
class Translate {
protected:
	HardwareSerial *master;
	HardwareSerial *right;
	HardwareSerial *bottom;

    inline void rightSend(device_communication_message_t* packet){
        rightSend(*packet);
    }

	//오른쪽에 있는 기기
    inline void rightSend(device_communication_message_t packet){
		#if CONTROL_BOARD == 1
		//LED신호일 경우 자주 드랍나서 5번 정도 재송신함
			if(packet.type == MESSAGE_TYPE::SET_COLOR){
				for(auto i = 0;i<PACKET_RETRY_COUNT;i++){
			ESP_LOGI(typename(this), "Right LED Send");
		#else
			ESP_LOGI(typename(this), "Right Send");
		#endif
        this->right->write(packet.bytes, sizeof(device_communication_message_t));
		#if CONTROL_BOARD == 1	
				}
			}else{
				ESP_LOGI(typename(this), "Right Send");
				this->right->write(packet.bytes, sizeof(device_communication_message_t));
			}
		#endif
    }

    inline void bottomSend(device_communication_message_t* packet){
        bottomSend(*packet);
    }
	//아래 방향으로 연결됀 기기들에 메세지를 보낼때
    inline void bottomSend(device_communication_message_t packet){
		//LED신호일 경우 자주 드랍나서 5번 정도 재송신함
		#if CONTROL_BOARD == 1
			if(packet.type == MESSAGE_TYPE::SET_COLOR){
				for(auto i = 0;i<PACKET_RETRY_COUNT;i++){
			ESP_LOGI(typename(this), "Bottom LED Send");
		#else
			ESP_LOGI(typename(this), "Bottom Send");
		#endif
        this->bottom->write(packet.bytes, sizeof(device_communication_message_t));
		#if CONTROL_BOARD == 1	
				}
			}else{
				ESP_LOGI(typename(this), "Bottom Send");
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
		//종료시 내부 버퍼들 정리
		master->flush();
		right->flush();
		bottom->flush();
	}

	void begin(){
		//설정됀 baudrate로 Serial 모듈들 초기화
		this->right->begin(HARDWARE_UART_BAUDRATE, HARDWARE_UART_SERIAL_MODE, RIGHT_UART_RX, RIGHT_UART_TX);
		this->bottom->begin(HARDWARE_UART_BAUDRATE, HARDWARE_UART_SERIAL_MODE, BOTTOM_UART_RX, BOTTOM_UART_TX);
		this->master->begin(HARDWARE_UART_BAUDRATE);
		ESP_LOGD(typename(this), "Serial Begin");
		ESP_LOGI(typename(this), "Right baudrate : %u", this->right->baudRate());
		ESP_LOGI(typename(this), "Bottom baudrate : %u", this->bottom->baudRate());
	}
	
	//수신시 아무런 핸들러가 없으면 에러 로그 출력
	virtual void recv() {
		ESP_LOGE(typename(this), "No configure recv");
	}
};
#endif