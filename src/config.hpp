#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <Arduino.h>
#include <inttypes.h>

//통신할때 사용하는 상수
const uint32_t FW_VERSION = 0;
const unsigned long HARDWARE_UART_BAUDRATE = 460800ul; //테스트용
const uint32_t HARDWARE_UART_SERIAL_MODE = SERIAL_8N1;
const size_t PACKET_RETRY_COUNT = 5;

//통신용 GPIO 설정
const gpio_num_t BOTTOM_UART_RX = GPIO_NUM_14;
const gpio_num_t BOTTOM_UART_TX = GPIO_NUM_27;
const gpio_num_t RIGHT_UART_RX = GPIO_NUM_26;
const gpio_num_t RIGHT_UART_TX = GPIO_NUM_25;

//주변 모듈과 통신하기 위한 GPIO 설정
const gpio_num_t MOTOR_DEFAULT_PIN = GPIO_NUM_33;
const gpio_num_t WATER_SOLENOID_VALVE_PIN = MOTOR_DEFAULT_PIN;
const gpio_num_t WATER_LEVEL_SENSOR_DEFAULT_PIN = GPIO_NUM_34;
const gpio_num_t ALARM_LED_DEFAULT_PIN = GPIO_NUM_13;
const gpio_num_t LED_CONTROL_DEFAULT_PIN = ALARM_LED_DEFAULT_PIN;
const gpio_num_t INTERVAL_BUTTON_DEFAULT_PIN = GPIO_NUM_4;
const gpio_num_t RUNTIME_BUTTON_DEFAULT_PIN = GPIO_NUM_15;

//OLED GPIO 설정
const gpio_num_t OLED_CLK_PIN = GPIO_NUM_18;
const gpio_num_t OLED_CS_PIN = GPIO_NUM_12;
const gpio_num_t OLED_DC_PIN = GPIO_NUM_5;
const gpio_num_t OLED_RESET_PIN = GPIO_NUM_19;

//시간 계산용 상수
const uint16_t S_TO_MS_FACTOR = 1000u;
const uint16_t MS_TO_US_FACTOR = S_TO_MS_FACTOR;
const uint16_t MAX_MOTOR_SPAN = 5u;
const uint16_t MAX_MOTOR_INTERVAL = 5u;
const uint16_t MOTOR_DEFAULT_SPAN = 1000u;
const uint32_t MOTOR_DEFAULT_TIME = 600u * S_TO_MS_FACTOR;
const uint16_t MOTOR_AIR_REMOVE_TIME = 1000u * 10u;
const uint16_t MOTOR_COMMAND_TIMEOUT = 1 * S_TO_MS_FACTOR;
const uint8_t LED_COUNT = 4;

//http
const uint16_t HTTP_SERVER_PORT = 80;
const char *const SSID = "THES3";
const char *const PASSWORD = "1234123412";
const uint8_t WATER_LOW_THRESHOLD = 20u;
const char *const WEBSOCKET_SERVER_HOST = "the-s.kr";
const uint16_t WEBSOCKET_SERVER_PORT = 8081u;

//motor pwm
const uint32_t PWM_FREQ = 5000;
const uint8_t PWM_CHANNEL = 0;
const uint8_t PWM_RESOLUTION = 8;
const uint8_t PWM_HIGH = 30;
const uint8_t PWM_LOW = 0;
const uint8_t PWM_MAX = UINT8_MAX;

//water sensor
const uint32_t I2C_TIMEOUT = 500;
const uint8_t NO_TOUCH = 0xFE;
const uint8_t THRESHOLD = 100;
const uint8_t ATTINY1_HIGH_ADDR = 0x78;
const uint8_t ATTINY2_LOW_ADDR = 0x77;
const uint8_t ATTINY1_HIGH_ADDR_SIZE = 12;
const uint8_t ATTINY2_LOW_ADDR_SIZE = 8;

//WATER 센서 threshold값
const int WATER_SENSOR_VALUE_MIN = 250;
const int WATER_SENSOR_VALUE_MAX = 255;

//웹 서버 표출용 http 코드
const ROMFN_ATTR char *const MOTOR_SET_HTML = "<!DOCTYPE html>\
\
<h1>Interval ON/OFF</h1>\
\
<label for='enable'>Enable : </label>\
\
<select id='enable'>\
  <option value='on'>on</option>\
  <option value='off'>off</option>\
</select>\
\
<br/>\
<label for='time'>Time : </label>\
<input id='time' type='number' placeholder='ms'/>\
\
<br/>\
<label for='span'>Span : </label>\
<input id='span' type='number' placeholder='ms'/>\
\
<br />\
<input id='intervalSet' type='button' value='Interval Set'/>\
\
<br />\
<h1>ON/OFF</h1>\
<input type='button' id='on' value='on' />\
<input type='button' id='off' value='off' />\
\
\
<script type='text/javascript'>\
	function request(url){\
		const xhttp = new XMLHttpRequest();\
    	xhttp.open('GET', url, true);\
		xhttp.send();\
    }\
  const set = document.getElementById('intervalSet');\
  const enable = document.getElementById('enable');\
  const time = document.getElementById('time');\
  const span = document.getElementById('span');\
  const on = document.getElementById('on');\
  const off = document.getElementById('off');\
  set.addEventListener('click', function(){\
	request(`motor/set/enable/${enable.value}/time/${time.value}/span/${span.value}`);\
  });\
  on.addEventListener('click', function(){\
	request('motor/power/on');\
  });\
  off.addEventListener('click', function(){\
	request('motor/power/off');\
  });\
</script>";


#endif