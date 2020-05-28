#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <SoftwareSerial.h>
#include <Arduino.h>

const uint32_t FW_VERSION = 0;
const bool IS_DEV = true;
const unsigned long SOFTWARE_UART_BAUDRATE = 460800;
const unsigned long HARDWARE_UART_BAUDRATE = SOFTWARE_UART_BAUDRATE; //테스트용
const uint32_t HARDWARE_UART_SERIAL_MODE = SERIAL_8N1;
const SoftwareSerialConfig SOFTWARE_UART_SERIAL_MODE = SWSERIAL_8N1;

const gpio_num_t BOTTOM_UART_RX = GPIO_NUM_14;
const gpio_num_t BOTTOM_UART_TX = GPIO_NUM_27;
const gpio_num_t RIGHT_UART_RX = GPIO_NUM_26;
const gpio_num_t RIGHT_UART_TX = GPIO_NUM_25;

const gpio_num_t MOTOR_DEFAULT_PIN = GPIO_NUM_32;
const gpio_num_t SLAVE_ENABLE_PIN = GPIO_NUM_33;
const gpio_num_t WATER_LEVEL_SENSOR_DEFAULT_PIN = GPIO_NUM_34;
const gpio_num_t ALARM_LED_DEFAULT_PIN = GPIO_NUM_13;
const gpio_num_t LED_CONTROL_DEFAULT_PIN = ALARM_LED_DEFAULT_PIN;

const gpio_num_t OLED_CLK_PIN = GPIO_NUM_18;
const gpio_num_t OLED_CS_PIN = GPIO_NUM_12;
const gpio_num_t OLED_DC_PIN = GPIO_NUM_17;
const gpio_num_t OLED_RESET_PIN = GPIO_NUM_19;

const uint16_t MOTOR_DEFAULT_SPAN = 60000u;
const uint16_t MOTOR_DEFAULT_TIME = 1000u;
const uint16_t MOTOR_AIR_REMOVE_TIME = 1000u * 10u;
const uint8_t LED_COUNT = 4;
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
	request(`motor/set/enable/${enable.value}/time/${time.value}/span/${span.value}`);\
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