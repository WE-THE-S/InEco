#include "../src/struct/packet.hpp"
#include <Arduino.h>
#include <unity.h>

using namespace std;

//컴파일러에서 계산하는 내부 메세지의 크기가 맞는지 테스트
void test_packet_size(void){
    TEST_ASSERT_EQUAL(sizeof(base_message_t), 6);
    TEST_ASSERT_EQUAL(sizeof(base_message_t), 6);
    TEST_ASSERT_EQUAL(sizeof(led_message_t), 6);
    TEST_ASSERT_EQUAL(sizeof(motor_message_t), 6);
    TEST_ASSERT_EQUAL(sizeof(device_communication_message_t), 8);
}

//테스트 init
void setup(){
    UNITY_BEGIN();

    RUN_TEST(test_packet_size);
    
    UNITY_END();
}

//테스트 종료후 아무 작업도 하지 않음
void loop(){

}