#ifndef __LCD_HPP__
#define __LCD_HPP__

#include "../struct/packet.hpp"
#include "../struct/service.hpp"
#include "../utils/broadcast.hpp"
#include "../config.hpp"
#include <Arduino.h>
#include <U8g2lib.h>
#include <cmath>
#include <cstdlib>

U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/OLED_CS_PIN, /* dc=*/OLED_DC_PIN, /* reset=*/OLED_RESET_PIN);

class LCD : public Service {
private:
	//기간
	uint16_t intervalTime;

	//켜지는 시간
	uint16_t intervalSpan;

	//반복 실행 설정
	bool intervalEnable;

    //물 레벨
    uint8_t waterLevel;

	//onOff
	bool onOff;

	bool ledOn;
	
    char buffer[32] = {
		0,
	};

	void messageBuilder(const char *key, const char *value) {
		memset(buffer, 0x00, sizeof(char) * 32);
		sprintf(buffer, "%s : %s", key, value);
	}
	void messageBuilder(const char *key, const int value, const char *valueType) {
		memset(buffer, 0x00, sizeof(char) * 32);
		sprintf(buffer, "%s : %d%s", key, value, valueType);
	}

    void display(){
        u8g2.clearBuffer();
        u8g2.setDrawColor(2);
        u8g2.setFontMode(2);
        u8g2.setFontDirection(0);
        u8g2.setFont(u8g2_font_profont22_tr);
        u8g2.drawStr(0, 22, "Motor");
        u8g2.drawStr(0, 105, "Alarm");
        u8g2.setFont(u8g2_font_profont15_tr);
        this->messageBuilder("Interval", this->intervalEnable ? "Enable" : "Disable");
        u8g2.drawStr(0, 37, buffer);
        this->messageBuilder("Span", this->intervalSpan, "ms");
        u8g2.drawStr(0, 52, buffer);
        this->messageBuilder("Time", this->intervalTime, "ms");
        u8g2.drawStr(0, 67, buffer);
        this->messageBuilder("level", this->waterLevel, "%");
        u8g2.drawStr(0, 82, buffer);
        this->messageBuilder("Status", this->ledOn ? "ON" : "OFF");
        u8g2.drawStr(0, 120, buffer);
        u8g2.sendBuffer();
    }

public:
    LCD() {
        intervalEnable = false;
        onOff = false;
        ledOn = false;
        intervalSpan = MOTOR_DEFAULT_SPAN;
        intervalTime = MOTOR_DEFAULT_TIME;
        waterLevel = 0u;
    }
    void execute(){
        u8g2.begin();
        this->display();
    }

    void onMessage(const service_signal_t message) {
        bool isSet = true;
        switch (message.type) {
            case SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET: {
                motor_interval_service_signal_t signal;
                signal.value = message.value;
                if (signal.isIntervalSet) {
                    this->intervalSpan = signal.intervalSpan;
                    this->intervalTime = signal.intervalTime;
                    this->intervalEnable = signal.intervalEnable;
                    ESP_LOGI(typename(this), "Enable %u", this->intervalEnable);
                    ESP_LOGI(typename(this), "Span %u", this->intervalSpan);
                    ESP_LOGI(typename(this), "Time %u", this->intervalTime);
                } else {
                    this->onOff = signal.onOff;
                    ESP_LOGI(typename(this), "Motor %s", this->onOff ? "ON" : "OFF");
                }
                break;
            }
            case SERVICE_SIGNAL_TYPE::ALARM: {
                water_level_service_signal_t signal;
                signal.value = message.value;
                if (signal.onOff) {
                    this->ledOn = true;
                } else {
                    this->ledOn = false;
                }
                this->waterLevel = signal.level;
                break;
            }
            default: {
                isSet = false;
                break;
            }
        }
        if(isSet) {
            this->display();
        }
    }
};
#endif