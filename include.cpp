#include "AccurateLargeInteger/AccurateLargeInteger.cpp"


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