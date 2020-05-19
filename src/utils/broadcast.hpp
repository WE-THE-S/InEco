#ifndef __BROADCAST_HPP__
#define __BROADCAST_HPP__

class Broadcast{
    private:
        static Broadcast* instance = nullptr;
        Broadcast(){
            
        }
    public:
        static Broadcast* getInstance(){
            if(instance == nullptr){
                instance = new Broadcast();
            }
            return instance;
        }
};

#endif