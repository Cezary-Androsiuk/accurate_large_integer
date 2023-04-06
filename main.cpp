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
    int xi, yi;

    // for(int i=-10; i<=10; i++){
    //     for(int j=-10; j<=10; j++){
    //         if(j != 0){
    //             z = ALi(i).modulo(ALi(j));
    //             if(!z.sign()) printf(" ");
    //             if((i>0 ? i : -i) == (j>0 ? j : -j) ||
    //             (j>0 ? j : -j) == 1 || i == 0) printf("  ");
    //             else z >> "d ";
    //         }
    //         else
    //             printf(" ## ");
    //     }
    //     printf("\n");
    // }

    // for(int i=0; i<1000; i++)
    //     printf("%d - %s\n",i,(ALi(i).isPrime() ? "true" : "false"));

        

    // x = -7;
    // y = -3;
    // z = x.division(y);
    // z >> "d\n";

    // -7 --2*3 = 

    // x.file("rb.dvfiles/128.bit");
    // x.assignmentString("d59915276166009814580099137019372690938324757901441206345557740088899790995780839944713502945016725739051048472084628544921343630140719254274111760036026524306065026899308731153011333661693598215134675642777519447624342143356345389393359931607215655489651407770117803040714202354949305170598044946132849604059");
    // x.assignmentString("d987643212341111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");


    // for(int i=0; i<101; i++){
    //     ALi x(i);
    //     x >> "d - ";
    //     x.squareRootAlternative(2) >> "d\n";
    // }

    // for(int i=0; i<100; i++){
    //     if(x.isPrime()) x >> "d\n";
    //     else printf("not %d\n",i);
    //     --x;
    // }
    x = 9876;
    y = 12345;

    x >> "d\n";
    y >> "d\n";
    z = x ^ y;
    z >> "d\n";

    return 0;
}