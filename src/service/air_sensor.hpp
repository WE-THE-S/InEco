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

	
//공기 센서 
class AirSensor : public Service {
private:
//마지막으로 보낸 현재 공기
	DFRobot_BME280_IIC bme;
	air_sensor_service_signal_t last;
	DFRobot_CCS811 ccs;

	//알람 신호를 주변 서비스에 뿌리는 함수
	inline void send(const air_sensor_service_signal_t value) {
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::AIR_SENSOR_VALUE;
		signal.value = value.value;
        ESP_LOGI(typename(this), "tvoc %u", value.tvoc);
        ESP_LOGI(typename(this), "co2 %u", value.co2);
        ESP_LOGI(typename(this), "temp %u", value.temp);
        ESP_LOGI(typename(this), "humi %u", value.humi);
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

	void init() {
		bme.reset();
		bme.begin();
		ccs.begin();
    	ccs.setMeasCycle(ccs.eCycle_250ms);
	}

public:
	//i2c 인스턴스 초기화
	AirSensor() : bme(&Wire, BME280_ADDRESS) {
		ESP_LOGI(typename(this), "sda : %u", SDA);
		ESP_LOGI(typename(this), "scl : %u", SCL);
		init();
	}

	void execute() {
		init();
		const auto start = millis() + 400;
		while(!ccs.checkDataReady()){
			if(millis() > start){
				break;
			}
			delay(10);
		}
		const auto temp = 0; // bme.getTemperature();
		const auto humi = 0; // bme.getHumidity();
		const auto co2 = ccs.getCO2PPM();
		const auto tvoc = ccs.getTVOCPPB();

		last.co2 = co2;
		last.tvoc = tvoc;
		last.temp = static_cast<int8_t>(temp);
		last.humi = static_cast<uint8_t>(humi);
		send(last);
	
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif