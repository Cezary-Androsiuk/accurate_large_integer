#include "include.h"
#include "AccurateLargeInteger/AccurateLargeInteger.h"

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



const bool test_equal(){
    ALi x,y;
    x.setSeparator(' ');
    y.setSeparator(' ');
    long long from = -0xFFFe;
    long long to   =  0xFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        for(long long j=from; j<to; j++){
            y.assignment(j);
            if(x.equal(y) != (i==j)){
                printf("\n%llu == %llu = %d = %d\n",i,j,i==j,x.equal(y));
                x.print('b',"\n");
                y.print('b',"\n\n");
                return false;
            }
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_greaterThan(){
    ALi x,y;
    x.setSeparator(' ');
    y.setSeparator(' ');
    long long from = -0xFFFe;
    long long to   =  0xFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        for(long long j=from; j<to; j++){
            y.assignment(j);
            if(x.greaterThan(y) != (i>j)){
                printf("\n%llu > %llu = %d = %d\n",i,j,i>j,x.greaterThan(y));
                x.print('b',"\n");
                y.print('b',"\n\n");
                return false;
            }
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_smallerThan(){
    ALi x,y;
    x.setSeparator(' ');
    y.setSeparator(' ');
    long long from = -0xFFFe;
    long long to   =  0xFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        for(long long j=from; j<to; j++){
            y.assignment(j);
            if(x.smallerThan(y) != (i<j)){
                printf("\n%llu < %llu = %d = %d\n",i,j,i<j,x.smallerThan(y));
                x.print('b',"\n");
                y.print('b',"\n\n");
                return false;
            }
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_increment(){
    ALi x;
    x.setSeparator(' ');
    long long from = -0xFFFFFFFe;
    long long to   =  0xFFFFFFFF;
    x.assignment(from);
    for(long long i=from; i<to; i++){
        if(!x.equal(i)){
            ALi(i).print('b',"\n");
            x.print('b',"\n");
            return false;
        }
        fflush(stdout);
        percent(from, i, to);
        x.increment();
    }
    return true;
}


const bool test_assignment(){
    ALi x;
    x.setSeparator(' ');
    long long from = -0xFFFFFFFe;
    long long to   =  0xFFFFFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        if(x.returnglobalHandle() != i){
            printf("%lld\n%s\n",i,toBin(i,ULL_VAR_SEP).c_str());
            x.print('b',"\n");
            return false;
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_assignment2(){
    ALi x,y;
    x.setSeparator(' ');
    y.setSeparator(' ');
    long long from = -0xFFFFFFFe;
    long long to   =  0xFFFFFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        y.assignment(x);
        if(!x.equal(y)){
            printf("%lld\n%s\n",i,toBin(i,ULL_VAR_SEP).c_str());
            x.print('b',"\n");
            return false;
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_print(){
    ALi x;
    x.setSeparator(' ');
    long long from = -0xFFe;
    long long to   =  0xFFF;
    for(long long i=from; i<to; i++){
        printf("%lld = %s = ",i, toBin(i,ULL_VAR_SEP).c_str());
        x.assignment(i);
        x.print('b',"\n");
    }
    return true;
}


const bool test_addition(){
    ALi x,y,z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');
    long long from = -0xFFFe;
    long long to   =  0xFFFF;

    for(long long i=from; i<to; i++){
        x.assignment(i);
        for(long long j=from; j<to; j++){
            y.assignment(j);
            z.assignment(x.addition(y));
            if(!z.equal(i+j)){
                printf("\n%lld + %lld = %lld = ",i,j,i+j);
                z.print('d',"\n");
                x.print('b',"\n");
                y.print('b',"\n");
                z.print('b',"\n\n");
                return false;
            }
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}


const bool test_additionAssign(){
    ALi x,y;//z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    // z.setSeparator(' ');
    long long from = 0;// -0xFFFe;
    long long to   =  0xFFFF;
    for(long long i=from; i<to; i++){
        for(long long j=from; j<to; j++){
            x.assignment(i);
            y.assignment(j);
            x.additionAssign(y);
            if(!x.equal(i+j)){
                printf("\n%lld + %lld = %lld = ",i,j,i+j);
                x.print('d',"\n");
                y.print('b',"\n");
                ALi(i).print('b',"\n");
                x.print('b',"\n");
                return false;
            }
        }
        fflush(stdout);
        percent(from, i, to);
    }
    return true;
}










int main(){
    std::clock_t start = std::clock();
    // // 236325ms
    // if(!test_equal()){
    //     printf("equal was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("equal ok!\n");


    // // 238068ms
    // if(!test_greaterThan()){
    //     printf("test_greaterThan was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("test_greaterThan ok!\n");


    // // 237135ms
    // if(!test_smallerThan()){
    //     printf("smallerThan was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("smallerThan ok!\n");


    // // 218291ms
    // if(!test_increment()){
    //     printf("increment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("increment ok!\n");


    // // 
    // if(!test_assignment()){
    //     printf("assignment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment ok!\n");


    // 
    // if(!test_assignment2()){
    //     printf("assignment2 was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment2 ok!\n");


    // // 
    // if(!test_print()){
    //     printf("assignment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment ok!\n");


    // // 1.88941e+06ms
    // if(!test_addition()){
    //     printf("addition was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition ok!\n");


    // 
    if(!test_additionAssign()){
        printf("additionAssign was't finished!\n");
        return 1;
    }
    else
        printf("additionAssign ok!\n");


    std::cout << "time : " << (std::clock() - start) / (double)(CLOCKS_PER_SEC/1000) << "ms\n";

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