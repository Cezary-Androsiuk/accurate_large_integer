#include "include.h"
#include "AccurateLargeInteger/AccurateLargeInteger.h"

// void printFromFile(const char* path){
//     FILE* file = fopen(path, "rb");
//     unsigned char tmp;
//     while(fread(&tmp,1,1,file) != 0){
//         printf("%s(%d) ",toByte(tmp).c_str(),tmp);
//     }
//     fclose(file);
//     printf("\n");
// }

// const std::string toBin(const long long& source){
//     // 8*8 bits +7 space signs
//     std::string byte("00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000"); 
//     unsigned long long mask = -1; // 1111...1111
//     mask >>= 1; // 0111...1111
//     mask = ~mask; // 1000...0000
//     for(int i=0; i<8*8+7; i++){
//         if(byte[i] != ' '){ // to keep space sign
//             byte[i] = (source & (mask) ? '1':'0');
//             mask >>= 1;
//         }
//     }
//     return byte;
// }


int main(){

    // BinaryVariable x(1136);
    // x.printBinary("\n");
    // x.printFileR("output1.txt");
    // x.readFileR("output1.txt");
    // x.printBinary("\n");

    // printf("%s\n",toBin(-1145445431126454566).c_str());
    // ALi x(-1145445431126454566);
    // x.setSeparator(' ');
    // x.print('b',"\n");

        // ALi v(-129);
        // v.print('b',": ");
        // v.invert();
        // v.print('d',"\n");
    // TODO describe thouse methods which do not have optymization method
    // TODO max unsigned long long value and signed long long values compare to ALi
    // TODO test few bits larger values than max long long container 
    // todo this will proof that ALi is working and can be rebuilded 

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


// void ALi::printDecimal(const char* additionText) const{
//     // if it is negative invert and print '-' sign
//     if(!this->isPositive()){
//         printf("-");
//         ALi tmp(*this);
//         tmp.invert();
//         tmp.printDecimal(additionText);
//         return;
//     }

//     if(this->isEmpty()){
//         printf("0");
//         printf("%s",additionText);
//         return;
//     }

//     // number system value
//     #define dvm 10
//     ALi slider(*this);
//     Stacker<char> decimal(new char('!'));
//     unsigned long long sliderBits = 0;
//     while(!slider.isEmpty()){ // iterate through to compute each decimal digit
//         slider.optymize(); // to keep as short as possible
//         // if(slider.globalHandle.L == &slider.globalHandle && slider.globalHandle.var < dvm){ // skips last cycle
//         //     decimal.push(slider.globalHandle.R->var);
//         //     break;
//         // }
//         // printf("x");
//         // this if allows to skip last cycle
//         if(slider.globalHandle.L != &slider.globalHandle || slider.globalHandle.var >= dvm){ 
//             sliderBits = BITS_PER_BYTE * (slider.length) * sizeof(Cell::var);
//             slider.newCell(0); // as a calculation buffer
//             for(int i=0; i<sliderBits; i++){ // iterate through each SHL using PLSB
//                 if(slider.globalHandle.R->var >= dvm)
//                     slider.globalHandle.R->var -= dvm;
//                 slider.PLSB((slider.globalHandle.R->var >= dvm ? 1 : 0));
//             }
//         }
//         // last iteration with keeping rest as a decimal digit
//         // decimal.push(slider.globalHandle.R->var - (slider.globalHandle.R->var >= dvm ? dvm : 0));
//         printf("%d",slider.globalHandle.R->var - (slider.globalHandle.R->var >= dvm ? dvm : 0));
//         slider.PLSB((slider.globalHandle.R->var >= dvm ? 1 : 0));
//         slider.delCell(); // delete calculation buffer
//     }
//     // print out decimal, cause now we know first digit
//     for(int i=0; i<decimal.size(); i++){
//         printf("%d",*decimal.top());
//         decimal.pop();
//     }

//     printf("%s",additionText);
// }