#pragma once

#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <random>

#include "BPrint/BPrint.h"


// #define UC_CELL

// #ifndef UC_CELL
    #define ULL_CELL
// #endif

// need to implement, to easier change to ull in future
#ifdef UC_CELL
    #define CELL_TYPE unsigned char 
#endif
#ifdef ULL_CELL
    #define CELL_TYPE unsigned long long
#endif
// i know this is not the best pleace to store cell definition class
// but if it is here might be easier with switching unsigned char and unsigned long long
/**
 * @brief single cell containing:
 * var (CELL_TYPE) store byte of information
 *      soon unsigned char will be changed to unsigned long long to save space like:
 *      if one cell can contain 8 times more bits(64 instead 8) then less cell are needed. 
 *      Cause every single cell one contains two pointers reduce 8 cells to just one remove 14 to Cell* saving a lot of space 
 * L (Cell*) pointer to cell on the left side 
 * R (Cell*) pointer to cell on the right side
 */
class Cell{ // i like more classes than structs 
public:
    CELL_TYPE var;
    Cell* L;
    Cell* R;
    
    // seems like without this in overall this won't be taking that much space like with those methods
    Cell(){}
    Cell(CELL_TYPE variable, Cell* left, Cell* right){
        this->var = variable;
        this->L = left;
        this->R = right;
    }
    ~Cell(){}
};


#define BITS_PER_BYTE 8
#define BYTES_PER_VAR sizeof(CELL_TYPE)
#define BITS_PER_VAR BYTES_PER_VAR * BITS_PER_BYTE
#define ULL_VAR_SEP "_"

// masks can be used instead of typing those bits
// help with switching from unsigned char to unsigned long long 
#ifdef ULL_CELL
#define mask000 (0b0000000000000000000000000000000000000000000000000000000000000000)
#define mask001 (0b0000000000000000000000000000000000000000000000000000000000000001)
#define mask010 (0b0111111111111111111111111111111111111111111111111111111111111110)
#define mask011 (0b0111111111111111111111111111111111111111111111111111111111111111)
#define mask100 (0b1000000000000000000000000000000000000000000000000000000000000000)
#define mask101 (0b1000000000000000000000000000000000000000000000000000000000000001)
#define mask110 (0b1111111111111111111111111111111111111111111111111111111111111110)
#define mask111 (0b1111111111111111111111111111111111111111111111111111111111111111)
#else
#define mask000 (0b00000000)
#define mask001 (0b00000001)
#define mask010 (0b01111110)
#define mask011 (0b01111111)
#define mask100 (0b10000000)
#define mask101 (0b10000001)
#define mask110 (0b11111110)
#define mask111 (0b11111111)
#endif


CELL_TYPE BCDincrement(CELL_TYPE cell);