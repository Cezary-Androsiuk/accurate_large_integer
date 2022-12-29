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
    
    
#define NEW_PRINT false


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
#if NEW_PRINT == true
            if(byte == "00000000")
                bin = std::string("0") + sep + bin;
            else if (byte == "11111111")
                bin = std::string("1") + sep + bin;
            else
#endif
                bin = byte + sep + bin;

        }
        else{
#if NEW_PRINT == true
            if(byte == "00000000")
                bin = std::string("0");
            else if (byte == "11111111")
                bin = std::string("1");
            else
#endif
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


CELL_TYPE BCDincrement(CELL_TYPE cell){
    // result can't exceed the range of CELL_TYPE
    // and assume there won't be nibble(4bits) greater than 1001 to save one if statement
    CELL_TYPE result = 0;
    int i=0;
    while(cell != 0){
        CELL_TYPE nibble = cell & 0b00001111;
        cell >>= 4;
        if(nibble > 4) nibble += 3;
        for(int j=0; j<i; j++) nibble <<= 4;
        
        result |= nibble;
        i++;
    }
    return result;
}