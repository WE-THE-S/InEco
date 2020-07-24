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

//grove waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class WaterLevel : public Service {
private:
	//레지스터 값 저장용 변수
	uint8_t waterRawValue[ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE] = {0, };

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
	void getRawValue(void) {
		uint32_t now = millis();
		memset(waterRawValue, 0, ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE);
		Wire.requestFrom(ATTINY2_LOW_ADDR, ATTINY2_LOW_ADDR_SIZE);
		//최대 500ms까지 대기하다가 넘어감
		while (ATTINY2_LOW_ADDR_SIZE != Wire.available()){
			if(now + I2C_TIMEOUT> millis()){
				break;
			}
		}
			
		for (int i = 0; i < ATTINY2_LOW_ADDR_SIZE; i++) {
			waterRawValue[i] = Wire.read(); 
		}
		Wire.requestFrom(ATTINY1_HIGH_ADDR, ATTINY1_HIGH_ADDR_SIZE);
		//최대 500ms까지 대기하다가 넘어감
		while (ATTINY1_HIGH_ADDR_SIZE != Wire.available()){
			if(now + I2C_TIMEOUT> millis()){
				break;
			}
		}

		for (int i = 0; i < ATTINY1_HIGH_ADDR_SIZE; i++) {
			waterRawValue[ATTINY2_LOW_ADDR_SIZE + i] = Wire.read();
		}
		//water level debug가 활성화 됀 경우, register raw table을 log로 보여줌
		#ifdef WATER_LEVEL_DEBUG
			char* str = new char[256];
			memset(str, 0x00, sizeof(char) * 256);
			for(int i = 0;i < ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE; i++){
				sprintf(&str[strlen(str)], "%u,", waterRawValue[i]);
			}
			ESP_LOGI(typename(this), "Water : {%s}", str);
			delete[] str;
		#endif

	}

public:
	//i2c 인스턴스 초기화
	WaterLevel(gpio_num_t _pin) : pin(_pin) {
        Wire.begin();
	}

	WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN) {
	}

	void execute() {
		//센서 값 파싱
		this->getRawValue();

        water_level_service_signal_t signal;
		signal.level = 0;
		//레지스터에 저장됀 값으로 현재 유량 계산
		for (int i = 0; i < ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE; i++) {
			if (this->waterRawValue[i] >= THRESHOLD) {
				signal.level = (5 * (i + 1));
			}else{
				break;
			}
		}

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