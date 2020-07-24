#ifndef __BROADCAST_RECEIVER_HPP__
#define __BROADCAST_RECEIVER_HPP__

#include "../packet.hpp"
#include "../helper.hpp"

//기기 내에서 서비스간 통신을 위해 활용되는 broadcast 리시버
template <typename T>
class BroadcastReceiver  {
    public:
    //유연하게 대처가능하게 템플릿으로 구성
    virtual void onMessage(const T message);
};

#endif