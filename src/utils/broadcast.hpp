#ifndef __BROADCAST_HPP__
#define __BROADCAST_HPP__

#include "../struct/packet.hpp"
#include "../struct/message_broadcast_receiver.hpp"

#include <vector>
#include <algorithm>

using namespace std;
class Broadcast{
    private:
        vector<MessageBroadcastReceiver*> receiver;
    public:
        static Broadcast* getInstance() {
            static Broadcast instance;
            return &instance;
        }
        
        void broadcast(const device_communication_message_t message){
            for(auto& recv : receiver){
                recv->messageRecv(message);
            }
        }

        void add(MessageBroadcastReceiver* client){
            receiver.push_back(client);
        }
};

#endif