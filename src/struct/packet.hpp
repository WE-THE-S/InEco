#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#include <cstdint>

using namespace std;

const uint8_t PACKET_SIZE = 6;

#pragma pack(push, 1)
enum class MESSAGE_TYPE : uint8_t {
    SET_COLOR,
    RUN_MOTOR,
    GET_SIZE
};

enum class MESSAGE_DIRECTION : uint8_t {
    TO_MASTER = 0x80,
    TO_SLAVE = 0x0,
};

enum class MOTOR_STATUS : uint8_t {
    MOTOR_OFF,
    MOTOR_ON,
};

enum class PACKET_OUTPUT_DIRECTION : uint8_t {
    MASTER,
    RIGHT,
    BOTTOM
};

typedef struct _base_message_t {
    uint8_t bytes[PACKET_SIZE];
} base_message_t;

typedef struct _color_t{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t bright;
} color_t;

typedef union _led_message_t {
    struct {
        uint8_t col; //열
        uint8_t row; //행, 줄
        color_t color;
    };
    base_message_t message;
} led_message_t;

typedef union _motor_message_t {
    struct {
        MOTOR_STATUS status;
    };
    base_message_t message;
} motor_message_t;

typedef union __device_communication_message_t {
    struct{
        union {
            MESSAGE_TYPE type;
            MESSAGE_DIRECTION dir;
        };
        base_message_t message;
        uint8_t crc;
    };
    uint8_t bytes[];
    uint8_t getCrc(){
        return static_cast<uint8_t>(bytes[0] ^ bytes[1] ^ bytes[2] ^ bytes[3] ^ bytes[4] ^ bytes[5] ^ bytes[6]);
    }
} device_communication_message_t;
#pragma pack(pop)
#endif