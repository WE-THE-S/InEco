#ifndef __SERVICE_SIGNAL_RECEIVER_HPP__
#define __SERVICE_SIGNAL_RECEIVER_HPP__

#include "../packet.hpp"
#include "./receiver.hpp"

class ServiceSignalBroadcastReceiver : public BroadcastReceiver<service_signal_t> {

};

#endif