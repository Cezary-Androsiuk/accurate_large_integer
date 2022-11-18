#include "include.h"
#include "AccurateLargeInteger/AccurateLargeInteger.h"

void printFromFile(const char* path){
    FILE* file = fopen(path, "rb");
    unsigned char tmp;
    while(fread(&tmp,1,1,file) != 0){
        printf("%s(%d) ",toByte(tmp).c_str(),tmp);
    }
    fclose(file);
    printf("\n");
}

const std::string toBin(const long long& source){
    // 8*8 bits +7 space signs
    std::string byte("00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000"); 
    unsigned long long mask = -1; // 1111...1111
    mask >>= 1; // 0111...1111
    mask = ~mask; // 1000...0000
    for(int i=0; i<8*8+7; i++){
        if(byte[i] != ' '){ // to keep space sign
            byte[i] = (source & (mask) ? '1':'0');
            mask >>= 1;
        }
    }
    return byte;
}


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
    // TODO max unsigned long long value and signed long long values compare to ALi
    // TODO test few bits larger values than max long long container 
    // todo this will proof that ALi is working and can be rebuilded 
    for(size_t i=0; i<257; i++){
        ALi v(i*(-1));
        v.print('b',": ");
        v.print('d',"\n");
    }

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