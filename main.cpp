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


    // for(int i=-1000; i<1001; i++){
    //     for(int j=-1000; j<1001; j++){
    //         x = i;
    //         y = j;

    //         z = x.multiplication(y);
    //         if(!z.equal(i*j)){
    //             x >> "d * ";
    //             y >> "d = ";
    //             z >> "d\n";
    //         }
    //     }
    //     // printf("\n");
    // }

    // x.file("rb.dvfiles/addition_input_1");
    // y.file("rb.dvfiles/addition_input_2");
    // z = x.multiplication(y);
    // x >> "d\n\n";
    // y >> "d\n\n";
    // z >> "d\n";
    


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