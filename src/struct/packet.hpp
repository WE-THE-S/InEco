#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#include <inttypes.h>

#pragma pack(push, 1)
enum class MessageType : uint8_t {
    SET_COLOR = 0,
    RUN_MOTOR = 1,
    SET = 2
};

typedef union _led_message_t {
    struct{

    };
    uint8_t bytes[];
} led_message_t;

typedef struct broadcast_message_t {
    MessageType type : 2;
    uint8_t length : 6; //최대 64byte
    uint8_t targetRow;
    uint8_t targetCol;
    uint8_t row;
    uint8_t col;
    uint8_t* value;
    uint8_t crc;
} message_t;
#pragma pack(pop)
#endif