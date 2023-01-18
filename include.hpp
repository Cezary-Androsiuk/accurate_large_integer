#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <cstdio>
#include <string>
#include "BPrint/BPrint.hpp"
#include "time/time.hpp"


#include <iostream> // unit tests
#include <random> // unit tests



#define UNSIGNED_LONG_LONG_CELL

// masks can be used instead of remember and typing those numbers
// help with switching from unsigned char to unsigned long long 
#ifdef UNSIGNED_LONG_LONG_CELL
    #define CELL_TYPE unsigned long long
    #define BITS_PER_BYTE 8
    #define BYTES_PER_VAR 8
    #define BITS_PER_VAR 64
    #define ULL_VAR_SEP "_"

    #define mask000 (0b0000000000000000000000000000000000000000000000000000000000000000)
    #define mask001 (0b0000000000000000000000000000000000000000000000000000000000000001)
    #define mask010 (0b0111111111111111111111111111111111111111111111111111111111111110)
    #define mask011 (0b0111111111111111111111111111111111111111111111111111111111111111)
    #define mask100 (0b1000000000000000000000000000000000000000000000000000000000000000)
    #define mask101 (0b1000000000000000000000000000000000000000000000000000000000000001)
    #define mask110 (0b1111111111111111111111111111111111111111111111111111111111111110)
    #define mask111 (0b1111111111111111111111111111111111111111111111111111111111111111)
#else
    #define CELL_TYPE unsigned char 
    #define BITS_PER_BYTE 8
    #define BYTES_PER_VAR 1
    #define BITS_PER_VAR 8

    #define mask000 (0b00000000)
    #define mask001 (0b00000001)
    #define mask010 (0b01111110)
    #define mask011 (0b01111111)
    #define mask100 (0b10000000)
    #define mask101 (0b10000001)
    #define mask110 (0b11111110)
    #define mask111 (0b11111111)
#endif // UNSIGNED_LONG_LONG_CELL

#endif // INCLUDE_HPP