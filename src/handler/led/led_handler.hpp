#ifndef __LED_HANDLER_HPP__
#define __LED_HANDLER_HPP__

#include "../../config.hpp"
#include "../../struct/handler/gpio_handler.hpp"
#include "../../struct/packet.hpp"
#include <Arduino.h>
#include <NeoPixelBus.h>
#include <Wire.h>
#include <cstdint>
#include <queue>
#include "../../struct/helper.hpp"
using namespace std;

//LED 메세지 핸들러
class LedHanlder : public MessageGpioControlHandler {
protected:
	//LED 제어를 위한 NeoPixelBus 인스턴스 포인터
	NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod> *strip = nullptr;

public:
	LedHanlder(gpio_num_t _led) : MessageGpioControlHandler(_led) {
	}

	LedHanlder() : LedHanlder(LED_CONTROL_DEFAULT_PIN) {
	}
	
	//다른 기기에서 LED설정 패킷이 날아오면 수신
	void onMessage(const device_communication_message_t message) {
        Helper helper;
		ESP_LOGI(typename(this), "Packet : %s", helper.bytesToHex(const_cast<uint8_t *>(message.bytes),
                         sizeof(device_communication_message_t)).c_str());
		//strip 변수가 초기화 되어 있지 않은경우, 할당
		if (strip == nullptr) {
			strip = new NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod>(LED_COUNT, this->pin);
			strip->Begin();
		}
		//색상 설정이 필요하면 패킷에 저장됀 rgb값으로 LED 색상 변경
		if (message.type == MESSAGE_TYPE::SET_COLOR) {
			led_message_t led;
			led.message = message.message;
			if (0 == led.col && 0 == led.row) {
				RgbColor color(led.color.r, led.color.g, led.color.b);
				//지정한 색으로 연결된 모든 LED 색상 초기화
				strip->ClearTo(color);
				//LED에 변경 사항 적용
				strip->Show();
			}
		}
	}

	//LED 핸들러가 종료되는 경우 strip 인스턴스 제거
	~LedHanlder() {
		delete this->strip;
	}
};

#endif