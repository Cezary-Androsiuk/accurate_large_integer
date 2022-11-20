#pragma once

#include "Stacker/Stacker.h"

#include <iostream>
#include <cstdio>
#include <optional>
// #include <fstream>
// #include <stdlib.h>
// #include <string>

#define BITS_PER_BYTE 8
#define BYTES_PER_VAR sizeof(Cell::var)
#define BITS_PER_VAR BYTES_PER_VAR * BITS_PER_BYTE

const std::string toByte(const unsigned char&);
long long pow_long(const long long&, const unsigned char&);