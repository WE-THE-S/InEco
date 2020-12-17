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


//analog waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class LedInterval : public Service {
private:
	uint32_t lastSetTime;
	//패턴과 사이즈 설정
	size_t patternSize;
	pair<uint8_t, uint8_t>* pattern;
	bool isSlow;
	color_t defaultColor;
	color_t blackColor;

	void send(const uint8_t x, const uint8_t y, const color_t color) const {
		service_signal_t signal;
		communcation_service_signal_t com;
		led_message_t *ledMessage = new led_message_t;
		memset(ledMessage->message.bytes, 0x00, sizeof(led_message_t));
		auto broadcast = Broadcast<service_signal_t>::getInstance();

		ledMessage->row = y;
		ledMessage->col = x;
		ledMessage->color = color;

		com.dir = MESSAGE_DIRECTION::TO_SLAVE;
		com.type = MESSAGE_TYPE::SET_COLOR;
		com.message = &ledMessage->message;

		signal.value = com.value;
		signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;

		broadcast->broadcast(signal);
	}

	void showNextPattern() const {
		led_message_t message;
		if(patternSize != 0){
			auto last = pattern[patternSize - 1];
			send(last.first, last.second, blackColor);
			pair<uint32_t, uint32_t> target = pattern[0];
			memmove(pattern, &pattern[1], sizeof(pair<uint8_t, uint8_t>*) * (patternSize - 1));
			pattern[patternSize - 1] = target;
			send(target.first, target.second, defaultColor);
		}
	}
public:
	//i2c 인스턴스 초기화
	LedInterval() {
		pattern = nullptr;
		defaultColor.bright = 255u;
		defaultColor.r = 255u;
		defaultColor.g = 255u;
		defaultColor.b = 255u;
		blackColor.bright = 0u;
		blackColor.r = 0u;
		blackColor.g = 0u;
		blackColor.b = 0u;
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
			led_message_t led;
		}
	}
};

#endif