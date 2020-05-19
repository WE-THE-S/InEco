#ifndef __HANDLER_HPP__
#define __HANDLER_HPP__

#include <queue>
#include <cstdint>
#include "../struct/packet.hpp"

using namespace std;

class Handler {
    protected:
        queue<pair<MESSAGE_TYPE, base_message_t>> commandQueue;

    public:
        Handler(){
        }

        ~Handler(){
        }

        void clear(){
            queue<pair<MESSAGE_TYPE, base_message_t>> empty;
            swap(empty, commandQueue);
        }
        
        virtual void execute(){

        }
};

#endif