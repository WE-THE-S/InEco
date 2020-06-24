#ifndef __HELPER__HPP__
#define __HELPER__HPP__


#include <string>

class Helper {
    protected:
    char const hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

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