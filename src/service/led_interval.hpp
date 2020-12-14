#ifndef __LED_INTERVAL_HPP__
#define __LED_INTERVAL_HPP__

#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/service.hpp"
#include "../utils/broadcast.hpp"

#include <Arduino.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <deque>
#include <cmath>
#include <soc/rtc_cntl_reg.h>
#include <vector>


using namespace std;

//analog waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class LedInterval : public Service {
private:
	uint32_t lastSetTime;
	//패턴과 사이즈 설정
	pair<uint32_t, uint32_t> size {0, 0};
	deque<pair<uint32_t, uint32_t>> pattern;
	bool isSlow;
	color_t color;

	//모터 세기를 설정
	void send(led_message_t message) const {
		//주변 장치들에 뿌리기 위해서 communication_service에 명령어 패킷을 전달
		auto broadcast = Broadcast<service_signal_t>::getInstance();
		service_signal_t signal;
		communcation_service_signal_t com;
	
		com.dir = MESSAGE_DIRECTION::TO_SLAVE;
		com.type = MESSAGE_TYPE::RUN_MOTOR;
		com.message = &(message.message);
		
		signal.value = com.value;
		signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;

		broadcast->broadcast(signal);
	}

	void showNextPattern() const {
		led_message_t message;
		if(!pattern.empty()){
			auto last = pattern.back();
			message.col = last.first;
			message.row = last.second;
			message.color.r = 0;
			message.color.g = 0;
			message.color.b = 0;
			message.color.bright = 0;
			send(message);
			pair<uint32_t, uint32_t> target = pattern.front();

			message.col = target.first;
			message.row = target.first;
			message.color.r = 0;
			message.color.g = 0;
			message.color.b = 0;
			message.color.bright = 0;
			pattern.pop_front();
			pattern.push_back(target);
			send(message);
		}
	}
public:
	//i2c 인스턴스 초기화
	LedInterval() {

	}

	void execute() {
		const auto now = millis();
		const auto diff = now - lastSetTime;
		if(isSlow){
			if(diff > SLOW_PATTERN_TIME){
				lastSetTime = now;
				showNextPattern();
			}
		}else{
			if(diff > FAST_PATTERN_TIME){
				lastSetTime = now;
				showNextPattern();
			}
		}
	}

	void onMessage(const service_signal_t message) {
		if(message.type == SERVICE_SIGNAL_TYPE::SET_LED_INTERVAL){

		}
	}
};

#endif