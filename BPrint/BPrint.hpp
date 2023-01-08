#ifndef BPrint_HPP
#define BPrint_HPP
#include <string>

// #define IMPROVED_PRINT

namespace BPrint{
    const std::string binary_x8(unsigned char src);

    const std::string binary_x64(long long src, const std::string& sep);
};
#endif