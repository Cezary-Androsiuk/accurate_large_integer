#pragma once

#include "Stacker/Stacker.h"

#include <iostream>
#include <cstdio>
#include <optional>
// #include <stdlib.h>
#include <string>

#define CELL_TYPE unsigned char // need to implement, to easier change to ull in future

#define BITS_PER_BYTE 8
#define BYTES_PER_VAR sizeof(Cell::var)
#define BITS_PER_VAR BYTES_PER_VAR * BITS_PER_BYTE

// // unsigned long long most significant bit
// #define ULL_MSB (~((-1ULL) >> 1))
// #define NEG_ULL_MSB ((-1ULL) >> 1)

// // unsigned char most significant bit
// #define UCH_MSB (1ULL << 7)
// #define NEG_UCH_MSB (~(1ULL << 7))



// unsigned long long most significant bit
#if false
#define MASK_00 (0b0000000000000000000000000000000000000000000000000000000000000000)
#define MASK_01 (0b0111111111111111111111111111111111111111111111111111111111111111)
#define MASK_10 (0b1000000000000000000000000000000000000000000000000000000000000000)
#define MASK_11 (0b1111111111111111111111111111111111111111111111111111111111111111)
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

const std::string toBin(const long long&);
const std::string toByte(const unsigned char&);
long long pow_long(const long long&, const unsigned char&);