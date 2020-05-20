#ifndef __BROADCAST_HPP__
#define __BROADCAST_HPP__

#include "../struct/packet.hpp"
#include "../struct/broadcast/message_broadcast_receiver.hpp"

#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class Broadcast{
    private:
        vector<MessageBroadcastReceiver*> receiver;
    public:
        static Broadcast* getInstance() {
            static Broadcast instance;
            return &instance;
        }
        
        void broadcast(const T message){
            for(auto& recv : receiver){
                recv->onMessage(message);
            }
        }

        void add(MessageBroadcastReceiver* client){
            receiver.push_back(client);
        }
};

#endif