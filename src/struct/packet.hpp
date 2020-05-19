#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#include <cstdint>

using namespace std;

#pragma pack(push, 1)
enum class MessageType : uint8_t {
    SET_COLOR,
    RUN_MOTOR,
    CON_SET_COLOR,
    CON_SET_MOTOR,
    GET_SIZE
};

enum class MessageDirection : uint8_t {
    TO_MASTER,
    TO_SLAVE,
};

typedef struct _base_message_t {
    uint8_t bytes[7];
} base_message_t;

typedef union _led_message_t {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    base_message_t message;
} led_message_t;

typedef union _con_led_message_t  {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t count;
    };
    base_message_t message;
} con_led_message_t;

typedef union _motor_message_t {
    struct {
        uint8_t status;
    };
    base_message_t message;
} motor_message_t;

typedef struct __device_communication_message_t {
    MessageType type : 7;
    MessageDirection dir : 1;
    base_message_t message;
} device_communication_message_t;
#pragma pack(pop)
#endif