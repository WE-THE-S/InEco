#ifndef __MOTOR_INTERVAL_HPP__
#define __MOTOR_INTERVAL_HPP__

#include <Arduino.h>
#include <cmath>
#include "../struct/packet.hpp"
#include "../struct/service.hpp"
#include "../utils/broadcast.hpp"
#include "../config.hpp"

class MotorInterval : public Service {
    private:
        //기간
        uint32_t intervalTime;

        //켜지는 시간
        uint16_t intervalSpan;
        
        //마지막으로 켜진 시간
        uint64_t lastTime;

        uint64_t airClearStartTime;

        //마지막으로 타이머가 설정한 값
        MOTOR_STATUS lastIntervalStatus;

        //반복 실행 설정
        bool intervalEnable; 

        //onOff
        bool onOff;
        
        bool flag;

        //모터 세기를 설정
        MOTOR_STATUS sendMessage(MOTOR_STATUS status) const {
            //사용 가능 플래그가 설정되지 않은 경우, 무조건 OFF
            status = (flag) ? status : MOTOR_STATUS::MOTOR_OFF;
            switch(status){
                //분사가 필요하면 최소 출력으로 모터를 송출
                //control board가 아닌경우 솔레노이드 벨브를 엶
                case MOTOR_STATUS::MOTOR_ON : {
                    #if CONTROL_BOARD == 1
                        ledcWrite(PWM_CHANNEL, PWM_HIGH);
                    #else
                        digitalWrite(WATER_SOLENOID_VALVE_PIN, LOW);
                    #endif
                    break;
                }
                //공기가 분사 이후에도 계속 남아있는 문제가 있어서 최소 출력으로 계속 가동
                case MOTOR_STATUS::MOTOR_OFF : {
                    #if CONTROL_BOARD == 1
                        //ledcWrite(PWM_CHANNEL, PWM_LOW);
                    #else
                        digitalWrite(WATER_SOLENOID_VALVE_PIN, HIGH);
                    #endif
                    break;
                }
                default : break;
            }
            //주변 장치들에 뿌리기 위해서 communication_service에 명령어 패킷을 전달
            service_signal_t signal;
            communcation_service_signal_t com;
            motor_message_t* motorMessage = new motor_message_t;
            memset(motorMessage->message.bytes, 0x00, sizeof(motor_message_t));
            auto broadcast = Broadcast<service_signal_t>::getInstance();
            
            motorMessage->status = status;

            com.dir = MESSAGE_DIRECTION::TO_SLAVE;
            com.type = MESSAGE_TYPE::RUN_MOTOR;
            com.message = &motorMessage->message;
            
            signal.value = com.value;
            signal.type = SERVICE_SIGNAL_TYPE::PACKET_SEND;

            broadcast->broadcast(signal);

            delete motorMessage;
            return status;
        }
    public:

    //모터 서비스 init
    MotorInterval() {
        flag = false;
        intervalEnable = true;
        onOff = false;
        lastIntervalStatus = MOTOR_STATUS::MOTOR_OFF;
        lastTime = 0;
        airClearStartTime = 0;
        intervalSpan = MOTOR_DEFAULT_SPAN;
        intervalTime = MOTOR_DEFAULT_TIME;
        pinMode(MOTOR_DEFAULT_PIN, OUTPUT);
        pinMode(WATER_SOLENOID_VALVE_PIN, OUTPUT);
        #if CONTROL_BOARD == 1
            ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
            ledcAttachPin(MOTOR_DEFAULT_PIN, PWM_CHANNEL);
            //ledcWrite(PWM_CHANNEL, PWM_LOW);
        #else
            digitalWrite(WATER_SOLENOID_VALVE_PIN, HIGH);
        #endif
    }

    //공기를 제거하는 과정
    //모든 솔레노이드 벨브를 닫고 최대 출력으로 물을 순환시켜, 공기를 제거함
    void removeAir(){
        #if CONTROL_BOARD == 1
            sendMessage(MOTOR_STATUS::MOTOR_OFF);
            delay(MOTOR_COMMAND_TIMEOUT);
            ledcWrite(PWM_CHANNEL, PWM_MAX);
        #else
            digitalWrite(WATER_SOLENOID_VALVE_PIN, HIGH);
        #endif
        airClearStartTime = millis();
    }

    //계속 실행되면서 시간을 체크
    //시간은 
    void execute(){
        if(airClearStartTime != 0){
            if(airClearStartTime + MOTOR_AIR_REMOVE_TIME < millis()){
                #if CONTROL_BOARD == 1
                //ledcWrite(PWM_CHANNEL, PWM_LOW);
                ledcWrite(PWM_CHANNEL, PWM_HIGH);
                #else
                digitalWrite(WATER_SOLENOID_VALVE_PIN, LOW);
                #endif
                airClearStartTime = 0;
            }
        }
        if(this->intervalEnable){
            const uint64_t now = millis();
            const auto requireOffTime = this->lastTime + this->intervalSpan;
            const auto requireOnTime = this->lastTime + this->intervalTime;

            switch(this->lastIntervalStatus){
                case MOTOR_STATUS::MOTOR_OFF : {
                    if(requireOnTime <= now){
                        ESP_LOGI(typename(this), "motor interval on %u", now);
                        this->lastTime = now;
                        this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_ON);
                    }
                    break;
                }
                case MOTOR_STATUS::MOTOR_ON : {
                    if(requireOffTime <= now){
                        ESP_LOGI(typename(this), "motor interval off %u", now);
                        this->lastTime = now;
                        this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_OFF);
                    }
                    break;
                }
            }

        }
    }

    //다른 서비스에서 broadcast 메세지가 온경우 실행
    void onMessage(const service_signal_t message){
        //메세지 종류가 모터 제어 관련이면
        switch(message.type){
            case SERVICE_SIGNAL_TYPE::MOTOR_INTERVAL_SET : {
                //내부 내용을 파싱해서 필요한 정보를 설정
                motor_interval_service_signal_t signal;
                signal.value = message.value;
                if(signal.isIntervalSet){
                    this->intervalSpan = signal.intervalSpan;
                    this->intervalTime = signal.intervalTime;
                    if(signal.intervalEnable != this->intervalEnable){
                        this->lastIntervalStatus = this->sendMessage(MOTOR_STATUS::MOTOR_OFF);
                        this->lastTime = min(millis() - signal.intervalTime, 0ul);
                    }
                    this->intervalEnable = signal.intervalEnable;
                    ESP_LOGI(typename(this), "Enable %u", this->intervalEnable);
                    ESP_LOGI(typename(this), "Span %u", this->intervalSpan);
                    ESP_LOGI(typename(this), "Time %u", this->intervalTime);
                    ESP_LOGI(typename(this), "LastRunningTime %u", this->lastTime);
                    ESP_LOGI(typename(this), "LastMotorStatus %u", this->lastIntervalStatus != MOTOR_STATUS::MOTOR_OFF);
                }else{
                    this->onOff = signal.onOff;
                    ESP_LOGI(typename(this), "Motor %s", this->onOff ? "ON" : "OFF");
                    this->sendMessage(this->onOff ? MOTOR_STATUS::MOTOR_ON : MOTOR_STATUS::MOTOR_OFF);
                }
                break;
            }
            case SERVICE_SIGNAL_TYPE::ALARM : {
                water_level_service_signal_t signal;
                signal.value = message.value;
                flag = !signal.onOff;
                break;
            }
            default : break;
        }
    }
};


#endif