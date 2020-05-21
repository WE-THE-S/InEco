#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <Arduino.h>

//uart 통신용
#define BottomSerial Serial1
#define RightSerial Serial2

const uint32_t FW_VERSION = 0;
const bool IS_DEV = true;
const unsigned long UART_BAUD_RATE = 460800ul; //테스트용
const uint32_t UART_SERIAL_MODE = SERIAL_8N1;

const gpio_num_t BOTTOM_UART_RX = GPIO_NUM_23;
const gpio_num_t BOTTOM_UART_TX = GPIO_NUM_22;
const gpio_num_t RIGHT_UART_RX = GPIO_NUM_25;
const gpio_num_t RIGHT_UART_TX = GPIO_NUM_26;

const gpio_num_t MOTOR_DEFAULT_PIN = GPIO_NUM_33;
const gpio_num_t LED_CONTROL_DEFAULT_PIN = GPIO_NUM_32;
const gpio_num_t WATER_LEVEL_SENSOR_DEFAULT_PIN = GPIO_NUM_34;
const gpio_num_t ALARM_LED_DEFAULT_PIN = GPIO_NUM_21;

const uint16_t MOTOR_DEFAULT_SPAN = 60000u;
const uint16_t MOTOR_DEFAULT_TIME = 1000u;
const char *const SSID = "THES3";
const char *const PASSWORD = "1234123412";
const int WATER_LOW_THRESHOLD = 500;
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
	request(`motor/set?enable=${enable.value}&time=${time.value}&span=${span.value}`);\
  });\
  on.addEventListener('click', function(){\
	request('motor/power/on');\
  });\
  off.addEventListener('click', function(){\
	request('motor/power/off');\
  });\
</script>";

const ROMFN_ATTR char *const LED_SET_HTML = "\
<!DOCTYPE html>\
<h1>LED ON/OFF</h1>\
<input type='button' id='on' value='on' />\
<input type='button' id='off' value='off' />\
<script type='text/javascript'>\
	function request(url){\
		const xhttp = new XMLHttpRequest();\
    	xhttp.open('GET', url, true);\
		xhttp.send();\
    }\
  on.addEventListener('click', function(){\
	request('led/power/on');\
  });\
  off.addEventListener('click', function(){\
	request('led/power/off');\
  });\
</script>";

#endif