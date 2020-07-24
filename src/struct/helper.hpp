#ifndef __HELPER__HPP__
#define __HELPER__HPP__


#include <string>

//가끔 필요한 함수를 모아둔 helper 클래스
class Helper {
    private:
    char const hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    public:
    //bytes array를 hex string으로 변환해준다
    std::string bytesToHex(uint8_t *bytes, size_t size) const {
        std::string str;
        for (size_t i = 0; i < size; ++i) {
            const uint8_t ch = bytes[i];
            str.append(&hex[(ch & 0xF0) >> 4], 1);
            str.append(&hex[ch & 0xF], 1);
        }
        return str;
    }
};

#endif