#ifndef __AIR_SENSOR_HPP__
#define __AIR_SENSOR_HPP__

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

#include <Wire.h>
#include <DFRobot_BME280.h>
#include <DFRobot_CCS811.h>

//analog waterlevel 모듈과 통신하여 현재 유량을 측정하는 클래스
class AirSensor : public Service {
private:
	//마지막으로 보낸 현재 공기
	air_sensor_service_signal_t last;
	DFRobot_BME280_IIC bme(&Wire, 0x77);
	DFRobot_CCS811 ccs;

	//알람 신호를 주변 서비스에 뿌리는 함수
	inline void sendAlarm(const air_sensor_service_signal_t value) {
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::AIR_SENSOR_VALUE;
		signal.value = value.value;
        ESP_LOGI(typename(this), "level %u", value.level);
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

public:
	//i2c 인스턴스 초기화
	AirSensor() {
		bme.
	}

	void execute() {
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif