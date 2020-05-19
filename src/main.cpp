#include <Arduino.h>
#include "./config.hpp"
#include "./struct/packet.hpp"

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
    if(Serial.available() >= sizeof(device_communication_message_t)){
      auto message = new device_communication_message_t;
      Serial.readBytes(message->bytes, sizeof(device_communication_message_t)); 
      switch(message->type){
        case MessageType::RUN_MOTOR : {
          BottomSerial.write(message->bytes, sizeof(device_communication_message_t));
          RightSerial.write(message->bytes, sizeof(device_communication_message_t));
          break;
        }
        case MessageType::SET_COLOR : {
          auto led = reinterpret_cast<led_message_t*>(&(message->message));
          if(led->row == 0 & led->col == 0){
            
          }else{
            led->col -= 1;
            RightSerial.write(message->bytes, sizeof(device_communication_message_t));
            led->col += 1;
            led->row -= 1;
            BottomSerial.write(message->bytes, sizeof(device_communication_message_t));
          }
          break;
        }
        default : break;
      }
      delete message;
    }
  #elif defined(CONTROL_BOARD)
    
  #endif
}