#include "AccurateLargeInteger.hpp"

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
    this->optymize();
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
/**
 * @brief return sign bit
 * @return true sign bit is 1 => number is negative
 * @return false sign bit is 0 => number is positive
 */
const bool ALi::sgn() const{
    return (this->globalHandle->R->var & mask100 ? true : false);
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
    // should remove left cells until one of these paterns exists at the beginning (IMPROVED_PRINT): 
    // 1_1_1_1_1_1_1_1 1XXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX
    // 0_0_0_0_0_0_0_0 0XXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX
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
    const Cell* const handle = this->globalHandle->R; //last cell
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
/**
 * @brief print each byte of variable seperated by separator sign and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printBinary() const{
    const Cell* handle = this->globalHandle->R;
    do{
        printf("%s%c", BPrint::binary_x64(handle->var, ULL_VAR_SEP).c_str(),this->separator);
        handle = handle->R;
    }while(handle != this->globalHandle->R);
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
    
    ALi bcd; // 0
    const Cell* handle = this->globalHandle;
    // change binary to binary-coded decimal
    do{
        handle = handle->R;
        CELL_TYPE mask = mask100;
        while(mask != 0){
            bcd.PLSB((handle->var & mask ? 1 : 0));
            const bool old_bcdsgn = bcd.sgn();
            Cell* bcdhandle = bcd.globalHandle;
            do{
                bcdhandle = bcdhandle->R;

                // keep BCD formation on each nibble
                // 0000(0000) 0001(0001) 0010(0010) 0011(0011) 0100(0100)
                // 0101(1000) 0110(1001) 0111(1010) 1000(1011) 1001(1100)
                CELL_TYPE result = 0;
                int i=0;
                while(bcdhandle->var != 0){
                    CELL_TYPE nibble = bcdhandle->var & 0b00001111;
                    bcdhandle->var >>= 4;
                    if(nibble > 4) nibble += 3;
                    for(int j=0; j<i; j++) nibble <<= 4;
                    
                    result |= nibble;
                    i++;
                }
                bcdhandle->var = result;
            }while(bcdhandle != bcd.globalHandle);
            if(old_bcdsgn != bcd.sgn()) bcd.newCell(mask000);
            mask >>= 1;
        }
    }while(handle != this->globalHandle);


    // CELL_TYPE BCDincrement(CELL_TYPE cell){
    //     // result can't exceed the range of CELL_TYPE
    //     // and assume there won't be nibble(4bits) greater than 1001 to save one if statement
    //     CELL_TYPE result = 0;
    //     int i=0;
    //     while(cell != 0){
    //         CELL_TYPE nibble = cell & 0b00001111;
    //         cell >>= 4;
    //         if(nibble > 4) nibble += 3;
    //         for(int j=0; j<i; j++) nibble <<= 4;
    //         result |= nibble;
    //         i++;
    //     }
    //     return result;
    // }
    // do{
    //     handle = handle->R;
    //     CELL_TYPE mask = mask100;
    //     while(mask != 0){
    //         bcd.PLSB((handle->var & mask ? 1 : 0));
    //         const bool old_bcdsgn = bcd.sgn();
    //         Cell* bcdhandle = bcd.globalHandle;
    //         do{
    //             bcdhandle = bcdhandle->R;
    //             bcdhandle->var = BCDincrement(bcdhandle->var);
    //         }while(bcdhandle != bcd.globalHandle);
    //         if(old_bcdsgn != bcd.sgn()) bcd.newCell(mask000);
    //         mask >>= 1;
    //     }
    // }while(handle != this->globalHandle);

    // print binary-coded decimal
    std::string rev;
    while(bcd.globalHandle->R->var != 0){ 
        const char nibble = bcd.globalHandle->R->var & 0b00001111;
        rev = std::string(1,(nibble > 4 ? nibble+45 : nibble+48)) + rev; 
        bcd.globalHandle->R->var >>= 4;
    }
    printf("%s",rev.c_str());
    bcd.delCell();
    if(bcd.is0()) return;

    do{
        std::string rev;
        for(int i=0; i<16; i++){
            const char nibble = bcd.globalHandle->R->var & 0b00001111; 
            rev = std::string(1,(nibble > 4 ? nibble+45 : nibble+48)) + rev; 
            bcd.globalHandle->R->var >>= 4;
        }
        printf("%s",rev.c_str());
    }while(bcd.delCell());

}
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
        std::string binary = BPrint::binary_x64(handle->var,ULL_VAR_SEP);
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
    // File
    FILE* file = fopen(path,"r");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    
    char type = '\0';
    fread(&type, 1, 1, file); // don't matter if fill type or not cause important is just what sign it is
    std::string strfdata;
    char buffer;
    while(fread(&buffer, 1, 1, file) != 0)
        if(buffer == '0' || buffer == '1')
            strfdata += buffer;


    switch (type){
    case 'b':{
        const char* fdata = strfdata.c_str();
        this->globalHandle->var = (*fdata=='0' ? mask000 : mask111);
        while(*fdata != '\0')
            this->PLSB((*(fdata++)=='0' ? 0 : 1)); // x++ increment after returning value
        break;
    }
    case 'd':{

        break;
    }
    // in future can be extended by octal and hex 
    default: printf("unknown number fromat type!\nnot attempted to overwrite number\n"); return;
    }

    this->optymize();
    return;
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
    
    // # Assignment
    
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
/**
 * @brief 
 * @param source 
 * @example "b010101010101"
 * @example "d765238745629"
 * @example "b1110101010101"
 * @example "d-765238745629"
 */
void ALi::assignment(const std::string& source){
    const char type = source[0];
    std::string value(source);
    value.erase(value.begin());
    switch(type){
        case 'b':

        break;
        case 'd':

        break;
        default: printf("unknown action!\nnot attempted to assign the value\n"); return;
    }
    
}
    // #
    
    // #
    
    // #
    
    // # Boolean
    
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
    
    // # Addition
    
    // #
    
    // #
    
    // #
/**
 * @brief increment ALi by one
 * 
 */
void ALi::increment(){
    // incrementing binary is acctually easy:
    // going through each bit from right
    // if is 1 then set to 0
    // if is 0 then set to 1 exit
    
    this->optymize();
    Cell *handle = this->globalHandle;
    // set handle on first cell that can store additional bit
    // 10101101  11111111  11111111  11111111  11111111 -> [10101101] 00000000  00000000  00000000  00000000
    // 11111111                                         -> [11111111]
    // 10101101  11111111  11100011  11111111  11111111 ->  10101101  11111111 [11100011] 00000000  00000000
    // 00000010                                         -> [00000010]
    // 01111111  11111111                               -> [01111111] 00000000
    // 01111111  11110011                               ->  01111111 [11110011]
    while(handle->var == mask111 && handle != this->globalHandle->R){
        handle->var = mask000; // 11111111 -> (1)00000000
        handle = handle->L;
    }
    // if number is positive, overflow can occur cause we are adding two positive numbers
    // 10101101  11111111  11111111  11111111  11111111 -> [10101101] 00000000  00000000  00000000  00000000
    // 11111111                                         -> [11111111]
    // 10101101  11111111  11100011  11111111  11111111 ->  10101101  11111111 [11100011] 00000000  00000000
    // 00000010                                         -> [00000010]
    // 01111111  11111111                               ->  00000000 [01111111] 00000000
    // 01111111  11110011                               ->  01111111 [11110011]
    if(!this->sgn() && handle->var == mask011 && handle == this->globalHandle->R){
        this->newCell(mask000);
    }
    // 10101101  11111111  11111111  11111111  11111111 -> [10101110] 00000000  00000000  00000000  00000000
    // 11111111                                         -> [00000000]
    // 10101101  11111111  11100011  11111111  11111111 ->  10101101  11111111 [11100100] 00000000  00000000
    // 00000010                                         -> [00000011]
    // 01111111  11111111                               ->  00000000 [10000000] 00000000
    // 01111111  11110011                               ->  01111111 [11110100]
    handle->var++;
    // this->optymize();
}
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::addition(const ALi& right) const{
    if(right.is0()){ // L + 0 = L
        return *this;
    }
    else if (this->is0()){ // 0 + R = R
        return right;
    } 
    else if (right.is1()){ // L + 1 = L++
        ALi out(*this);
        out.increment();
        return out;
    }
    else if (this->is1()){ // 1 + R = R++
        ALi out(right);
        out.increment();
        return out;
    }

    ALi out;

    const Cell* const lgh = this->globalHandle;
    const Cell* lh = lgh->L;
    const bool lsign = this->sgn();
    Cell lmask{
        lmask.var = (lsign ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };
    // Cell lmask;
    // lmask.var = (lsign ? mask111 : mask000);
    // lmask.L = &lmask;
    // lmask.R = &lmask;

    const Cell* const rgh = right.globalHandle;
    const Cell* rh = rgh->L;
    const bool rsign = right.sgn();
    Cell rmask{
        rmask.var = (rsign ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };

    // Cell rmask;
    // rmask.var = (rsign ? mask111 : mask000);
    // rmask.L = &rmask;
    // rmask.R = &rmask;
    
    CELL_TYPE carry = 0;
    CELL_TYPE ofldet; // just keep equation result, instead of computing it every time while comparing
    
    ofldet = lgh->var + rgh->var;
    out.globalHandle->var = ofldet;
    if(ofldet < rgh->var && ofldet < lgh->var)
        carry = 1;
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    while(lh != &lmask || rh != &rmask){
        ofldet = lh->var + rh->var + carry;
        out.newCell(ofldet);
        // vvvv           carry is 0              ||               carry is 1                     vvvv
        if((ofldet < lh->var && ofldet < rh->var) || (ofldet <= lh->var && ofldet <= rh->var && carry))
            carry = 1;
        else
            carry = 0;

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;
    }
    // overflow can only appear when both operation argument sign values are equal
    if(lsign == rsign && lsign != out.sgn())
        out.newCell(lmask.var);
    out.optymize();
    return out;
}
/**
 * @brief 
 * @param right 
 */
void ALi::additionAssign(const ALi& right){
    if(right.is0()){ // L += 0 == L
        return;
    }
    else if (this->is0()){ // 0 += R == R
        this->assignment(right);
        return;
    }
    else if (right.is1()){ // L += 1 == L++
        this->increment();
        return;
    }
    else if (this->is1()){ // 1 += R == R++
        this->assignment(right);
        this->increment();
        return;
    }
    
    // this->assignment(this->addition(right));

    ALi* const lobj = this; 
    Cell* const lgh = lobj->globalHandle;
    Cell* lh = lgh->L;
    const bool lsign = lobj->sgn();
    const CELL_TYPE lmask = (lsign ? mask111 : mask000);

    const ALi* const robj = &right;
    const Cell* const rgh = robj->globalHandle;
    const Cell* rh = rgh->L;
    const bool rsign = robj->sgn();
    const CELL_TYPE rmask = (rsign ? mask111 : mask000);
    
    CELL_TYPE carry = 0;

    lgh->var += rgh->var;
    if(lgh->var < rgh->var)
        carry = 0;
        
    while(lh != lgh || rh != rgh){ // continue if both are not their global handles !(lh == lgh && rh == rgh)
        if(lh == lgh) this->newCell(lmask + (rh == rgh ? rmask : rh->var) + carry);
        else lh->var += (rh == rgh ? rmask : rh->var) + carry;

        if(lh->var < rh->var || (lh->var <= rh->var && carry)) carry = 1;
        else carry = 0;

        if(lh != lgh) lh = lh->L;
        if(rh != rgh) rh = rh->L;
    }
    // overflow can only appear when both operation argument sign values are equal
    if(lsign == rsign && lsign != this->sgn())
        this->newCell(lmask);
    this->optymize();
}
    // #
    
    // #
    
    // #
    
    // # Subtraction
    
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
    // if number is negative, overflow can occur cause we are adding two negative numbers
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
ALi ALi::subtraction(const ALi& right) const{
    if(right.is0()){ // L - 0 = L
        return *this;
    }
    else if(this->is0()){ // 0 - R = -R
        ALi out(right);
        out.invert();
        return out;
    }
    else if(right.is1()){ // L - 1 = L--
        ALi out(*this);
        out.decrement();
        return out;
    }
    else if(this->is1()){ // 1 - R = (-R)++
        ALi out(right);
        out.invert();
        out.increment();
        return out;
    }
    else if(this->equal(right)){ // L - R = 0    L==R
        return ALi(0);
    }

    //############################# not large impact to efficiency (+x)-(+y) == (+x)+(-y)
    ALi right_(right);
    right_.invert();

    return this->addition(right_);
    //#############################
}
/**
 * @brief 
 * @param right 
 */
void ALi::subtractionAssign(const ALi& right){
    if(right.is0()){ // L -= 0 == L
        return;
    }
    else if(this->is0()){ // 0 -= R == -R
        this->assignment(right);
        this->invert();
        return;
    }
    else if(right.is1()){ // L -= 1 == L--
        this->decrement();
        return;
    }
    else if(this->is1()){ // 1 -= R == (-R)++
        this->assignment(right);
        this->invert();
        this->increment();
        return;
    }
    else if(this->equal(right)){ // L -= R == 0    L==R
        this->clear();
        return;
    }
    
    //############################# not large impact to efficiency (+x)-=(+y) == (+x)+=(-y)
    ALi right_(right);
    right_.invert();

    this->additionAssign(right_);
    //#############################
}
    // #
    
    // #
    
    // #
    
    // # Multiplication
    
    // #
    
    // #
    
    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::multiplication(const ALi& right) const{
    if(this->is0() || right.is0()) return 0;
    else if(right.is1()) return *this;
    else if(this->is1()) return right;

    ALi _left(*this);
    ALi _right(right);
    ALi result;

    _left.PLSB(0);
    // _left.print('b',"\n");
    for(unsigned long long i=0; i<_left.length*BITS_PER_VAR-1; i++){
        // _left.print('b',"\n");
        // result.print('b',"\n\n");

        switch (_left.globalHandle->var & 0b11){
        case 1: // 01
            result.additionAssign(_right);
            break;
        case 2: // 10
            result.subtractionAssign(_right);
            break;
        // 00 & 11
        }
        // store lsb and shr result or shl _right
        _right.SHL();
        _left.SHR();
        _left.globalHandle->R->var = _left.globalHandle->R->var & mask011; // treat whole number as a unsigned shr
    }

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
    
    // # Division
    
    // #
    
    // #

    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::division(const ALi& right) const{
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
    
    // # Public
    
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
    
    // # Get / Set
    
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
    // #
    
    // #
    
    // #
    
    // # Operators
    
    // #
    
    // #
    
    // #

/**
 * @brief 
 * @param right "xtext" x-type of print (b/d), text-additional text
 * @example "d\n" - decimal print with new line 
 */
void ALi::operator >> (const char* right){
    this->print(right[0]);
    while(*right != '\0'){
        ++right;
        printf("%c",*right);
    }
}
/**
 * @brief read from file
 * @param right "xpath" x-type of file (r/b), path-path to file
 * @example "r.dvfiles/128.bit" - readable file in ".dvfiles/128.bit" path
 */
void ALi::operator << (const char* right){
    std::string _right(right);
    _right.erase(_right.begin());
    this->readFile(_right.c_str(),right[0]);
}
/**
 * @brief 
 * @param right 
 */
void ALi::operator =  (const ALi& right){
    this->assignment(right);
}

bool ALi::operator == (const ALi& right) const{
    return this->equal(right);
}
bool ALi::operator != (const ALi& right) const{
    return !this->equal(right);
}
bool ALi::operator >  (const ALi& right) const{
    return this->greaterThan(right);
}
bool ALi::operator <= (const ALi& right) const{
    return !this->greaterThan(right);
}
bool ALi::operator <  (const ALi& right) const{
    return this->smallerThan(right);
}
bool ALi::operator >= (const ALi& right) const{
    return !this->smallerThan(right);
}

ALi  ALi::operator ++ (int){
    ALi out(*this);
    this->increment();
    return out;
}
ALi  ALi::operator ++ (){
    this->increment();
    return *this;
}
ALi  ALi::operator +  (const ALi& right) const{
    return this->addition(right);
}
void ALi::operator += (const ALi& right){
    this->additionAssign(right);
}

ALi  ALi::operator -- (int){
    this->decrement();
    return *this;
}
ALi  ALi::operator -- (){
    this->decrement();
    return *this;
}
ALi  ALi::operator -  (const ALi& right) const{
    return this->subtraction(right);
}
void ALi::operator -= (const ALi& right){
    this->subtractionAssign(right);
}

ALi  ALi::operator *  (const ALi& right) const{
    return this->multiplication(right);
}
void ALi::operator *= (const ALi& right){
    this->multiplicationAssign(right);
}

// ALi  ALi::operator /  (const ALi& right){
//     return this->division(right);
// }
// void ALi::operator /= (const ALi& right){
//     this->divisionAssign(right);
// }
