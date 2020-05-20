#ifndef __SIGNAL_RECEIVER_HPP__
#define __SIGNAL_RECEIVER_HPP__


#include "../packet.hpp"

class ProcessSignalBroadcastReceiver {
    public:
    virtual void onMessage(const device_communication_message_t message);
};


#endif