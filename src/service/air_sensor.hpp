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
#include <SparkFunBME280.h>
#include <SparkFunCCS811.h>

	
//공기 센서 
class AirSensor : public Service {
private:
//마지막으로 보낸 현재 공기
	BME280 bme;
	air_sensor_service_signal_t last;
	CCS811 ccs;

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
		Wire.begin();
		ESP_LOGI(typename(this), "bme : %u", bme.begin());
		ESP_LOGI(typename(this), "ccs : %u", ccs.begin());
	}

public:
	//i2c 인스턴스 초기화
	AirSensor() : ccs(CCS811_ADDRESS) {
		//***Driver settings********************************//
		//commInterface can be I2C_MODE
		//specify I2C address.  Can be 0x77(default) or 0x76

		//For I2C, enable the following
		bme.settings.commInterface = I2C_MODE;
		bme.settings.I2CAddress = BME280_ADDRESS;

		//***Operation settings*****************************//

		//runMode can be:
		//  0, Sleep mode
		//  1 or 2, Forced mode
		//  3, Normal mode
		bme.settings.runMode = 3; //Forced mode

		//tStandby can be:
		//  0, 0.5ms
		//  1, 62.5ms
		//  2, 125ms
		//  3, 250ms
		//  4, 500ms
		//  5, 1000ms
		//  6, 10ms
		//  7, 20ms
		bme.settings.tStandby = 0;

		//filter can be off or number of FIR coefficients to use:
		//  0, filter off
		//  1, coefficients = 2
		//  2, coefficients = 4
		//  3, coefficients = 8
		//  4, coefficients = 16
		bme.settings.filter = 0;

		//tempOverSample can be:
		//  0, skipped
		//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
		bme.settings.tempOverSample = 1;

		//pressOverSample can be:
		//  0, skipped
		//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
		bme.settings.pressOverSample = 1;

		//humidOverSample can be:
		//  0, skipped
		//  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
		bme.settings.humidOverSample = 1;
		ESP_LOGI(typename(this), "sda : %u", SDA);
		ESP_LOGI(typename(this), "scl : %u", SCL);
		init();
	}

	void execute() {
		if (ccs.dataAvailable()) {
			ccs.readAlgorithmResults();

			const float rawTemp = bme.readTempC();
			const float rawHumi = bme.readFloatHumidity(); 
			const uint16_t co2 = ccs.getCO2();
			const uint16_t tvoc = ccs.getTVOC();
			const int8_t temp = static_cast<int8_t>(rawTemp);
			const uint8_t humi = static_cast<uint8_t>(rawHumi);

    		ccs.setEnvironmentalData(rawHumi, rawTemp);
			last.co2 = co2;
			last.tvoc = tvoc;
			last.temp = temp;
			last.humi = humi;
			send(last);
		}else if(ccs.checkForStatusError()){
			ESP_LOGW(typename(this), "error code : %u", ccs.getErrorRegister());
			init();
		}

	
	}

	void onMessage(const service_signal_t message) {
	}
};

#endif