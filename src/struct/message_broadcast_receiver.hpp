#ifndef __IBROADCASTRECIVER_HPP__
#define __IBROADCASTRECIVER_HPP__

#include "./packet.hpp"

class MessageBroadcastReceiver {
    public:
    virtual void messageRecv(const device_communication_message_t const message);
};

#endif