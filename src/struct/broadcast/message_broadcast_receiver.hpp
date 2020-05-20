#ifndef __MESSAGE_BROADCAST_RECEIVER_HPP__
#define __MESSAGE_BROADCAST_RECEIVER_HPP__

#include "../packet.hpp"
#include "./receiver.hpp"

class MessageBroadcastReceiver : public BroadcastReceiver<device_communication_message_t> {
    
};

#endif