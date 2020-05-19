#include <Arduino.h>
#include "./config.hpp"

void setup() {
  Serial.begin(115200);
  BottomSerial.begin(UART_BAUD_RATE, UART_SERIAL_MODE, BOTTOM_UART_RX, BOTTOM_UART_TX);
  RightSerial.begin(UART_BAUD_RATE, UART_SERIAL_MODE, RIGHT_UART_RX, RIGHT_UART_TX);
  #if defined(LED_BOARD)
 
  #elif defined(CONTROL_BOARD)
      
  #endif
}

void loop() {
  #if defined(LED_BOARD)
  
  #elif defined(CONTROL_BOARD)
    
  #endif
}