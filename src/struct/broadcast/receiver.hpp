#ifndef __BROADCAST_RECEIVER_HPP__
#define __BROADCAST_RECEIVER_HPP__

#include "../packet.hpp"

template <typename T>
class BroadcastReceiver {
    public:
    virtual void onMessage(const T message);
};

#endif