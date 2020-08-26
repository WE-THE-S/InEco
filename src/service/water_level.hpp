#ifndef __WATER_LEVEL_HPP__
#define __WATER_LEVEL_HPP__

#include "../config.hpp"
#include "../struct/packet.hpp"
#include "../struct/service.hpp"
#include "../utils/broadcast.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <esp_adc_cal.h>
#include <soc/rtc_cntl_reg.h>

//analog waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class WaterLevel : public Service {
private:
	//마지막으로 보낸 현재 유량 상태
	water_level_service_signal_t last;

	//원래 analog타입 water level 센서를 사용했을때 사용하던 레거시 변수
	gpio_num_t pin;

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
		return analogRead(pin);
	}

public:
	//i2c 인스턴스 초기화
	WaterLevel(gpio_num_t _pin) : pin(_pin) {
        
	}

	WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN) {
	}

	void execute() {
		//센서 값 파싱
		const uint16_t rawValue = this->getRawValue();
		const uint16_t minCheckValue = max(rawValue, WATER_MIN_THRESHOLD);
		const uint16_t maxCheckValue = min(minCheckValue, WATER_MAX_THRESHOLD);
		
		ESP_LOGE(typename(this), "%u", rawValue);
        water_level_service_signal_t signal;
		signal.level = map(maxCheckValue, WATER_MIN_THRESHOLD, WATER_MAX_THRESHOLD, 0, 100);
		
		//만약 현재 유량이 일정 수치보다 낮을 경우 alarm 신호 활성화
		if (signal.level <= WATER_LOW_THRESHOLD) {
            signal.onOff = 1u;
		} else {
			signal.onOff = 0u;
		}

		//마지막으로 보낸 신호와 지금 계산됀 값이 다를 경우 신호 전송
        if(signal.level != last.level){
            sendAlarm(signal);
        }
        last = signal;
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif