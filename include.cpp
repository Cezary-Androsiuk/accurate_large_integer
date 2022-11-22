#include "AccurateLargeInteger/AccurateLargeInteger.cpp"
// #include "Stacker/Stacker.cpp"


// return single bit of char as a int, depends from weight (1,2,4,8,16,32,64,128)
// printf("%d",bit(65,64));
// #define bit(src,weight) (src%256 & (weight) ? '1':'0')

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


long long pow_long(const long long& base, const unsigned char& power){
    // not the best but simple
    long long result = 1;
    for (size_t i = 0; i < power; i++)
        result *= base;
    return result;
}