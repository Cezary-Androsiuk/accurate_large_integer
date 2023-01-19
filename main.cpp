#include "include.hpp"
#include "AccurateLargeInteger/AccurateLargeInteger.hpp"

int prev = -1;

void percent(unsigned long long from, unsigned long long current, unsigned long long to){
    unsigned long long range = to - from;
    unsigned long long currentinrange = current - from;
    int prcntxd = (currentinrange * 100) / range;
    if(prcntxd != prev){
        printf("  %d%%\b\b\b\b\b",prcntxd);
        // std::cout << "\b\b\b" << prcntxd << "0%";
        prev = prcntxd;
    }
}






int main(){

    ALi x, y, z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    x = 9;
    y = -8;
    y >> "b\n";
    y.SHL();
    y >> "b\n";

    // z = x.multiplication(y);
    // x >> "d * ";
    // y >> "d = ";
    // z >> "d\n";
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}