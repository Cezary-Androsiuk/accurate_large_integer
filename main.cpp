#include "include.h"
#include "AccurateLargeInteger/AccurateLargeInteger.h"


int main(){

    // TODO describe thouse methods which do not have optymization method
    // TODO max unsigned long long value and signed long long values compare to ALi

    // long long variable = 0x7fffffffffffffffLL;
    // long long variable = 0b0111111111111111111111111111111111111111111111111111111111111111;
    // // ALi v(variable);
    // printf("Max long long variable      (64bit)\t %lld\n",variable);
    // // v.print('d',"\n");
    // ALi v128("128.bit",'r');
    // printf("My Accurate Long Integer    (128bit)\t ");
    // v128.print('d',"\n");
    // ALi v256("256.bit",'r');
    // printf("My Accurate Long Integer    (256bit)\t ");
    // v256.print('d',"\n");
    // ALi v512("512.bit",'r');
    // printf("My Accurate Long Integer    (512bit)\t ");
    // v512.print('d',"\n");
    // ALi v1024("1024.bit",'r');
    // printf("My Accurate Long Integer    (1024bit)\t ");
    // v1024.print('d',"\n");
    // printf("...\n");
    // decltype(nullptr);

    // printf("%s\n",toBin(1).c_str());
    // printf("%s\n",toBin(1ull).c_str());

    // printf("%s\n",toBin(-1).c_str());
    // printf("%s\n",toBin(-1u).c_str());
    // printf("%s\n",toBin(-1ull).c_str());
    // printf("%s\n",toBin((-1ULL)).c_str());
    // printf("%s\n",toBin((-1LL)).c_str());
    // printf("-1LL  SHR %s\n\n",toBin((-1LL >> 1)).c_str());

    // printf("%s -1ULL \n",toBin((-1ULL)).c_str());
    // printf("%s -1ULL SHR \n",toBin((-1ULL >> 1)).c_str());
    // printf("%s -1ULL SHR NG \n",toBin(~(-1ULL >> 1)).c_str());
    // printf("%s -1ULL SHR NG \n",toBin((1U << 7)).c_str());
    // printf("%s -1ULL SHR NG \n",toBin((~(1ULL << 7))).c_str());

    // int x = 9;
    // int y = 7;
    
    // for(int i=-0xFFFE; i<0xFFFF; i++){
    //     for(int j=-0xFFFF; j<0xFFFF; j++){
    //         int z;
    //         z = i + j;
    //         if(x == y){
    //             printf("%d,%d,%d,%d,%d\n",i,j,z,y,x);
    //         }
    //     }
    // }
    // printf("done");
    /*
    for(int i=-0xFFFE; i<0xFFFF; i++){
        for(int j=-0xFFFF; j<0xFFFF; j++){
            ALi x(i), y(j),z;
            z.assignment(x.addition(y));
            if(!z.equal(j+i)){
                printf("%d + %d = %d = ",i,j,i+j);
                z.print('d',"\n");
            }
        }
        printf("%d\n",i);
    }
    printf("done");
    //*/

// -65267 + -1 = -65268 = -130804
// -65267 + 65523 = 256 = -65280

    // int intx = -65267, inty = -65523;
    // ALi x(intx), y(inty), z, w(intx+inty);
    // x.setSeparator(' ');
    // y.setSeparator(' ');
    // z.setSeparator(' ');
    // w.setSeparator(' ');
    // z.assignment(x.addition(y));
    // printf("%d + %d = %d = ",intx,inty,intx+inty);
    // z.print('d',"\n  ");
    // x.print('b',"\n+ ");
    // y.print('b',"\n= ");
    // w.print('b',"\n= ");
    // z.print('b',"\n");
    
    
    
    // x.print('b',"\n");
    // x.print('d',"\n");
    // y.print('d',"\n");
    // z.assignment(x.addition(y));
    // z.print('d',"\n");
    // z.assignment(y.addition(x));
    // z.print('b',"\n");
    // printf("%llu\n",x.MSCell());
    
    
    // printf("%c",-128);


    // ALi v(298);
    // v.print('b',"\n");
    // v.print('d',"\n");
    // ALi v(100000);
    // v.setSeparator(' ');
    // v.print('b',"\n");
    // v.print('d',"\n");
    // v.printApproximation('d',"\n");

    // for(int i=0; i<300; i++){
    //     ALi v(i);
    //     printf("%d : ",i);
    //     v.print('d',"\n");
    // }


    // Stacker stack;
    // stack.top();
    // stack.push('H');
    // printf("e stack fele: %d\n",stack.top());
    // printf("size: %lld\n",stack.size());

    // std::stack<char> stos;
    // printf("%d,%ld\n", stos.top(),stos.size());
    // stos.push('h');
    // printf("%c,%ld\n", stos.top(),stos.size());

    // for(size_t i=0; i<257; i++){
    //     ALi v(i*(-1));
    //     v.print('b',": ");
    //     v.print('d',"\n");
    // }

    // v.invert();
    // v.print('b',"\n");

    // x.setSeparator(' ');
    // x.print('b',"\n");
    // x.file("fileReadingTest1.dat",'w','r');
    // x.file("fileReadingTest2.dat",'w','b');
    // x.print('b',"\n");
    // x.file("fileReadingTest1.dat",'r','r');
    // x.print('b',"\n");
    // x.file("fileReadingTest2.dat",'r','b');
    // x.print('b',"\n");
    
    // signed long long a = -17;
    // unsigned long long x = static_cast<unsigned long long>(a);
    // printf("%s\n",toBin(a).c_str());

    // printf("%d\n",0b11111111 & 0b10000000);

    // x.printBinary("\n");
    // x.printBinary("\n");
    // printFromFile("output2.txt");
    // x.readFileR("output2.txt");

}