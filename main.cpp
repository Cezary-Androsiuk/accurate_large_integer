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
    z.setSeparator(' ');

    // x = 4;
    // y = 6;
    // z = x.multiplication(y);
    // x >> "d * ";
    // y >> "d = ";
    // z >> "d\n";

    x << "b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111101";
    y << "b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000110";
    x << "b11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111101 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001";
    y << "b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000110 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
    x >> "b\n";
    y >> "b\n";
    z = x.addition(y); 
    z >> "b\n";
    // 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111101 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001 
    // 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000110 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 
    // 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000110 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000001 

    // for(x=0; x<11; x++){
    //     for(y=0; y<11; y++){
    //         z = x.multiplication(y);
    //         x >> "d * ";
    //         y >> "d = ";
    //         z >> "d\n";
    //     }
    //     printf("\n");
    // }


    // x = 9;
    // y = 8;
    // z = x.multiplication(y);
    // x >> "d  * ";
    // y >> "d  = ";
    // z >> "d\n";

    // x = -9;
    // y = 8;
    // z = x.multiplication(y);
    // x >> "d * ";
    // y >> "d  = ";
    // z >> "d\n";

    // x = 9;
    // y = -8;
    // z = x.multiplication(y);
    // x >> "d  * ";
    // y >> "d = ";
    // z >> "d\n";

    // x = -9;
    // y = -8;
    // z = x.multiplication(y);
    // x >> "d * ";
    // y >> "d = ";
    // z >> "d\n";
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;
}