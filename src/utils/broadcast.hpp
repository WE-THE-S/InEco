#ifndef __BROADCAST_HPP__
#define __BROADCAST_HPP__

#include "../struct/packet.hpp"
#include "../struct/broadcast/message_broadcast_receiver.hpp"

#include <vector>
#include <algorithm>

using namespace std;

//내부 모듈간 통신을 위한 Broadcast 클래스
template <typename T>
//내부에서 통신을 위해서 사용하는 패킷은 여러 종류가 있으므로, 템플릿 적용
class Broadcast{
    private:
        //같은 패킷 타입을 사용하는 receiver들을 저장하는 vector 인스턴스
        vector<BroadcastReceiver<T>*> receiver;
    public:
        //인스턴스가 하나만 생성돼야해서 싱글톤 적용
        static Broadcast* getInstance() {
            static Broadcast instance;
            return &instance;
        }
        
        //모든 객체에 메세지를 뿌리는 함수
        void broadcast(const T message){
            for(auto& recv : receiver){
                recv->onMessage(message);
            }
        }

        //객체에 receiver 추가
        void add(BroadcastReceiver<T>* client){
            receiver.push_back(client);
        }
};

#endif