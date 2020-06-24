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

class LedHanlder : public MessageGpioControlHandler {
protected:
	NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod> *strip = nullptr;

public:
	LedHanlder(gpio_num_t _led) : MessageGpioControlHandler(_led) {
	}

	LedHanlder() : LedHanlder(LED_CONTROL_DEFAULT_PIN) {
	}

	void onMessage(const device_communication_message_t message) {
        Helper helper;
		ESP_LOGI(typename(this), "Packet : %s", helper.bytesToHex(const_cast<uint8_t *>(message.bytes),
                         sizeof(device_communication_message_t)).c_str());
		if (strip == nullptr) {
			strip = new NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1800KbpsMethod>(LED_COUNT, this->pin);
			strip->Begin();
		}
		if (message.type == MESSAGE_TYPE::SET_COLOR) {
			led_message_t led;
			led.message = message.message;
			if (0 == led.col && 0 == led.row) {
				RgbColor color(led.color.r, led.color.g, led.color.b);
				strip->ClearTo(color);
				strip->Show();
			}
		}
	}
	~LedHanlder() {
		delete this->strip;
	}
};

#endif