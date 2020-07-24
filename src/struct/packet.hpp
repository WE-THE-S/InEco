#ifndef __PACKET_HPP__
#define __PACKET_HPP__
#include <cstdint>

//기본 패킷 사이즈
const uint8_t PACKET_SIZE = 6;

//컴파일러에서 struct에 padding을 넣는걸 방지하기 위한 pragma 플래그 설정
#pragma pack(push, 1)

//message type enum 정의
enum class MESSAGE_TYPE : uint8_t {
    MODULE_RESTART,
    SET_COLOR,
    RUN_MOTOR,
    GET_SIZE
};

//message 방향 정의
enum class MESSAGE_DIRECTION : uint8_t {
    TO_MASTER = 0x80,
    TO_SLAVE = 0x0,
};

//모터 상태값
enum class MOTOR_STATUS : uint8_t {
    MOTOR_OFF,
    MOTOR_ON,
};


//기본 메세지 패킷 struct
typedef struct _base_message_t {
    uint8_t bytes[PACKET_SIZE];
} base_message_t;

//색상 데이터
typedef struct _color_t{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t bright;
} color_t;

//led 설정을 위한 패킷
typedef union _led_message_t {
    struct {
        uint8_t col; //열
        uint8_t row; //행, 줄
        color_t color;
    };
    base_message_t message;
} led_message_t;

//모터 설정을 위한 패킷
typedef union _motor_message_t {
    MOTOR_STATUS status;
    base_message_t message;
} motor_message_t;

/***
 * @brief 장치간 통신을 위한 패킷
 **/
typedef union __device_communication_message_t {
    struct{
        MESSAGE_TYPE type;
        MESSAGE_DIRECTION dir;
        base_message_t message;
    };
    uint8_t bytes[];
} device_communication_message_t;

//기기 내부에서 서비스간 통신을 위한 packet type
enum class SERVICE_SIGNAL_TYPE : uint8_t {
    ALARM,
    WATER_LEVEL,
    MOTOR_INTERVAL_SET,
    PACKET_SEND
};

//서비스간 신호 전달용 기본 구조
typedef struct _service_signal_t {
    SERVICE_SIGNAL_TYPE type; 
    union{
        uint64_t value;
        uint8_t bytes[];
    };
} service_signal_t;

//water level 신호 전달
typedef union _water_level_service_signal_t {
    uint64_t value;
    struct{
        uint8_t level;
        bool onOff;
    };
} water_level_service_signal_t;

//모터 ON/OFF 및 반복 서비스 설정 신호 전달
typedef union _motor_interval_service_signal_t {
    uint64_t value;
    struct{
        uint32_t intervalTime;
        uint16_t intervalSpan;
        uint8_t isIntervalSet : 1;
        uint8_t intervalEnable : 1;
        uint8_t onOff : 1;
    };
} motor_interval_service_signal_t;

//외부 모듈에 데이터를 보내기 위한 packet
//내부 서비스로 보내진 다음 base_message_t* 에 있는 값이 전송됀다.
typedef union _communcation_service_signal_t {
    uint64_t value;
    struct{
        union {
            struct{
                MESSAGE_TYPE type;
                MESSAGE_DIRECTION dir;
            };
            uint8_t header;
        };
        base_message_t* message;
    };
} communcation_service_signal_t;

#pragma pack(pop)
#endif