#include "include.h"
#include "AccurateLargeInteger/AccurateLargeInteger.h"

int main(){


    ALi x,y,z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');
    unsigned long long val;
    std::clock_t start1,start2,start3;
    std::clock_t stop1,stop2,stop3;
    // start1 = std::clock();
    // for(unsigned long long i=0; i<0xFFFFFFFFFFFFFFFF; i++){
    //     val=0x0F2F0FF5FFFaFF9Fu;
    //     val <<= 1;
    // }
    // stop1 = std::clock();

    // start2 = std::clock();
    // for(unsigned long long i=0; i<0xFFFFFFFFFFFFFFFF; i++){
    //     val=0x0F2F0FF5FFFaFF9Fu;
    //     val *= 2;
    // }
    // stop2 = std::clock();
    
    // start3 = std::clock();
    // for(unsigned long long i=0; i<0xFFFFFFFFFFFFFFFF; i++){
    //     val=0x0F2F0FF5FFFaFF9Fu;
    //     if(val == 0x0F2F0FF5FFFaFF9Fu)
    //         val = 2188221567420727102;
    // }
    // stop3 = std::clock();

    long long x1,y1,z1;
    double x2,y2,z2;
    x1 = 0x00FFFFFFFFFFFFFF;
    y1 = 0x01EFFFFFFFFFFFFF;
    x2 = 0x00FFFFFFFFFFFFFF;
    y2 = 0x01EFFFFFFFFFFFFF;
    x.assignment(x1);
    y.assignment(y1);

    start1 = std::clock();
    for(int i=-0xFFFE; i<0xFFFF; i++){
        // x1 = i;
        // x.assignment(i);
        for(int j=-0xFFFE; j<0xFFFF; j++){
            // y1 = j;
            // y.assignment(j);
            // x1 + y1;
            // z1 = x1 + y1;
            x2 + y2;
            // z2 = x2 + y2;
            // x.addition(y);
            // z.assignment(x.addition(y));
        }
        // printf("%d\n",i);
    }
    stop1 = std::clock();
    std::cout << "time : " << (stop1 - start1) / (double)(CLOCKS_PER_SEC/1000) << "ms\n";

    exit(0);



    // #define TEST_ASSIGN

    // #define TEST_EQUAL
    // #define TEST_GREATER
    // #define TEST_SMALLER

    // #define TEST_INC
    // #define TEST_ADD
    // #define TEST_ADDAS

    // #define TEST_DEC
    // #define TEST_SUB
    // #define TEST_SUBAS

    #define TEST_MUL
    // #define TEST_MULAS

    // #define TEST_UNIT


    #ifdef TEST_ASSIGN
        for(int i=-0xFFFE; i<0xFFFF; i++){
            x.assignment(i);
            x.print('b'," - ");
            x.print('d',"\n");
        }
    #endif

    //* bool equal
    #ifdef TEST_EQUAL
        // for(int i=-0xFFFE; i<0xFFFF; i++){
        //     x.assignment(i);
        //     for(int j=-0xFFFE; j<0xFFFF; j++){
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
        //     printf("%d\n",i);
        // }
        // printf("addition done\n");
    #endif

    //* bool greater
    #ifdef TEST_GREATER
        // for(int i=-0xFFFE; i<0xFFFF; i++){
        //     x.assignment(i);
        //     for(int j=-0xFFFE; j<0xFFFF; j++){
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
        //     printf("%d\n",i);
        // }
        // printf("addition done\n");
    #endif

    //* bool smaller
    #ifdef TEST_SMALLER
        // for(int i=-0xFFFE; i<0xFFFF; i++){
        //     x.assignment(i);
        //     for(int j=-0xFFFE; j<0xFFFF; j++){
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
        //     printf("%d\n",i);
        // }
        // printf("addition done\n");
    #endif

    //* increment
    #ifdef TEST_INC
        x.assignment(-0xFFFE);
        for(int i=-0xFFFE; i<0xFFFF; i++){
            if(!x.equal(i)){
                printf("%d = ",i);
                x.print('d'," ");
                x.print('b',"\n");
                exit(0);
            }
            x.increment();
        }
        printf("incrementation done\n");
    #endif

    //* addition
    #ifdef TEST_ADD
        start = std::clock();

        for(int i=-0xFE; i<0xFF; i++){
            x.assignment(i);
            for(int j=-0xFE; j<0xFF; j++){
                y.assignment(j);
                z.assignment(x.addition(y));
                if(!z.equal(j+i)){
                    printf("%d + %d = %d = ",i,j,i+j);
                    z.print('d',"\n");
                    x.print('b',"\n");
                    y.print('b',"\n");
                    z.print('b',"\n");
                    exit(0);
                }
            }
            // printf("%d\n",i);
        }
        stop = std::clock();
        // printf("addition done\n");
        std::cout << "time: " << (stop - start) / (double)(CLOCKS_PER_SEC/1000) << "ms\n";
    #endif


    #ifdef TEST_ADDAS

    #endif


    //* decrement
    #ifdef TEST_DEC
        x.assignment(0xFFFF);
        for(int i=0xFFFF; i>-0xFFFF; i--){
            if(!x.equal(i)){
                printf("%d = ",i);
                x.print('d'," ");
                x.print('b',"\n");
                exit(0);
            }
            x.decrement();
        }
        printf("decrementation done\n");
    #endif


    //* subtraction
    #ifdef TEST_SUB
        for(int i=-0xFFFE; i<0xFFFF; i++){
            x.assignment(i);
            for(int j=-0xFFFE; j<0xFFFF; j++){
                y.assignment(j);
                z.assignment(x.subtraction(y));
                if(!z.equal(i-j)){
                    printf("%d - %d = %d = ",i,j,i-j);
                    z.print('d',"\n");
                    x.print('b',"\n");
                    y.print('b',"\n");
                    z.print('b',"\n");
                    exit(0);
                }
            }
            printf("%d\n",i);
        }
        printf("subtraction done");
    #endif


    #ifdef TEST_SUBAS

    #endif
}