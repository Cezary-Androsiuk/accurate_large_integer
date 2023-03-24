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


std::string generate_random_8bit(int seed){
    int mod_seed = seed%10;
    switch (mod_seed){
    case 0:
        return "00000000";
        break;
    case 1:
        return "11111111";
        break;
    case 2:
        return "11110000";
        break;
    case 3:
        return "00001111";
        break;
    case 4:
        return "11001100";
        break;
    case 5:
        return "00110011";
        break;
    case 6:
        return "01111111";
        break;
    case 7:
        return "10000000";
        break;
    case 8:
        return "01111110";
        break;
    case 9:
        return "10000001";
        break;
    
    default:
        return "00000000";
        break;
    }
}
std::string generate_random_value(int len_seed){
    std::string value = "b";
    int len = len_seed % 32;
    for(int i=0; i<len; i++){
        value += generate_random_8bit(rand());
    }
    return value;
}



int main(){
    srand(time(nullptr));

    // std::cout << generate_random_value(rand()) << std::endl;

    // return 0;

    ALi x, y, z, z1, z2;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');
    z1.setSeparator(' ');
    z2.setSeparator(' ');

    x = 9876;
    y = 1234;

    x >> "d\n";
    y >> "d\n";
    z = x ^ y;
    z >> "d\n";

    // x = 0b00101001;
    // y = 21;
    // y.invert();


//   00000000_00000000_00000000_00000000_00000000_00000000_00000000_00101001 
// + 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11101011
//   00000000_00000000_00000000_00000000_00000000_00000000_00000000_00010100
//   00000000_00000000_00000000_00000000_00000000_00000000_00000000_00111110

    return 0;

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

    //         // z = x.multiplication(y);
    //         x.multiplicationAssign(y);
    //         if(!x.equal(i*j)){
    //             x >> "d \n";
    //             y >> "d ";
    //             // z >> "d\n";
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
    
    // x = -10;
    // y = -12;

    for(int i=0; i<100'000; i++){
        x << generate_random_value(rand()).c_str();
        y << generate_random_value(rand()).c_str();

        z1 = x * y;

        z2 = x;
        z2 *= y;

        if(z1 != z2){
            x >> "d\n";
            y >> "d\n";
            z1 >> "d\n";
            z2 >> "d\n";

            x >> "b\n";
            y >> "b\n";
            z1 >> "b\n";
            z2 >> "b\n";
        }
    }

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