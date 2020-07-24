#ifndef __CONTROL_TRANSLATE_HPP__
#define __CONTROL_TRANSLATE_HPP__

#include <Arduino.h>
#include "../struct/packet.hpp"
#include "../struct/translate.hpp"
#include "../struct/broadcast/service_signal_receiver.hpp"
#include "../struct/helper.hpp"

//컨트롤러 보드에서 사용하는 Translate
class ControlTranslate : public Translate, public ServiceSignalBroadcastReceiver {
    public:
        void recv(){
            //아래쪽에 연결됀 보드에서 보낸 데이터가 있을 경우 처리
            if(this->bottom->available() != 0){
                auto len = this->bottom->available();
                auto str = static_cast<char*>(calloc(sizeof(char), len + 1));
                this->bottom->readBytes((uint8_t*)str, len);
                ESP_LOGI(typename(this), "Bottom : %d", len);
                ESP_LOGI(typename(this), "Bottom : %s", str);
                //로그로 출력
                this->bottom->flush();
                //버퍼 지움
                free(str);
            }
            //오른쪽에 연결됀 보드에서 보낸 데이터가 있을 경우 처리
            if(this->right->available() != 0){
                auto len = this->right->available();
                auto str = static_cast<char*>(calloc(sizeof(char), len + 1));
                this->right->readBytes((uint8_t*)str, len);
                ESP_LOGI(typename(this), "Right : %d", len);
                ESP_LOGI(typename(this), "Right : %s", str);
                //로그로 출력
                this->right->flush();
                //버퍼 지움
                free(str);
            }
        }

        ControlTranslate() : Translate() {

        }

        //내부 broadcast 서비스에서 보내온 메세지 처리
        void onMessage(const service_signal_t message){
            //보내야할 패킷이면 전송
            if(message.type == SERVICE_SIGNAL_TYPE::PACKET_SEND){
                communcation_service_signal_t signal;
                device_communication_message_t packet;
                
                signal.value = message.value;
                packet.message = *signal.message;
                packet.type = signal.type;
                packet.dir = signal.dir;
                Helper helper;
                //패킷 내용 확인
                ESP_LOGI(typename(this), "Hex : %s", helper.bytesToHex(packet.bytes, sizeof(device_communication_message_t)).c_str());
                
                //색 설정인 경우 좌표가 있어서 우, 아래에 따라서 패킷 내용 변조해서 보냄
                if(signal.type == MESSAGE_TYPE::SET_COLOR){
                    led_message_t led;
                    led.message = *signal.message;
                    ESP_LOGI(typename(this), "row : %u", led.row);

                    //2번째 부터는 아래로
                    if(led.row > 1){
                        led.row -= 2;
                        packet.message = led.message;
                        bottomSend(packet);
                    }else{
                        rightSend(packet);                        
                    }
                }else{
                    send(packet);
                }
            }
        }
        /*
            col_ _
            |
        row |
        */
};

#endif