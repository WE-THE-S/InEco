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
	DFRobot_BME280_IIC bme(&Wire, BME280_ADDRESS);
	DFRobot_CCS811 ccs(&Wire, CCS811_ADDRESS);

	//알람 신호를 주변 서비스에 뿌리는 함수
	inline void send(const air_sensor_service_signal_t value) {
		service_signal_t signal;
		signal.type = SERVICE_SIGNAL_TYPE::AIR_SENSOR_VALUE;
		signal.value = value.value;
        ESP_LOGI(typename(this), "level %u", value.level);
		Broadcast<service_signal_t>::getInstance()->broadcast(signal);
	}

public:
	//i2c 인스턴스 초기화
	AirSensor() {
		bme.reset();
		while(bme.begin() != BME::eStatusOK) {
			ESP_LOGI(typename(this), "BME280 INIT FAIL");
			delay(100);
		}
		while(ccs.begin() != 0){
			ESP_LOGI(typename(this), "CCS INIT FAIL");
			delay(100);
		}
    	ccs.setMeasCycle(ccs.eCycle_250ms);
	}

	void execute() {
		if(ccs.checkDataReady()){
			const auto co2 = ccs.getCO2PPM();
			const auto tvoc = ccs.getTVOCPPB();
			const auto temp = bme.getTemperature();
			const auto humi = bme.getHumidity();
			airSignal.co2 = co2;
			airSignal.tvoc = tvoc;
			airSignal.temp = static_cast<int8_t>(temp);
			airSignal.humi = static_cast<uint8_t>(humi);
			send(airSignal);
		}
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif