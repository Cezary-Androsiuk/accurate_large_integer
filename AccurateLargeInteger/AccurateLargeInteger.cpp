#include "AccurateLargeInteger.h"

//   fold all Ctrl + K,  Ctrl + 0

//   fold 1st Ctrl + K,  Ctrl + 1

// unfold all Ctrl + K,  Ctrl + J




/**
 * @brief Construct a new ALi object
 */
ALi::ALi(){
    this->length = 1;
    this->globalHandle = new Cell;
    this->globalHandle->L = this->globalHandle;
    this->globalHandle->R = this->globalHandle;
    this->globalHandle->var = mask000;
    this->separator = '\0'; // '/0' means no separator
}
/**
 * @brief Construct a new ALi object by using previously existing object
 * @param source object which will be source to build identical ALi
 */
ALi::ALi(const ALi& source) : ALi(){
    this->assignment(source);
}
/**
 * @brief Construct a new ALi object by using intiger value
 * @param source object which will be source to build identical ALi
 */
ALi::ALi(const signed long long& source) : ALi(){
    this->globalHandle->var = source;
}
/**
 * @brief Construct a new ALi object by using existing file with value
 * @param path file where variable should be stored
 * @param type type of source file: readable('r'), binary('b')
 */
ALi::ALi(const char* sourcePath, const char& type) : ALi(){
    this->readFile(sourcePath,type);
}
/**
 * @brief Destroy the ALi object
 * deletes all dynamically allocated cells
 */
ALi::~ALi(){
    this->clear();
    delete this->globalHandle;
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief create new cell from the left side
 * @param standardSource: unsigned long long value that will be used to optional assigned value to new cell
 */
void ALi::newCell(const CELL_TYPE &standardSource){
    Cell* handle = new Cell;
    handle->L = this->globalHandle;
    handle->R = this->globalHandle->R;
    this->globalHandle->R->L = handle;
    this->globalHandle->R = handle;

    handle->var = standardSource;
    this->length++;
}
/**
 * @brief remove cell from the left, not including this->globalHandle
 * @return 1: if there are more cells that can be deleted
 * @return 0: if there are no more cells that can be deleted
 */
const bool ALi::delCell(){
    if(this->globalHandle->R == this->globalHandle){
        // all cells that can be deleted, was already deleted
        // one cell left
        this->globalHandle->var = mask000;
        return false;
    }
    // n dynamically alocated cells where n >= 1
    Cell* handle = this->globalHandle->R;
    handle->R->L = this->globalHandle;
    this->globalHandle->R = handle->R;
    this->length--;
    delete handle;
    return true;
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief execute shift right operation
 */
void ALi::SHR(){
    // works just like an >> operator for signed values
    Cell* handle = this->globalHandle->R;
    bool buffer[2] = {0,this->sgn()};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        // hold right bit of a cell
        buffer[0] = handle->var & mask001; 
        // shr cell
        handle->var >>= 1; 
        // if left cell's LSB, from this cell was 1, then set handle cell MSB to 1
        if(buffer[1]) handle->var |= mask100;
        // else to 0
        else handle->var &= mask011;
        // hold handle cell's LSB to next loop iteration (if is last one this just will be dropped)
        buffer[1] = buffer[0]; 
        handle = handle->R;
    } while (handle != this->globalHandle->R);
}
/**
 * @brief execute shift left operation
 */
void ALi::SHL(){
    // works just like an << operator for signed values
    Cell* handle = this->globalHandle; 
    bool buffer[2] = {0,0};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        // hold left bit of a cell
        buffer[0] = handle->var & mask100;
        // shl cell
        handle->var <<= 1;
        // if right cell's MSB, from this cell was 1, then set handle cell LSB to 1
        if(buffer[1]) handle->var |= mask001;
        // else to 0
        else handle->var &= mask110;
        // hold handle cell's MSB to next loop iteration (if is last one this just will be dropped)
        buffer[1] = buffer[0];
        handle = handle->L;
    }while(handle != this->globalHandle);

    // where most left bit after SHL changed their value, then overflow has occurred
    if((this->globalHandle->R->var & mask100) != buffer[1]){
        if(buffer[1]) // check if value was positive or negative before SHL
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}
/**
 * @brief push the most significant bit
 * @param bit if 0, 0 will be pushed if 1, 1 will be pushed on lsb
 */
void ALi::PMSB(const bool& bit){
    // works just like an >> operator for signed values
    Cell* handle = this->globalHandle->R;
    bool buffer[2] = {0,bit};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        // hold right bit of a cell
        buffer[0] = handle->var & mask001; 
        // shr cell
        handle->var >>= 1; 
        // if left cell's LSB, from this cell was 1, then set handle cell MSB to 1
        if(buffer[1]) handle->var |= mask100;
        // else to 0
        else handle->var &= mask011;
        // hold handle cell's LSB to next loop iteration (if is last one this just will be dropped)
        buffer[1] = buffer[0]; 
        handle = handle->R;
    } while (handle != this->globalHandle->R);
}
/**
 * @brief push the least significant bit
 * @param bit if 0, 0 will be pushed if 1, 1 will be pushed on lsb
 */
