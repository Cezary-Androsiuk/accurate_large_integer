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

ALi GenRandomTestValue(int additionallen){
    // unsigned long long 8*8
    // 8 times gen random
    ALi out;
    out.globalHandle->var = longRandom();
    for(int l=0; l<additionallen; l++)
        out.newCell(longRandom());
    out.newCell(0);
    return out;
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


const bool test_PLSB_PMSB(){
    ALi x,y,_x;
    x.setSeparator(' ');
    // x.globalHandle->var = mask111;
    x << "r.dvfiles/128.bit";
    x.print('b', "\n\n");
    // for(int i=0; i<70; i++){
        x.PMSB(0);
        x.print('b',"\n");
    // }
    return false;
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
    ALi x,y;
    x = 10;
    x.increment();
    x.print('d',"\n");
    x = 10;
    y = x++;
    x.print('d',"\n");
    y.print('d',"\n");
    return false;
    // x.setSeparator(' ');
    // long long from = -0xFFFFFFFe;
    // long long to   =  0xFFFFFFFF;
    // x = from;
    // for(long long i=from; i<to; i++){
    //     if(!x.equal(i)){
    //         ALi(i).print('b',"\n");
    //         x.print('b',"\n");
    //         return false;
    //     }
    //     fflush(stdout);
    //     percent(from, i, to);
    //     x.increment();
    // }
    // return true;
}


const bool test_increment2(){
    ALi x(".dvfiles/asrc.bit",'r');
    x.setSeparator(' ');
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    // x.print('d',"\n");
    return true;
}


const bool test_decrement(){
    ALi x(".dvfiles/asrc.bit",'r');
    x.setSeparator(' ');
    for(int i=0; i<400; i++){
        x.print('b',"\n");
        x.decrement();
    }
    // x.print('d',"\n");
    return true;
}


const bool test_decrement2(){
    ALi x(".dvfiles/asrc.bit",'r');
    x.setSeparator(' ');
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    x.print('b',"\n");
    x.increment();
    // x.print('d',"\n");
    return true;
}


const bool test_assignment(){
    ALi x;
    x.setSeparator(' ');
    long long from = -0xFFFFFFFe;
    long long to   =  0xFFFFFFFF;
    for(long long i=from; i<to; i++){
        x.assignment(i);
        // if(x.returnglobalHandle() != i){
        //     printf("%lld\n%s\n",i,toBin(i,ULL_VAR_SEP).c_str());
        //     x.print('b',"\n");
        //     return false;
        // }
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
            // printf("%lld\n%s\n",i,BPrint::binary_x64(i,ULL_VAR_SEP).c_str());
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
    x << "r.dvfiles/128.bit";
    x >> "d\n";
    return true;
    long long from = -0xFFe;
    long long to   =  0xFFF;
    for(long long i=from; i<to; i++){
        // printf("%lld = %s = ",i, BPrint::binary_x64(i,ULL_VAR_SEP).c_str());
        x.assignment(i);
        x.print('b',"\n");
    }
    return true;
}


const bool test_addition(){
    ALi x(".dvfiles/addition_input_1",'r'),
    y(".dvfiles/addition_input_2",'r'),
    z;
    // x.setSeparator(' ');
    // y.setSeparator(' ');
    // z.setSeparator(' ');

    // z = x + y;

    // x.print('d'," + ");
    // y.print('d'," = ");
    // z.print('d',"\n");

    // x.print('b',"\n");
    // y.print('b',"\n");
    // z.print('b',"\n");
    
    return false;





    // long long from = -0xFFFe;
    // long long to   =  0xFFFF;

    // for(long long i=from; i<to; i++){
    //     x = i;
    //     for(long long j=from; j<to; j++){
    //         y = j;
    //         z = x + y;
    //         if(z != i+j){
    //             printf("\n%lld + %lld = %lld = ",i,j,i+j);
    //             z.print('d',"\n");
    //             x.print('b',"\n");
    //             y.print('b',"\n");
    //             z.print('b',"\n\n");
    //             return false;
    //         }
    //     }
    //     fflush(stdout);
    //     percent(from, i, to);
    // }
    // return true;
}


const bool test_addition2(){
    ALi x,y,z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');
    long long from_pow_2 = 0;
    long long to_pow_2   = 80;

    for(long long i=from_pow_2; i<to_pow_2; i++){
        x.PLSB(std::rand()%2);
        y.clear();
        for(long long j=from_pow_2; j<to_pow_2; j++){
            y.PLSB(std::rand()%2);
            z.assignment(x.addition(y));
            if(!z.equal(i+j)||1){
                // printf("\ni:%lld j:%lld \n",i,j);
                // z.print('d',"\n");
                x.print('b',"\n");
                y.print('b',"\n");
                z.print('b',"\n");
                x.print('d'," ");
                y.print('d'," ");
                z.print('d',"\n\n");
                // return false;
            }
        }
        fflush(stdout);
        // percent(from_pow_2, i, to_pow_2);
    }
    return true;
}


const bool test_addition3(){
    // add only equal numbers
    ALi x(".dvfiles/asrc.bit",'r'),y(".dvfiles/asrc.bit",'r'),z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');

    for(int i=0; i<20; i++){
        x.print('b',"  x,y\n");
        z.assignment(x.addition(y));
        z.print('b',"  x+y\n");
        z.assignment(y.addition(x));
        z.print('b',"  y+x\n\n");
        x.increment();
        y.increment();
    }
    return true;
}


const bool test_addition4(){

    return true;
    ALi x(".dvfiles/asrc.bit",'r'),y(".dvfiles/bsrc.bit",'r'),z;
    x.setSeparator('\n');
    // y.invert();
    y.setSeparator('\n');
    z.setSeparator('\n');
    // x.assignment(10);
    // y.assignment(43);
    // z.assignment(x.addition(y));
    x.print('b',"= ");
    x.print('d',"\n");
    z.assignment(x.subtraction(y));
    y.print('b',"= ");
    y.print('d',"\n");
    z.print('b',"= ");
    z.print('d',"\n\n");
    // x.print('b',"\n");
    // y.print('b',"\n");
    // z.print('b',"\n\n");

// x - y
//                                                                                                                                                 00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000 - 
// 00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 = 
// 11111111_11111111_11111111_11111111_11111111_11111111_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 = 
// 11111111_11111111_11111111_11111111_11111111_11111110_11111111_11111111 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111 00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000
//
//                                         65536 -
//  22300745198530623141535718272648361505980416 =
// -22300745198530623141535736719392435215466496


// y - x
// 00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000
//                                                                                                                                                 00000000_00000000_00000000_00000000_00000000_00000001_00000000_00000000 
// 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111 11111111_11111111_11111111_11111111_11111111_11111111_00000000_00000000

// 00000000_00000000_00000000_00000000_00000000_00000000_11111111_11111111 11111111_11111111_11111111_11111111_11111111_11111111_11111111_11111111 11111111_11111111_11111111_11111111_11111111_11111111_00000000_00000000

// 223007451985306231415357182726483615059 80416 -
//                                         65536 =
// 223007451985306231415357182726483615059 14880

    // 65536
    //  22300745198530623141535718272648361505980416
    // -22300745198530623141535736719392435215466496

    // -22300745198530623141535718272648361505914880
    //  22300745198530623141535718272648361505914880
    return false;
}


const bool test_addition_subtraction(){
    /*
    target:
        x,y = random
        z1 = x+y
        z2 = y+x
        compare1:
            z1 == z2
        compare2:
            x == z1-y
            y == z1-x
        additional:
            z1 -= 2
            x--
            y--
            z2 = x+y
            compare3:
                z1 == z2 // if (x-1)+(y-1)==(x+y)-2 then with multiple combinations will be ok

    */
   return false;
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


const bool test_multiplication(){
    ALi x(-6),y(7),z;
    x.setSeparator(' ');
    y.setSeparator(' ');
    z.setSeparator(' ');
    // x << "r.dvfiles/128.bit";
    x.print('b',"\n");
    y.print('b',"\n\n");
    z = x.multiplication(y);
    z.print('d',"\n");

    return false;
}




int main(){


    // if(!test_equal()){ // 236325ms
    //     printf("equal was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("equal ok!\n");

    // if(!test_PLSB_PMSB()){ 
    //     printf("test_PLSB_PMSB was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("test_PLSB_PMSB ok!\n");

    // if(!test_greaterThan()){ // 238068ms
    //     printf("test_greaterThan was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("test_greaterThan ok!\n");

    // if(!test_smallerThan()){ // 237135ms
    //     printf("smallerThan was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("smallerThan ok!\n");

    // if(!test_increment()){ // 218291ms
    //     printf("increment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("increment ok!\n");

    // if(!test_increment2()){
    //     printf("increment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("increment ok!\n");

    // if(!test_decrement()){
    //     printf("decrement was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("decrement ok!\n");

    // if(!test_increment2()){
    //     printf("increment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("increment ok!\n");

    // if(!test_assignment()){
    //     printf("assignment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment ok!\n");

    // if(!test_assignment2()){
    //     printf("assignment2 was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment2 ok!\n");

    // if(!test_print()){
    //     printf("assignment was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("assignment ok!\n");

    // if(!test_addition()){
    //     printf("addition was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition ok!\n");

    // if(!test_addition2()){
    //     printf("addition2 was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition2 ok!\n");

    // if(!test_addition3()){
    //     printf("addition3 was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition3 ok!\n");

    // if(!test_addition4()){
    //     printf("addition4 was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition4 ok!\n");

    // if(!test_addition_subtraction()){
    //     printf("addition_subtraction was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("addition_subtraction ok!\n");

    // if(!test_additionAssign()){
    //     printf("additionAssign was't finished!\n");
    //     return 1;
    // }
    // else
    //     printf("additionAssign ok!\n");

    if(!test_multiplication()){
        printf("multiplication was't finished!\n");
        return 1;
    }
    else
        printf("multiplication ok!\n");



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