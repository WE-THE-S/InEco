#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#include <cstdint>

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

/***
 * @brief 장치간 통신을 위한 패킷
 **/
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
        uint8_t crc = 0x00;
        for(uint8_t i = 0;i<sizeof(union __device_communication_message_t) - 1;i++){
            crc ^= bytes[i];
        }
        return crc;
    }
} device_communication_message_t;

enum class SERVICE_SIGNAL_TYPE : uint8_t {
    ALARM,
    WATER_LEVEL,
    MOTOR_INTERVAL_SET
};

typedef struct _service_signal_t {
    SERVICE_SIGNAL_TYPE type; 
    union{
        uint64_t value;
        uint8_t bytes[];
    };
} service_signal_t;

typedef union _motor_interval_service_signal_t {
    uint64_t value;
    struct{
        bool isIntervalSet;
        uint16_t intervalTime;
        uint16_t intervalSpan;
        bool enable;
        bool onOff;
    };
} motor_interval_service_signal_t;

#pragma pack(pop)
#endif