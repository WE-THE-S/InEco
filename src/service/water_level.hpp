#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

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

//analog waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class WaterLevel : public Service {
private:
	//마지막으로 보낸 현재 유량 상태
	water_level_service_signal_t last;

	//처음 보내는 패킷인지
	bool isFirstSend;

	//원래 analog타입 water level 센서를 사용했을때 사용하던 레거시 변수
	gpio_num_t pin;
	std::deque<uint16_t> buffer;

	//알람 신호를 주변 서비스에 뿌리는 함수
	inline void sendAlarm(const water_level_service_signal_t value) {
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::ALARM;
		signal.value = value.value;
        ESP_LOGI(typename(this), "level %u", value.level);
        ESP_LOGI(typename(this), "alarm turn %s", value.onOff ? "On" : "Off");
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

	//water level 모듈에서 레지스터를 읽어오는 함수
	uint16_t getRawValue(void) {
		adc_power_on();
		ESP_ERROR_CHECK(adc1_config_channel_atten(WATER_LEVEL_SENSOR_DEFAULT_CHANNEL, ADC_ATTEN_11db));
		adcAttachPin(pin);
		auto read_raw = adc1_get_raw(WATER_LEVEL_SENSOR_DEFAULT_CHANNEL);
		adc_power_off();
		return static_cast<uint16_t>(read_raw);
	}

public:
	//i2c 인스턴스 초기화
	WaterLevel(gpio_num_t _pin) : pin(_pin) {
		isFirstSend = true;
	}

	WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN) {
	}

	void execute() {
		//센서 값 파싱
		const uint16_t rawValue = this->getRawValue();
		const uint16_t minCheckValue = max(rawValue, WATER_MIN_THRESHOLD);
		const uint16_t maxCheckValue = min(minCheckValue, WATER_MAX_THRESHOLD);
		
		while(buffer.size() > WATER_RAW_BUFFER_SIZE){
			buffer.pop_front();
		}
		buffer.push_back(maxCheckValue);
		uint16_t avg = std::accumulate(buffer.begin(), buffer.end(), 0.0) / buffer.size();
		//for debug
		#ifdef WATER_LEVEL_DEBUG
			#if WATER_LEVEL_DEBUG == 1
			ESP_LOGE(typename(this), "%u : %u", buffer.size(), avg);
			#endif
		#endif	
		water_level_service_signal_t signal;
		signal.level = std::min(map(avg, WATER_MIN_THRESHOLD, WATER_MAX_THRESHOLD, 0, 101), 100L);
		
		//만약 현재 유량이 일정 수치보다 낮을 경우 alarm 신호 활성화
		if (signal.level <= WATER_LOW_THRESHOLD) {
            signal.onOff = 1u;
		} else {
			signal.onOff = 0u;
		}

		//마지막으로 보낸 신호와 지금 계산됀 값이 다를 경우 신호 전송
        if(signal.level != last.level || isFirstSend){
            sendAlarm(signal);
			isFirstSend = false;
        }
        last = signal;
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif