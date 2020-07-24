#ifndef __SERVICE_SIGNAL_RECEIVER_HPP__
#define __SERVICE_SIGNAL_RECEIVER_HPP__

#include "../packet.hpp"
#include "./broadcast_receiver.hpp"

//기기 내부의 서비스간 통신을 위해 사용하는 broadcast receiver
class ServiceSignalBroadcastReceiver : public BroadcastReceiver<service_signal_t> {

};

#endif