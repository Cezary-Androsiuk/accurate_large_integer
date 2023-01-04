#include "BPrint.hpp"

const std::string BPrint::binary_x8(unsigned char src){
    std::string result = "";
    for(int i=0; i<8; i++){
        result = (src & 0b1 ? '1' : '0') + result;
        src >>= 1;
    }
    return result;
}

const std::string BPrint::binary_x64(long long src, const std::string& sep){

    std::string result = "";
    for(int i=0; i<8; i++){
        std::string byte = BPrint::binary_x8(src & 0b11111111);
        src >>= 8;

        if(i) result = sep + result;

        #ifdef IMPROVED_PRINT
            if(byte == "00000000")      result = '0' + result;
            else if(byte == "11111111") result = '1' + result;
            else                        result = byte + result;
        #else
            result = byte + result;
        #endif
    }
    return result;

}

#ifdef IMPROVED_PRINT
    #undef IMPROVED_PRINT
#endif
