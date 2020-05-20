#ifndef __MESSAGE_BROADCAST_RECEIVER_HPP__
#define __MESSAGE_BROADCAST_RECEIVER_HPP__

#include "./packet.hpp"

class MessageBroadcastReceiver {
    public:
    virtual void onMessage(const device_communication_message_t message);
};

#endif