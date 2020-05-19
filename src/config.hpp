#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__ 

#include <Arduino.h>

//uart 통신용
#define BottomSerial Serial1
#define RightSerial Serial2

const uint32_t FW_VERSION = 0;
const bool IS_DEV = true;
const unsigned long UART_BAUD_RATE = 3686400ul; //테스트용
const uint32_t UART_SERIAL_MODE = SERIAL_8N1;

const gpio_num_t BOTTOM_UART_RX = GPIO_NUM_18;
const gpio_num_t BOTTOM_UART_TX = GPIO_NUM_17;
const gpio_num_t RIGHT_UART_RX = GPIO_NUM_16;
const gpio_num_t RIGHT_UART_TX = GPIO_NUM_14;

const gpio_num_t MOTOR_DEFAULT_PIN = GPIO_NUM_12;
const gpio_num_t LED_CONTROL_DEFAULT_PIN = GPIO_NUM_10;

#endif