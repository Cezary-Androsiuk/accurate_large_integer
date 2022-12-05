#include "AccurateLargeInteger/AccurateLargeInteger.cpp"


const std::string toBin(const long long& source){
    // 8*8 bits +7 space signs
    std::string byte("00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000"); 
    unsigned long long mask = -1;
    mask >>= 1;
    mask = ~mask; // 1000...0000
    for(int i=0; i<8*8+7; i++){
        if(byte[i] != ' '){ // to keep space sign
            byte[i] = (source & (mask) ? '1':'0');
            mask >>= 1;
        }
    }
    return byte;
}

// #include <string>
const std::string toByte(const unsigned char& source){
    // 1*8 bits
    std::string byte("00000000"); 
    unsigned char mask = -1; // 11111111
    mask >>= 1; // 01111111
    mask = ~mask; // 10000000
    for(int i=0; i<1*8; i++){
        if(byte[i] != ' '){ // to keep space sign
            byte[i] = (source & (mask) ? '1':'0');
            mask >>= 1;
        }
    }
    return byte;
}