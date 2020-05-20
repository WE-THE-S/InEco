#ifndef __HANDLER_HPP__
#define __HANDLER_HPP__

#include <queue>
#include <cstdint>
#include "../struct/packet.hpp"
#include "../struct/broadcast_receiver.hpp"

using namespace std;

class Handler : public BroadcastReceiver{
    public:
        void messageRecv(const device_communication_message_t const message){
            ESP_LOGE(typename(this), "No configure message receiver");    
        }
};

#endif