void ALi::PLSB(const bool& bit){
    // works just like an << operator for signed values
    Cell* handle = this->globalHandle; 
    bool buffer[2] = {0,bit};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        // hold left bit of a cell
        buffer[0] = handle->var & mask100;
        // shl cell
        handle->var <<= 1;
        // if right cell's MSB, from this cell was 1, then set handle cell LSB to 1
        if(buffer[1]) handle->var |= mask001;
        // else to 0
        else handle->var &= mask110;
        // hold handle cell's MSB to next loop iteration (if is last one this just will be dropped)
        buffer[1] = buffer[0];
        handle = handle->L;
    }while(handle != this->globalHandle);

    // where most left bit after SHL changed their value, then overflow has occurred
    if((this->globalHandle->R->var & mask100) != buffer[1]){
        if(buffer[1]) // check if value was positive or negative before SHL
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief return sign bit
 * @return true sign bit is 1 => number is negative
 * @return false sign bit is 0 => number is positive
 */
const bool ALi::sgn() const{
    return (this->globalHandle->var & mask100 ? true : false);
}
/**
 * @brief 
 * @return true 
 * @return false 
 */
const bool ALi::is0() const{
    if(this->length == 1 && this->globalHandle->var == mask000) return true;
    else return false;
}
/**
 * @brief 
 * @return true 
 * @return false 
 */
const bool ALi::is1() const{
    if(this->length == 1 && this->globalHandle->var == mask001) return true;
    else return false;
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief remove all dynamic allocated cells 
 */
void ALi::clear(){
    while(this->delCell());
    this->globalHandle->var = mask000;
}
/**
 * @brief removes cells from begin which are not include any new information like 00000000 or 11111111
 */
void ALi::optymize(){
    // should remove left cells untill paterns like this represented as a regular expesions exists:
    // ^[1111111111111111111111111111111111111111111111111111111111111111] [1.*]
    // ^[0000000000000000000000000000000000000000000000000000000000000000] [0.*]
    // and variable has more than one cell

    // -- 1 -- // 
    // const Cell* const handle = this->globalHandle->R;
    // if(handle != this->globalHandle){
    //     if((handle->var == mask111 && handle->R->var & mask100) || (handle->var == mask000 && ~handle->R->var & mask100)){
    //         this->delCell();
    //         this->optymize();
    //     }
    // }

    // -- 2 -- //
    // const Cell* const handle = this->globalHandle->R;
    // if((handle != this->globalHandle && handle->var == mask111 && handle->R->var & mask100) || 
    // (handle != this->globalHandle && handle->var == mask000 && ~handle->R->var & mask100)){
    //         this->delCell();
    //         this->optymize();
    // }

    // -- 3 -- //
    const Cell* const handle = this->globalHandle->R;
    if(handle != this->globalHandle && 
    ((handle->var == mask111 && handle->R->var & mask100) || (handle->var == mask000 && ~handle->R->var & mask100))){
            this->delCell();
            this->optymize();
    }

    // -- 4 -- //
    // if(this->globalHandle->R != this->globalHandle && 
    // ((this->globalHandle->R->var == mask111 && this->globalHandle->R->R->var & mask100) || 
    // (this->globalHandle->R->var == mask000 && ~this->globalHandle->R->R->var & mask100))){
    //         this->delCell();
    //         this->optymize();
    // }

    // -- 5 -- //
    // if(this->sign()){
    //     while(this->globalHandle->R->var == mask111 && 
    //     this->globalHandle->R->R->var & mask100 &&
    //     this->globalHandle->R != this->globalHandle)
    //         this->delCell();
    // }
    // else{ // is positive
    //     while(this->globalHandle->R->var == mask000 && 
    //     ~this->globalHandle->R->R->var & mask100 &&
    //     this->globalHandle->R != this->globalHandle)
    //         this->delCell();
    // }
}
/**
 * @brief change every single bit to oposite value
 */
void ALi::negate(){
    Cell* handle = this->globalHandle;
    do{
        handle->var = ~handle->var;
        handle = handle->L;
    }while(handle != this->globalHandle);
}
/**
 * @brief inverting value from (3) to (-3) and from (-10) to (10) ...
 */
void ALi::invert(){
    this->negate();
    this->increment();
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief print each byte of variable seperated by separator sign and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printBinary() const{
    const Cell* handle = this->globalHandle->R;
    do{
        printf("%s%c",toBin(handle->var, ULL_VAR_SEP).c_str(),this->separator);
        handle = handle->R;
    }while(handle != this->globalHandle->R);
    printf("\b "); // space is not the best choice, cause despite it is invisible, it is not \0
}
/**
 * @brief print digit of variable seperated by separator sign every 3 digits and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printDecimal() const{
    // if it is negative invert and print '-' sign
    if(this->sgn()){
        printf("-");
        ALi tmp(*this);
        tmp.invert();
        tmp.printDecimal();
        return;
    }
    else if(this->is0()){
        printf("0");
        return;
    }

    // ALi copy(*this);
    // copy.setSeparator(' ');
    // copy.print('b',"\n");
    // for(unsigned long long ishr=0; ishr<BITS_PER_VAR*this->length; ishr++){
    //     Cell* handle = copy.globalHandle;
    //     for (unsigned long long i=0; i<copy.length-this->length; i++){
    //         handle = handle->R;
    //         printf("%s\n",toBin(handle->var,' ').c_str());
    //         // handle->var = add3tocell(handle->var);

    //         int v;
    //         CELL_TYPE result = 0;
    //         for (int i=0; i<BITS_PER_VAR/4; i++){
    //             v = handle->var & 0b00001111;
    //             handle->var >>= 4;
    //             if(v > 4) v += 3;
    //             for(int j=0; j<15-i; j++) v <<= 4;
    //             result += v;
    //         }
    //         handle->var = result;
    //         printf(" %s\n",toBin(handle->var,' ').c_str());
    //     }
    //     copy.SHL();
    // }

    // Cell* handle = copy.globalHandle;
    // for (unsigned long long i=0; i<copy.length-this->length; i++){
    //     handle = handle->R;
    //     for (int j=0; j<BITS_PER_VAR/4; j++){
    //         int v = handle->var;
    //         for(int h=0; h<15-j; h++) v >>= 4;
    //         // printf("%d",v)
    //     }
    // }


    // exists better method called double dabble

    #define divby 10

    ALi slider(*this);
    Stacker<CELL_TYPE> decimal(new CELL_TYPE(1));
    unsigned long long sliderBitsCount = 0;
    while(slider.globalHandle->var != mask000 || slider.length != 1){ // iterate through each decimal digit
    slider.delCell();
    //     slider.optymize(); // to keep as short as possible
    //     if(slider.globalHandle->L != slider.globalHandle || slider.globalHandle->var >= divby){
    //         sliderBitsCount = BITS_PER_VAR * (slider.length);
    //         slider.newCell(mask000); // as a buffer
    //         for(int i=0; i<sliderBitsCount; i++){ // iterate through each SHL using PLSB
    //             if(slider.globalHandle->R->var >= divby){
    //                 slider.globalHandle->R->var -= divby;
    //                 slider.PLSB(mask001);
    //             }
    //             else slider.PLSB(mask000);
    //         }
    //     }
    //     if(slider.globalHandle->R->var >= divby){
    //         decimal.push(slider.globalHandle->R->var - divby);
    //         slider.PLSB(mask001);
    //     } 
    //     else{
    //         decimal.push(slider.globalHandle->R->var);
    //         slider.PLSB(mask000);
    //     }
    //     slider.delCell();
    }

    // while(!decimal.isEmpty()){
    //     printf("%llu",*decimal.top());
    //     decimal.pop();
    // }
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief printing binary variable in scientific notation format
 * @param appPrec simply how many digits (counting from left and without variable sign) will be printed
 */
void ALi::printBinaryApproximation(unsigned long long appPrec) const{
    // appPrec is not 0
    // print variable in scientific notation format
    // precision is in default 2 result will be xx * 
    // 
    printf("printBinaryApproximation is not finished yet\n");
}
/**
 * @brief printing decimal variable in scientific notation format
 * @param appPrec simply how many digits (counting from left and without variable sign) will be printed
 */
void ALi::printDecimalApproximation(unsigned long long appPrec) const{
    // appPrec is not 0
    // print variable in scientific notation format
    // precision is in default 2 result will be xx * 
    // 
    printf("printDecimalApproximation is not finished yet\n");
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief save variable to file in binary form
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFileBinary(const char* path) const{
    FILE* file = fopen(path,"wb");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    const Cell* handle = this->globalHandle;
    do{
        fwrite(&handle->var, BYTES_PER_VAR, 1, file); // bufer, size of cell, cell amout, source
        handle = handle->L;
    }while(handle != this->globalHandle);
    fclose(file);
}
/**
 * @brief save variable to file in readable form allowing to add separator sign every 8 bits 
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFileReadable(const char* path) const{
    FILE* file = fopen(path,"w");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    Cell* handle = this->globalHandle->R;
    do{
        std::string binary = toBin(handle->var,ULL_VAR_SEP);
        fwrite(binary.c_str(), sizeof(char), binary.length(), file); // bufer, size of cell, cell amout, source
        handle = handle->R;
    }while(handle != this->globalHandle->R);
    fclose(file);
}
/**
 * @brief save variable to file 
 * @param path path to file where variable should be stored
 * @param type type of file, readable('r') or binary('b')
 * @param append append or overwrite file if exist 
 */
void ALi::writeFile(const char* path, const char& type) const{
    switch (type){
        case 'b': this->writeFileBinary(path); break;
        case 'r': this->writeFileReadable(path); break;
        default: printf("unknown type!\nnot attempted to create file\n"); return;
    }
}
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
    // #
/**
 * @brief copy value from source file to variable
 * @param path source where an readable binary file is stored
 */
void ALi::readFileBinary(const char* path){
    this->clear();
    FILE* file = fopen(path,"rb");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    fread(&this->globalHandle->var, BYTES_PER_VAR, 1, file);
    char buffer;
    while(fread(&buffer, BYTES_PER_VAR, 1, file) != 0){
        this->newCell(buffer);
    }
    this->optymize();
    fclose(file);
}
/**
 * @brief copy value from readable source file to variable and ignore others than '1' and '0' signs
 * @param path source where an readable binary file is stored
 */
void ALi::readFileReadable(const char* path){
    // not need to be idiot proof
    printf("readFileReadable was not ended\n");
    return;


    // what if number of bits in file is not divided by 8
    // good way to sovle this will be 
    // or just leave this like it is, cause user can not use my extention
    // and it do not must be idiot proof
    // 1 execute few operations SHR
    // 2 comunicate user that data is not correctly fomrated and result will be different (larger)
    // 3 as a user use separators every 8 or even 4 bits


    FILE* file = fopen(path,"r");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    int i=7;
    char buffer;
    CELL_TYPE var = 0;
    ALi tmp;
    while(fread(&buffer, 1, 1, file) != 0){
        if(buffer == '1' || buffer == '0'){
            if(buffer == '1')
                var |= mask001 << i;
            if(i == 0){
                tmp.newCell(var);
                i = 8;
                var = 0;
            }
            i--;
        }
    }
    // from [6][5][4][3][2][1](0)
    // to      [1][2][3][4][5](6)
    this->clear();
    this->globalHandle->var = tmp.globalHandle->R->var; // add 6 to globalhandle
    Cell* tmphandle = tmp.globalHandle->R->R;
    while(tmphandle != tmp.globalHandle){ // iterate from 5 to 1 (including 1)
        this->newCell(tmphandle->var);
        tmphandle = tmphandle->R;
    }
    this->optymize();
    fclose(file);
}
/**
 * @brief copy value from file to variable
 * @param path source where an readable binary file is stored
 * @param type type of file, readable('r') or binary('b')
 */
void ALi::readFile(const char* path, const char& type){
    switch (type){
        case 'b': this->readFileBinary(path); break;
        case 'r': this->readFileReadable(path); break;
        default: printf("unknown type!\nnot attempted to read file\n"); return;
    }
}
    // #
    // #
    // #
    // #
    // #
    // # Assignment
    // #
    // #
    // #
    // #
    // #
/**
 * @brief assigns existed ALi to variable
 * @param source ALi what will be assigned to variable
 */
void ALi::assignment(const ALi& source){
    // it will be nice to just drop this variable globalhandle and focus on assigning 
    // clearig can be leaved for other cpu process
    // const int *  -  (pointer to const int)
    // int const *  -  (pointer to const int)
    // int * const  -  (const pointer to int)
    const Cell* srchandle = source.globalHandle->L;
    this->clear();
    this->globalHandle->var = source.globalHandle->var;
    while (srchandle != source.globalHandle){
        this->newCell(srchandle->var);
        srchandle = srchandle->L;
    }
    this->optymize();
}
/**
 * @brief assigns source int to variable
 * @param source int what will be assigned to variable
 */
void ALi::assignment(const signed long long& source){
    this->clear();
    #ifdef UC_CELL
        unsigned long long sample = source;
        this->globalHandle->var = sample % 256;
        sample /= 256;
        while(sample > 0){
            this->newCell(sample % 256);
            sample /= 256;
        }
        // for(int i=0; i<7; i++){
        //     this->newCell(sample % 256);
        //     sample /= 256;
        // }
        this->optymize();
    #endif
    #ifdef ULL_CELL
        this->globalHandle->var = source;
    #endif
}
CELL_TYPE ALi::returnglobalHandle() const{
    return this->globalHandle->var;
}
    // #
    // #
    // #
    // #
    // #
    // # Bolean
    // #
    // #
    // #
    // #
    // #
/**
 * @brief check if left object is equal to right object
 * @param right object
 * @return true left equal right
 * @return false left not equal right
 */
const bool ALi::equal(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    if(this->sgn() != right.sgn()) return false;
    if(this->length != right.length) return false;
    const Cell* hL = this->globalHandle;
    const Cell* hR = right.globalHandle;
    do{
        if(hL->var != hR->var) return false;
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->globalHandle);
    return true;
}
/**
 * @brief check if left object is greater than right object
 * @param right object
 * @return true left is greater
 * @return false left is not grater
 */
const bool ALi::greaterThan(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    /*
    test if  L > R
        sign gives 1 if is negative
    return + == return 0
    return - == return 1

    if(Lsgn != Rsgn){
        L+ > R- return true
        L- > R+ return false
        return true if L is positive, false if is not

        L+ > R- return -
        L- > R+ return +
        return Rsgn
    }
    else if(Llen > Rlen){
        left is greater if left is longer?
        case where both are positive
            answer is true
        case where both are negative
            answer is false

        return true if L is positive, false if is not
        return !Lsgn
    }
    else if(Llen < Rlen){
        left is greater if left is shorter?
        case where both are positive
            answer is false
        case where both are negative
            answer is true

        return false if L is positive, true if is not
        return Lsgn
    }
    */
    const bool Lsgn = this->sgn();
    const bool Rsgn = right.sgn();
    if(Lsgn != Rsgn) return Rsgn; 
    // signs are equal
    else if(this->length > right.length) return !Lsgn;
    else if(this->length < right.length) return Lsgn;
    // lengths are equal
    Cell* hL = this->globalHandle;
    Cell* hR = right.globalHandle;
    do{
        if(hL->var > hR->var) return true; // left is greater
        if(hL->var < hR->var) return false; // left is smaller
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->globalHandle); // compare each cell, begin from the most significant
    return false; // both are equal => left is NOT greater
}
/**
 * @brief check if left object is smaller than right object
 * @param right object
 * @return true left is smaller
 * @return false left is not smaller
 */
const bool ALi::smallerThan(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    /*
    test if  L < R
        sign gives 1 if is negative
    return + == return 0
    return - == return 1

    if(Lsgn != Rsgn){
        L+ < R- return false
        L- < R+ return true
        return false if L is positive, true if is not

        L+ < R- return +
        L- < R+ return -
        return Lsgn
    }
    else if(Llen > Rlen){
        left is smaller if left is longer?
        case where both are positive
            answer is false
        case where both are negative
            answer is true

        return false if L is positive, true if is not
        return Lsgn
    }
    else if(Llen < Rlen){
        left is smaller if left is shorter?
        case where both are positive
            answer is true
        case where both are negative
            answer is false

        return true if L is positive, false if is not
        return !Lsgn
    }
    */
    const bool Lsgn = this->sgn();
    const bool Rsgn = right.sgn();
    if(Lsgn != Rsgn) return Lsgn;
    // signs are equal
    else if(this->length > right.length) return Lsgn;
    else if(this->length < right.length) return !Lsgn;
    // lengths are equal
    Cell* hL = this->globalHandle;
    Cell* hR = right.globalHandle;
    do{
        if(hL->var > hR->var) return false; // left is greater
        if(hL->var < hR->var) return true; // left is smaller
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->globalHandle); // compare each cell, begin from the most significant
    return false; // both are equal => left is NOT smaller
}
    // #
    // #
    // #
    // #
    // #
    // # Addition
    // #
    // #
    // #
    // #
    // #
/**
 * @brief increment ALi by one
 * 
 */
void ALi::increment(){
    Cell *handle = this->globalHandle;
    while(handle->var == mask111 && handle != this->globalHandle->R){
        handle->var = mask000; // 11111111 -> (1)00000000
        handle = handle->L;
    }
    // protect from overflow, if we adding two positive values (*this + 1)
    if(!this->sgn() && (handle->var == mask011 && handle == this->globalHandle->R)){
        this->newCell(mask000);
    }
    handle->var++;
    this->optymize();
}
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::addition(const ALi& right){
    // printf("in: %llu\nin: %llu\n\n",this->globalHandle->var, right.globalHandle->var);
    if(right.is0()) return *this; // x+0 = x
    else if (this->is0()) return right; // 0+x = x
    else if (right.is1()){
        ALi tmp(*this);
        tmp.increment();
        return tmp;
    }
    else if (this->is1()){
        ALi tmp(right);
        tmp.increment();
        return tmp;
    }

    ALi result;
    ALi const* lobj = this; 
    ALi const* robj = &right;
    CELL_TYPE carry = 0;
    // case pull out from while cause idk how to implement while which starts from handle and ends on handle iterating through all 
    result.globalHandle->var = lobj->globalHandle->var + robj->globalHandle->var; 
    
    // keep carry to next iteration
    if(result.globalHandle->var < robj->globalHandle->var && result.globalHandle->var < lobj->globalHandle->var)
        carry = 1;
    else 
        carry = 0;

    const bool lsign = lobj->sgn();
    const bool rsign = robj->sgn();
    const CELL_TYPE lmask = (lsign ? mask111 : mask000);
    const CELL_TYPE rmask = (rsign ? mask111 : mask000);
    const Cell* const lgh = lobj->globalHandle;
    const Cell* const rgh = robj->globalHandle;
    const Cell* lh = lgh->L;
    const Cell* rh = rgh->L;

    while(lh != lgh || rh != rgh){
        // addition both cells and carry
        result.newCell((lh == lgh ? lmask : lh->var) + (rh == rgh ? rmask : rh->var) + carry);
        
        // keep carry to next iteration
        if((result.globalHandle->R->var < rh->var && result.globalHandle->R->var < lh->var) ||
        (result.globalHandle->R->var <= rh->var && result.globalHandle->R->var <= lh->var && carry)) 
            carry = 1;
        else 
            carry = 0;

        // change handles to next cell if not reached end variable yet
        if(lh != lgh) lh = lh->L;
        if(rh != rgh) rh = rh->L;
    }
    // overflow can only appear when both operation argument sign values are equal
    if(lsign == rsign && lsign != result.sgn())
        result.newCell(lmask);
    // result.optymize();
    // printf("end: %llu\nend: %llu\nend: %llu\n\n",this->globalHandle->var, right.globalHandle->var, result.globalHandle->var);

    return result;
}
/**
 * @brief 
 * @param right 
 */
void ALi::additionAssign(const ALi& right){
    this->assignment(this->addition(right));
    // if(right.isEmpty()) return; // both are 0 or right is 0
    // if(this->isEmpty()){ // left is 0
    //     this->assignment(right);
    //     return;
    // }
    
    // ALi* lobj = this; 
    // ALi const* robj = &right;

    // CELL_TYPE carry = 0;
    // // case pull out from while cause 
    // // idk how to implement while which starts from handle and ends on handle iterating through all 
    // CELL_TYPE lobj_cell_tmp = lobj->globalHandle->var;
    // lobj->globalHandle->var += robj->globalHandle->var; 
    // // keep carry to next iteration
    // if(lobj->globalHandle->var < robj->globalHandle->var && lobj->globalHandle->var < lobj_cell_tmp)
    //     carry = 1;
    // else 
    //     carry = 0;

    // const bool lsign = lobj->isPositive();
    // const bool rsign = robj->isPositive();
    // Cell* lHandle = lobj->globalHandle->L;
    // Cell const* rHandle = robj->globalHandle->L;

    // do{
    //     // addition both cells and carry
    //     if(lHandle == lobj->globalHandle)
    //         lobj->newCell((lsign ? mask000 : mask111) + rHandle->var + carry);
    //     else
            


    //     result.newCell((lHandle == lobj->globalHandle ? (lsign ? mask000 : mask111) : lHandle->var) + 
    //     (rHandle == robj->globalHandle ? (rsign ? mask000 : mask111) : rHandle->var) + carry);
        
    //     // keep carry to next iteration
    //     if((result.globalHandle->R->var < rHandle->var && result.globalHandle->R->var < lHandle->var) ||
    //     (result.globalHandle->R->var <= rHandle->var && result.globalHandle->R->var <= lHandle->var && carry == 1)) 
    //         carry = 1;
    //     else 
    //         carry = 0;

    //     // change handles to next cell if not reached end variable yet
    //     if(lHandle != lobj->globalHandle) lHandle = lHandle->L;
    //     if(rHandle != robj->globalHandle) rHandle = rHandle->L;
    // }while(lHandle != lobj->globalHandle || rHandle != robj->globalHandle);

    // // overflow can only appear when both operation argument sign values are equal
    // if(lsign == rsign && lsign != result.isPositive()){
    //     // if lsign(and rsign) is positive => result is also positive
    //     if(lsign) result.newCell(mask000);
    //     // if lsign(and rsign) is negative => result is also negative
    //     else result.newCell(mask111);
    // }
    // result.optymize();

    // return result;
}
    // #
    // #
    // #
    // #
    // #
    // # Subtraction
    // #
    // #
    // #
    // #
    // #
/**
 * @brief increment ALi by one
 * 
 */
void ALi::decrement(){
    Cell *handle = this->globalHandle;
    while(handle->var == mask000 && handle != this->globalHandle->R){
        handle->var = mask111; // (1)00000000 -> (0)11111111
        handle = handle->L;
    }
    // protect from overflow, cause we adding two positive values (*this + 1)
    if(this->sgn() && (handle->var == mask100 && handle == this->globalHandle->R)){
        this->newCell(mask111);
    }
    handle->var--;
    this->optymize();
}
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::subtraction(const ALi& right){
    ALi result;
    if(right.is0()) return *this; // both are 0 or right is 0
    else if(this->is0()){ // left is 0
        result.assignment(right);
        result.invert();
        return result;
    }
    
    ALi const* lobj = this; 
    ALi const* robj = &right;

    CELL_TYPE carry = 1;
    // case pull out from while cause idk how to implement while which starts from handle and ends on handle iterating through all 
    result.globalHandle->var = lobj->globalHandle->var + ~robj->globalHandle->var + carry; 
    // keep carry to next iteration
    if(result.globalHandle->var <= lobj->globalHandle->var && 
    result.globalHandle->var <= (CELL_TYPE)(~robj->globalHandle->var) && 
    carry == 1)
        carry = 1;
    else 
        carry = 0;
    
    
    Cell* lHandle = lobj->globalHandle->L;
    Cell* rHandle = robj->globalHandle->L;
    const bool lsign = !lobj->sgn();
    const bool rsign = !robj->sgn();

    do{
        // addition both cells and carry
        result.newCell((lHandle == lobj->globalHandle ? (lsign ? mask000 : mask111) : lHandle->var) + 
        ~(rHandle == robj->globalHandle ? (rsign ? mask000 : mask111) : rHandle->var) + carry);
        
        // keep carry to next iteration
        if((result.globalHandle->R->var < lHandle->var && result.globalHandle->R->var < rHandle->var) ||
        (result.globalHandle->R->var <= lHandle->var && result.globalHandle->R->var <= rHandle->var && carry == 1)) 
            carry = 1;
        else 
            carry = 0;

        // change handles to next cell if not reached end variable yet
        if(lHandle != lobj->globalHandle) lHandle = lHandle->L;
        if(rHandle != robj->globalHandle) rHandle = rHandle->L;
    }while(lHandle != lobj->globalHandle || rHandle != robj->globalHandle);

    // overflow can only appear when subtracting oposite signs values
    if(lsign != rsign){
        // if lsign is positive => result is also positive
        if(lsign) result.newCell(mask000);
        // if lsign is negative => result is also negative
        else result.newCell(mask111);
    }
    result.optymize();

    return result;
}
/**
 * @brief 
 * @param right 
 */
void ALi::subtractionAssign(const ALi& right){
    this->assignment(this->subtraction(right));
}
    // #
    // #
    // #
    // #
    // #
    // # Multiplication
    // #
    // #
    // #
    // #
    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::multiplication(const ALi& right){
    if(this->is0() || right.is0()) return 0;
    else if(right.is1()) return *this;
    else if(this->is1()) return right;

    const ALi*const greater = (this->length > right.length ? this : &right);
    const ALi*const  smaller = (this->length > right.length ? &right : this);
    
    ALi snegate(*smaller);
    snegate.invert();
    
    ALi* lhalf = new ALi();
    ALi* rhalf = new ALi(*greater);
    for(unsigned long long i=0; i<greater->length-1; i++)
        lhalf->newCell(0);
    
    CELL_TYPE carry = 0;
    for(unsigned long long i=0; i<greater->length-1; i++){
        if(rhalf->globalHandle->var & mask001 == 1 && carry == 0)
            lhalf->additionAssign(*smaller);
        else if(rhalf->globalHandle->var & mask001 == 0 && carry == 1)
            lhalf->additionAssign(snegate);

        if(rhalf->globalHandle->var & mask001) carry = 1;
        else carry = 0;

        if(lhalf->globalHandle->var & mask001) rhalf->PMSB(1);
        else rhalf->PMSB(0);

        if(lhalf->globalHandle->R->var & mask100) lhalf->PMSB(1);
        else lhalf->PMSB(1);
    }
    ALi result(*rhalf);
    // const Cell* lhalfhandle = lhalf->globalHandle;
    // for(unsigned long long i=0; i<greater->length-1; i++){
    //     result.newCell(lhalfhandle->var);
    //     lhalfhandle = lhalfhandle->L;
    // }

    return result;
}
/**
 * @brief 
 * @param right 
 */
void ALi::multiplicationAssign(const ALi& right){

}
    // #
    // #
    // #
    // #
    // #
    // # Division
    // #
    // #
    // #
    // #
    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::division(const ALi& right){
    return 0;
}
/**
 * @brief 
 * @param right 
 */
void ALi::divisionAssign(const ALi& right){
    
}
    // #
    // #
    // #
    // #
    // #
    // # Public
    // #
    // #
    // #
    // #
    // #
/**
 * @brief print variable as a binary or decimal
 * @param type type of file, decimal('d') or binary('b')
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::print(const char& type, const char* additionText, unsigned long long alignment) const{
    switch (type){
        case 'b': 
            for(unsigned long long i=0; i<alignment; i++){
                printf("%s",(this->sgn() ? "11111111" : "00000000"));
                if(this->separator != '\0') printf("%c",this->separator);
            }
            this->printBinary();
            break;
        case 'd': 
            for(unsigned long long i=0; i<alignment; i++){
                printf("%s","000");
                if(this->separator != '\0') printf("%c",this->separator);
            }
            this->printDecimal(); 
            break;
        default: printf("unknown type!\n"); return;
    }
    printf("%s",additionText);
}
/**
 * @brief printing binary variable in scientific notation format
 * @param type
 * @param additionText 
 * @param approximationPrecision simply how many digits (counting from left and without variable sign) will be printed
 */
void ALi::printApproximation(const char& type, const char* additionText, unsigned long long approximationPrecision) const{
    switch (type){
    case 'b': this->printBinaryApproximation(approximationPrecision); break;
    case 'd': this->printDecimalApproximation(approximationPrecision); break;
    default: printf("unknown type!\n"); return;
    }
    printf("%s",additionText);
}
/**
 * @brief actions on files around variable
 * @param path path to file or direction
 * @param action type of action write('w') or read('r')
 * @param type type of file, readable('r') or binary('b')
 */
void ALi::file(const char* path, const char& action, const char& type){
    switch (action){
        case 'w': this->writeFile(path,type); break;
        case 'r': this->readFile(path,type); break;
        default: printf("unknown action!\nnot attempted to write/append/read a file\n"); return;
    }
}
    // #
    // #
    // #
    // #
    // #
    // # Get / Set
    // #
    // #
    // #
    // #
    // #
/**
 * @brief set separator sign which should shown every 8 bits (byte)
 * @param separatorSign default is '\0'(all bytes will be concatenated) separate every byte from others (easier to read)
 */
void ALi::setSeparator(const char& separator){
    this->separator = separator;
}
/**
 * @brief returns separator sign which should shown every 8 bits (byte)
 * @return const char separator sign
 */
const char ALi::getSeparator() const{
    return this->separator;
}


/**
 * @brief check if is empty
 * @return true ALi is empty
 * @return false ALi containing something
 */
const bool ALi::isEmpty() const{
    return this->is0();
}