#ifndef __MESSAGE_BROADCAST_RECEIVER_HPP__
#define __MESSAGE_BROADCAST_RECEIVER_HPP__

#include "../packet.hpp"
#include "./broadcast_receiver.hpp"

//기기간 통신한 데이터를 뿌려주는 broadcast receiver
class MessageBroadcastReceiver : public BroadcastReceiver<device_communication_message_t> {
    
};

#endif