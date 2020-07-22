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

class WaterLevel : public Service {
private:
	uint8_t waterRawValue[ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE] = {0, };

	water_level_service_signal_t last;
	gpio_num_t pin;
	inline void sendAlarm(const water_level_service_signal_t value) {
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::ALARM;
		signal.value = value.value;
        ESP_LOGI(typename(this), "level %u", value.level);
        ESP_LOGI(typename(this), "alarm turn %s", value.onOff ? "On" : "Off");
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

	void getRawValue(void) {
		uint32_t now = millis();
		memset(waterRawValue, 0, ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE);
		Wire.requestFrom(ATTINY2_LOW_ADDR, ATTINY2_LOW_ADDR_SIZE);
		while (ATTINY2_LOW_ADDR_SIZE != Wire.available()){
			if(now + I2C_TIMEOUT> millis()){
				break;
			}
		}
			

		for (int i = 0; i < ATTINY2_LOW_ADDR_SIZE; i++) {
			waterRawValue[i] = Wire.read(); 
		}
		Wire.requestFrom(ATTINY1_HIGH_ADDR, ATTINY1_HIGH_ADDR_SIZE);
		while (ATTINY1_HIGH_ADDR_SIZE != Wire.available()){
			if(now + I2C_TIMEOUT> millis()){
				break;
			}
		}

		for (int i = 0; i < ATTINY1_HIGH_ADDR_SIZE; i++) {
			waterRawValue[ATTINY2_LOW_ADDR_SIZE + i] = Wire.read();
		}
		#if CORE_DEBUG_LEVEL > 3
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
	WaterLevel(gpio_num_t _pin) : pin(_pin) {
        Wire.begin();
	}

	WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN) {
	}

	void execute() {
		this->getRawValue();

        water_level_service_signal_t signal;
		signal.level = 0;
		for (int i = 0; i < ATTINY2_LOW_ADDR_SIZE + ATTINY1_HIGH_ADDR_SIZE; i++) {
			if (this->waterRawValue[i] >= THRESHOLD) {
				signal.level = (5 * (i + 1));
			}else{
				break;
			}
		}

		if (signal.level <= WATER_LOW_THRESHOLD) {
            signal.onOff = 1u;
		} else {
			signal.onOff = 0u;
		}
        if(signal.level != last.level){
            sendAlarm(signal);
        }
        last = signal;
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif