#include "../src/struct/packet.hpp"
#include <Arduino.h>
#include <unity.h>

using namespace std;

void test_packet_size(void){
    TEST_ASSERT_EQUAL(sizeof(base_message_t), 7);
    TEST_ASSERT_EQUAL(sizeof(base_message_t), 7);
    TEST_ASSERT_EQUAL(sizeof(led_message_t), 7);
    TEST_ASSERT_EQUAL(sizeof(motor_message_t), 7);
    TEST_ASSERT_EQUAL(sizeof(con_led_message_t), 7);
    TEST_ASSERT_EQUAL(sizeof(device_communication_message_t), 8);
}

void setup(){
    UNITY_BEGIN();

    RUN_TEST(test_packet_size);
    
    UNITY_END();
}

void loop(){

}