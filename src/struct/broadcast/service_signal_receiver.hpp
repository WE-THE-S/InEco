#ifndef __SERVICE_SIGNAL_RECEIVER_HPP__
#define __SERVICE_SIGNAL_RECEIVER_HPP__

#include "../packet.hpp"
#include "./receiver.hpp"

class ServiceSignalBroadcastReceiver : public BroadcastReceiver<device_communication_message_t> {
    
};

#endif