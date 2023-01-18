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


unsigned long long longRandom(){
    unsigned long long out = 0;
    for(int i=0; i<8; i++){
        out <<= 8;
        out |= (std::rand()%256);
    }
    return out;
}





int main(){

    ALi x;
    
    // x.print("b\n");
    

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    return 0;

    // std::clock_t start;
    // std::clock_t stop;
    // start = std::clock();
    // system("sleep 1");
    // stop = std::clock();
    // std::cout << "time : " << (stop - start) / (double)(CLOCKS_PER_SEC/1000) << "ms\n";

    // exit(0);
    // ALi x,y,z;
    // x.setSeparator(' ');
    // y.setSeparator(' ');
    // z.setSeparator(' ');

    // //* assignment
    // for(int i=-0xFFFE; i<0xFFFF; i++){
    //     x.assignment(i);
    //     x.print('b'," - ");
    //     x.print('d',"\n");
    // }

    // //* addition
    // start = std::clock();

    // for(int i=-0xFE; i<0xFF; i++){
    //     x.assignment(i);
    //     for(int j=-0xFE; j<0xFF; j++){
    //         y.assignment(j);
    //         z.assignment(x.addition(y));
    //         if(!z.equal(j+i)){
    //             printf("%d + %d = %d = ",i,j,i+j);
    //             z.print('d',"\n");
    //             x.print('b',"\n");
    //             y.print('b',"\n");
    //             z.print('b',"\n");
    //             exit(0);
    //         }
    //     }
    //     // printf("%d\n",i);
    // }
    // stop = std::clock();
    // // printf("addition done\n");
    // std::cout << "time: " << (stop - start) / (double)(CLOCKS_PER_SEC/1000) << "ms\n";

    // //* decrement
    // x.assignment(0xFFFF);
    // for(int i=0xFFFF; i>-0xFFFF; i--){
    //     if(!x.equal(i)){
    //         printf("%d = ",i);
    //         x.print('d'," ");
    //         x.print('b',"\n");
    //         exit(0);
    //     }
    //     x.decrement();
    // }
    // printf("decrementation done\n");


    // //* subtraction
    // for(int i=-0xFFFE; i<0xFFFF; i++){
    //     x.assignment(i);
    //     for(int j=-0xFFFE; j<0xFFFF; j++){
    //         y.assignment(j);
    //         z.assignment(x.subtraction(y));
    //         if(!z.equal(i-j)){
    //             printf("%d - %d = %d = ",i,j,i-j);
    //             z.print('d',"\n");
    //             x.print('b',"\n");
    //             y.print('b',"\n");
    //             z.print('b',"\n");
    //             exit(0);
    //         }
    //     }
    //     printf("%d\n",i);
    // }
    // printf("subtraction done");
}