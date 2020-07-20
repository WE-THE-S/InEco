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

const uint8_t NO_TOUCH = 0xFE;
const uint8_t THRESHOLD = 100;
const uint8_t ATTINY1_HIGH_ADDR = 0x78;
const uint8_t ATTINY2_LOW_ADDR = 0x77;

class WaterLevel : public Service {
private:
	unsigned char low_data[8] = {
		0,
	};
	unsigned char high_data[12] = {
		0,
	};

    const int sensorvalue_min = 250;
    const int sensorvalue_max = 255;

	bool lastStatus;
	gpio_num_t pin;
	inline void sendAlarm(const uint64_t value) {
		lastStatus = (value != 0);
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::ALARM;
		signal.value = value;
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

	void getHigh12SectionValue(void) {
		memset(high_data, 0, sizeof(high_data));
		Wire.requestFrom(ATTINY1_HIGH_ADDR, 12);
		while (12 != Wire.available())
			;

		for (int i = 0; i < 12; i++) {
			high_data[i] = Wire.read();
		}
		delay(10);
	}

	void getLow8SectionValue(void) {
		memset(low_data, 0, sizeof(low_data));
		Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
		while (8 != Wire.available())
			;

		for (int i = 0; i < 8; i++) {
			low_data[i] = Wire.read(); // receive a byte as character
		}
		delay(10);
	}

public:
	WaterLevel(gpio_num_t _pin) : pin(_pin) {
	}

	WaterLevel() : WaterLevel(WATER_LEVEL_SENSOR_DEFAULT_PIN) {
	}

	void execute() {
		int low_count = 0;
		int high_count = 0;
		uint32_t touch_val = 0;
		uint8_t trig_section = 0;
		this->getLow8SectionValue();
		this->getHigh12SectionValue();

		for (int i = 0; i < 8; i++) {
			if (low_data[i] >= sensorvalue_min && low_data[i] <= sensorvalue_max) {
				low_count++;
			}
		}
		for (int i = 0; i < 12; i++) {
			if (high_data[i] >= sensorvalue_min && high_data[i] <= sensorvalue_max) {
				high_count++;
			}
		}
		for (int i = 0; i < 8; i++) {
			if (low_data[i] > THRESHOLD) {
				touch_val |= 1 << i;
			}
		}
		for (int i = 0; i < 12; i++) {
			if (high_data[i] > THRESHOLD) {
				touch_val |= (uint32_t)1 << (8 + i);
			}
		}

		while (touch_val & 0x01) {
			trig_section++;
			touch_val >>= 1;
		}
        water_level_service_signal_t signal;
		signal.level = static_cast<uint8_t>(trig_section * 5);
		if (signal.level <= WATER_LOW_THRESHOLD) {
			ESP_LOGI(typename(this), "raw : turn on");
            signal.onOff = 1u;
		} else {
			ESP_LOGI(typename(this), "raw : turn off");
			signal.onOff = 0u;
		}
        sendAlarm(signal.value);
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif