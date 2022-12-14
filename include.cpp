#include "AccurateLargeInteger/AccurateLargeInteger.cpp"

// const std::string toBin(long long src, const char& sep){
//     std::string bin;
//     for(int i=0; i<8; i++){
//         std::string byte = "00000000";
//         unsigned char mask = 0b10000000;
//         for(int j=0; j<8; j++){
//             if(src & mask)
//                 byte[j] = '1';
//             mask >>= 1;
//         }
//         src >>= 8;
//         if(sep != '\0')
//             bin = byte + sep + bin;
//         else 
//             bin = byte + bin;
//     }
//     return bin;
// }
const std::string toBin(long long src, const std::string& sep){
    std::string bin;
    for(int i=0; i<8; i++){
        std::string byte = "00000000";
        unsigned char mask = 0b10000000;
        for(int j=0; j<8; j++){
            if(src & mask)
                byte[j] = '1';
            mask >>= 1;
        }
        src >>= 8;
        if(i){
            // if(byte == "00000000")
            //     bin = std::string("0") + sep + bin;
            // else if (byte == "11111111")
            //     bin = std::string("1") + sep + bin;
            // else
                bin = byte + sep + bin;

        }
        else{
            // if(byte == "00000000")
            //     bin = std::string("0");
            // else if (byte == "11111111")
            //     bin = std::string("1");
            // else
                bin = byte;
        }
    }
    return bin;
}

// #include <string>
// const std::string toByte(const unsigned char& source){
//     // 1*8 bits
//     std::string byte("00000000"); 
//     unsigned char mask = -1; // 11111111
//     mask >>= 1; // 01111111
//     mask = ~mask; // 10000000
//     for(int i=0; i<1*8; i++){
//         if(byte[i] != ' '){ // to keep space sign
//             byte[i] = (source & (mask) ? '1':'0');
//             mask >>= 1;
//         }
//     }
//     return byte;
// }


CELL_TYPE add3tocell(CELL_TYPE cell){
    // result can't exceed the range of CELL_TYPE
    // 0b0000000000000000000000000000000000000000000000000000000000001111
    // 0b0000000000000000000000000000000000000000000000000000000011110000
    // 0b0000000000000000000000000000000000000000000000000000111100000000
    // 0b0000000000000000000000000000000000000000000000001111000000000000
    // 0b0000000000000000000000000000000000000000000011110000000000000000
    // 0b0000000000000000000000000000000000000000111100000000000000000000
    // 0b0000000000000000000000000000000000001111000000000000000000000000
    // 0b0000000000000000000000000000000011110000000000000000000000000000
    // 0b0000000000000000000000000000111100000000000000000000000000000000
    // 0b0000000000000000000000001111000000000000000000000000000000000000
    // 0b0000000000000000000011110000000000000000000000000000000000000000
    // 0b0000000000000000111100000000000000000000000000000000000000000000
    // 0b0000000000001111000000000000000000000000000000000000000000000000
    // 0b0000000011110000000000000000000000000000000000000000000000000000
    // 0b0000111100000000000000000000000000000000000000000000000000000000
    // 0b1111000000000000000000000000000000000000000000000000000000000000
    // 0b1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1:15
    int v;
    CELL_TYPE result = 0;
    for (int i=0; i<BITS_PER_VAR/4; i++){
        v = cell & 0b00001111;
        cell >>= 4;
        if(v > 4) v += 3;
        for(int j=0; j<15-i; j++) v <<= 4;
        result += v;
        // printf("%s\n",toBin(result,' '));
    }
    return result;
}