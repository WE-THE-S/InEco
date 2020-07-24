#ifndef __LED_TRANSLATE_HPP__
#define __LED_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/helper.hpp"
//LED 보드에서 사용하는 신호 수신용 모듈
class LedTranslate : public Translate {
    protected:
    void broadcast(const device_communication_message_t message){
        //데이터가 들어온 경우 뿌려주는 함수
        Broadcast<device_communication_message_t>::getInstance()->broadcast(message);
    }

    public:
    LedTranslate() : Translate() {
    }

    void recv(){ 
        //위나 컨트롤 보드에서 신호가 날아오면 수신하는 부분
        if(this->master->available() >= sizeof(device_communication_message_t)){
            //수신한 데이터의 크기가 기본 사이즈보다 크면 패킷 파싱 처리 시작
            auto message = new device_communication_message_t;
            this->master->readBytes(message->bytes, sizeof(device_communication_message_t)); 
            this->broadcast(*message);
            Helper helper;
            //받아온 패킷 데이터 확인
            ESP_LOGI(typename(this), "Packet : %s", helper.bytesToHex(message->bytes, sizeof(device_communication_message_t)).c_str());
            switch(message->type){
                //메세지 타입별 명령어 수행
                case MESSAGE_TYPE::RUN_MOTOR : 
                case MESSAGE_TYPE::MODULE_RESTART : {
                    send(message);
                    if(message->type == MESSAGE_TYPE::MODULE_RESTART){
                        delay(10);
                        ESP.restart();
                    }
                    break;
                }
                case MESSAGE_TYPE::SET_COLOR : {
                    //LED의 경우 좌표가 지정되어 있어서 현 모듈에 요청한거 아니면 해당 부분으로 보냄
                    auto led = reinterpret_cast<led_message_t*>(&(message->message));
                    if(led->col > 0){
                        led->col -= 1;
                        rightSend(message);
                        led->col += 1;
                    }
                    if(led->row > 0){
                        led->row -= 1;
                        bottomSend(message);
                    }
                    break;
                }
                default : break;
            }
            delete message;
        }
    }
};

#endif