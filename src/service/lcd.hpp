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

//LCD 인스턴스 생성
U8G2_SSD1327_WS_128X128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/OLED_CS_PIN, /* dc=*/OLED_DC_PIN, /* reset=*/OLED_RESET_PIN);

class LCD : public Service {
private:
	//기간
	uint32_t intervalTime;

	//켜지는 시간
	uint16_t intervalSpan;

	//반복 실행 설정
	bool intervalEnable;

    //물 레벨
    uint8_t waterLevel;

	//onOff
	bool onOff;

    //alarm 상태
	bool ledOn;
	
    //문자열 저장용 임시 변수
    char buffer[32] = {
		0,
	};

    char timeFormat[4][3] = {
        {'m', 's', '\0'},
        {'s', '\0'},
        {'m', '\0'},
        {'h', '\0'}
    };

    std::pair<char*, int> timeLevel(uint32_t time) {
        std::pair<char*, int> result;
        if(time < 1000){
            result.first = timeFormat[0];
            result.second = time;
        }else if(time < 1000 * 60){
            result.first = timeFormat[1];
            result.second = (time / 1000);
        }else if(time < 1000 * 60 * 60){
            result.first = timeFormat[2];
            result.second = (time / 60 / 1000);
        }else{
            result.first = timeFormat[3];
            result.second = (time / 1000 / 60 / 60);
        }
        return result;
    }

    //대부분 문자열 출력 형식이 비슷해서 형식에 맞게 출력하는 함수
	void messageBuilder(const char *key, const char *value) {
		memset(buffer, 0x00, sizeof(char) * 32);
		sprintf(buffer, "%s : %s", key, value);
	}
	void messageBuilder(const char *key, const int value, const char *valueType) {
		memset(buffer, 0x00, sizeof(char) * 32);
		sprintf(buffer, "%s : %d%s", key, value, valueType);
	}

    void display(){
        //OLED에 표시됀 내용을 업데이트
        ESP_LOGD(typename(this), "Display update");
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
        auto span = timeLevel(this->intervalSpan);
        this->messageBuilder("Span", span.second, span.first);
        u8g2.drawStr(0, 52, buffer);
        auto time = timeLevel(this->intervalTime);
        this->messageBuilder("Time", time.second, time.first);
        u8g2.drawStr(0, 67, buffer);
        this->messageBuilder("level", this->waterLevel, "%");
        u8g2.drawStr(0, 82, buffer);
        this->messageBuilder("Status", this->ledOn ? "ON" : "OFF");
        u8g2.drawStr(0, 120, buffer);
        u8g2.sendBuffer();
    }

public:
    LCD() {
        //LCD에 표시할 초기 데이터 init
        intervalEnable = true;
        onOff = false;
        ledOn = false;
        intervalSpan = MOTOR_DEFAULT_SPAN;
        intervalTime = MOTOR_DEFAULT_TIME;
        waterLevel = 0u;
    }
    void execute(){
        //LCD초기화와 화면 표출
        u8g2.begin();
        this->display();
    }

    //다른 서비스에서 broadcast 메세지가 온경우 실행
    void onMessage(const service_signal_t message) {
        bool isSet = true;
        switch (message.type) {
            case SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET: {
                //모터 실행 주기가 변경됀 경우
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
                //알람 메세지가 전달됀 경우
                water_level_service_signal_t signal;
                signal.value = message.value;
                if(this->ledOn != signal.onOff){
                    ESP_LOGI(typename(this), "Time %u", signal.onOff);
                    this->ledOn = signal.onOff;
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