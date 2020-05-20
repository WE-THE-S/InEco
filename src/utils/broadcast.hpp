#ifndef __BROADCAST_HPP__
#define __BROADCAST_HPP__

#include "../struct/packet.hpp"
#include "../struct/broadcast_receiver.hpp"

#include <vector>
#include <algorithm>

using namespace std;
class Broadcast{
    private:
        static Broadcast* instance;
        vector<BroadcastReceiver> receiver;
        Broadcast(){
            
        }
    public:
        static Broadcast* getInstance(){
            if(instance == nullptr){
                instance = new Broadcast();
            }
            return instance;
        }
        
        void broadcast(const device_communication_message_t const message){
            for(auto& recv : receiver){
                recv.messageRecv(message);
            }
        }

        void add(const BroadcastReceiver& client){
            receiver.push_back(client);
        }
};

#endif