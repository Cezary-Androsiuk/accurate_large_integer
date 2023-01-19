#include "AccurateLargeInteger.hpp"

//   fold all Ctrl + K,  Ctrl + 0

//   fold 1st Ctrl + K,  Ctrl + 1

// unfold all Ctrl + K,  Ctrl + J


/**
 * @brief Construct a new ALi object
 */
ALi::ALi(){
    this->length = 1;
    this->begin_ptr = new Cell;
    this->begin_ptr->L = this->begin_ptr;
    this->begin_ptr->R = this->begin_ptr;
    this->begin_ptr->var = mask000;
    this->separator = '\0'; // '/0' means no separator
}
/**
 * @brief Construct a new ALi object by using previously existing object
 * @param source ALi object which will be used to build identical ALi
 */
ALi::ALi(const ALi& source) : ALi(){
    this->assignment(source);
}
/**
 * @brief Construct a new ALi object by using intiger value
 * @param source value which will be the source to build ALi
 */
ALi::ALi(const signed long long& source) : ALi(){
    this->begin_ptr->var = source;
}
/**
 * @brief Construct a new ALi object by using existing file with value
 * @param type type of source file: binary: 'b', decimal: 'd'
 * @param path path to file where variable is stored
 */
ALi::ALi(const char* type, const char* sourcePath) : ALi(){
    this->readFile(type,sourcePath);
}
/**
 * @brief Destroy the ALi object
 * deletes all dynamically allocated cells
 */
ALi::~ALi(){
    this->clear();
    delete this->begin_ptr;
}
    // #
    
    // #
    
    // #
/**
 * @brief create new cell from the left side
 * @param standardSource: unsigned long long value that will be used to set, new cell value
 */
void ALi::newCell(const CELL_TYPE &standardSource){
    Cell* handle = new Cell;
    handle->L = this->begin_ptr;
    handle->R = this->begin_ptr->R;
    this->begin_ptr->R->L = handle;
    this->begin_ptr->R = handle;

    handle->var = standardSource;
    this->length++;
}
/**
 * @brief remove cell from the left, but not including this->begin_ptr
 * @brief if there is no cell to delete set this->begin_ptr to mask000
 * @return 1: if there are more cells that can be deleted,
 * @return 0: if there are no more cells that can be deleted (just last one was deleted)
 */
const bool ALi::delCell(){
    if(this->begin_ptr->R == this->begin_ptr){
        // all cells that can be deleted, was already deleted
        // one cell left
        this->begin_ptr->var = mask000;
        return false;
    }
    // n dynamically alocated cells where n >= 1
    Cell* handle = this->begin_ptr->R;
    handle->R->L = this->begin_ptr;
    this->begin_ptr->R = handle->R;
    this->length--;
    delete handle;
    return true;
}
    // #
    
    // #
    
    // #
/**
 * @brief execute shift right operation
 * @example 0000 1010 -> 0000 0101
 * @example 0011 1010 -> 0001 1101
 * @example 1000 0000 -> 1100 0000
 * @example 1000 0111 -> 1100 0011
 */
void ALi::SHR(){
    // works just like an >> operator for signed values
    Cell* handle = this->begin_ptr->R;
    bool buffer[2] = {0,this->sgn()};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        buffer[0] = handle->var & mask001;
        handle->var >>= 1; 
        if(buffer[1]) handle->var |= mask100;
        else handle->var &= mask011;
        buffer[1] = buffer[0]; 
        handle = handle->R;
    } while (handle != this->begin_ptr->R);
    this->optymize();
}
/**
 * @brief execute shift left operation
 * @example 0011 0110 -> 0110 1100
 * @example 1101 0000 -> 1010 0000
 * @example 1011 0110 -> 1111 0110 1100
 * @example 1000 0000 -> 1111 0000 0000
 * @example 0111 0010 -> 0000 1110 0100
 */
void ALi::SHL(){
    // works just like an << operator for signed values
    Cell* handle = this->begin_ptr; 
    bool buffer[2] = {0,0};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        buffer[0] = handle->var & mask100;
        handle->var <<= 1;
        if(buffer[1]) handle->var |= mask001;
        else handle->var &= mask110;
        buffer[1] = buffer[0];
        handle = handle->L;
    }while(handle != this->begin_ptr);

    // where most left bit after SHL changed their value, then overflow has occurred
    if((this->begin_ptr->R->var & mask100) != (buffer[1] ? mask100 : mask000)){
        if(buffer[1]) // check if value was positive or negative before SHL
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}
/**
 * @brief push the most significant bit
 * @param bit if 0, 0 will be pushed if 1, 1 will be pushed on lsb
 * @example bit=1 0000 -> 1000
 * @example bit=1 1000 -> 1100
 * @example bit=1 0010 0000 -> 1001 0000
 * @example bit=1 1110 0000 -> 1111 0000
 * @example bit=0 1111 -> 0111
 * @example bit=0 1000 -> 0100
 * @example bit=0 0010 0000 -> 0001 0000
 * @example bit=0 1110 0000 -> 0111 0000
 */
void ALi::PMSB(const bool& bit){
    // works just like an >> operator for signed values
    Cell* handle = this->begin_ptr->R;
    bool buffer[2] = {0,bit};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        buffer[0] = handle->var & mask001; 
        handle->var >>= 1; 
        if(buffer[1]) handle->var |= mask100;
        else handle->var &= mask011;
        buffer[1] = buffer[0]; 
        handle = handle->R;
    } while (handle != this->begin_ptr->R);
}
/**
 * @brief push the least significant bit
 * @param bit if 0, 0 will be pushed if 1, 1 will be pushed on 
 * @example bit=0 0011 0110 -> 0110 1100
 * @example bit=0 1101 0000 -> 1010 0000
 * @example bit=0 1011 0110 -> 1111 0110 1100
 * @example bit=0 1000 0000 -> 1111 0000 0000
 * @example bit=0 0111 0010 -> 0000 1110 0100
 * @example bit=1 0011 0110 -> 0110 1101
 * @example bit=1 1101 0000 -> 1010 0001
 * @example bit=1 1011 0110 -> 1111 0110 1101
 * @example bit=1 1000 0000 -> 1111 0000 0001
 * @example bit=1 0111 0010 -> 0000 1110 0101
 */
void ALi::PLSB(const bool& bit){
    // works just like an << operator for signed values
    Cell* handle = this->begin_ptr; 
    bool buffer[2] = {0,bit};
    /*[0] - cell bit out*/    /*[1] - cell bit in*/

    do{
        buffer[0] = handle->var & mask100;
        handle->var <<= 1;
        if(buffer[1]) handle->var |= mask001;
        else handle->var &= mask110;
        buffer[1] = buffer[0];
        handle = handle->L;
    }while(handle != this->begin_ptr);

    // where most left bit after SHL changed their value, then overflow has occurred
    if((this->begin_ptr->R->var & mask100) != (buffer[1] ? mask100 : mask000)){
        if(buffer[1]) // check if value was positive or negative before SHL
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}
    // #
    
    // #
    
    // #
/**
 * @brief return sign bit
 * @return true sign bit is 1 => number is negative,
 * @return false sign bit is 0 => number is positive
 */
const bool ALi::sgn() const{
    return (this->begin_ptr->R->var & mask100 ? true : false);
}
/**
 * @brief return if value is 0
 * @return true if value is 0,
 * @return false if value is something other than 0
 */
const bool ALi::is0() const{
    if(this->length == 1 && this->begin_ptr->var == mask000) return true;
    else return false;
}
/**
 * @brief return if value is 0
 * @return true if value is 1,
 * @return false if value is something other than 1
 */
const bool ALi::is1() const{
    if(this->length == 1 && this->begin_ptr->var == mask001) return true;
    else return false;
}
    // #
    
    // #
    
    // #
/**
 * @brief remove all dynamically allocated cells 
 */
void ALi::clear(){
    while(this->delCell());
}
/**
 * @brief removes cells from begin which are not include any new information like 00000000 or 11111111
 * @example 1111 1111 1100 0101 -> 1100 0101
 * @example 0000 0000 0100 0101 -> 0100 0101
 * @example 1111 1111 0100 0101 -> 1111 0100 0101
 * @example 0000 0000 1100 0101 -> 0000 1100 0101
 */
void ALi::optymize(){
    // -- 1 -- // 
    // const Cell* const handle = this->begin_ptr->R;
    // if(handle != this->begin_ptr){
    //     if((handle->var == mask111 && handle->R->var & mask100) || (handle->var == mask000 && ~handle->R->var & mask100)){
    //         this->delCell();
    //         this->optymize();
    //     }
    // }

    // -- 2 -- //
    // const Cell* const handle = this->begin_ptr->R;
    // if((handle != this->begin_ptr && handle->var == mask111 && handle->R->var & mask100) || 
    // (handle != this->begin_ptr && handle->var == mask000 && ~handle->R->var & mask100)){
    //         this->delCell();
    //         this->optymize();
    // }

    // -- 3 -- //
    const Cell* const handle = this->begin_ptr->R; //last cell
    if(handle != this->begin_ptr &&
    ((handle->var == mask111 && handle->R->var & mask100) || (handle->var == mask000 && ~handle->R->var & mask100))){
            this->delCell();
            this->optymize();
    }

    // -- 4 -- //
    // if(this->begin_ptr->R != this->begin_ptr && 
    // ((this->begin_ptr->R->var == mask111 && this->begin_ptr->R->R->var & mask100) || 
    // (this->begin_ptr->R->var == mask000 && ~this->begin_ptr->R->R->var & mask100))){
    //         this->delCell();
    //         this->optymize();
    // }

    // -- 5 -- //
    // if(this->sign()){
    //     while(this->begin_ptr->R->var == mask111 && 
    //     this->begin_ptr->R->R->var & mask100 &&
    //     this->begin_ptr->R != this->begin_ptr)
    //         this->delCell();
    // }
    // else{ // is positive
    //     while(this->begin_ptr->R->var == mask000 && 
    //     ~this->begin_ptr->R->R->var & mask100 &&
    //     this->begin_ptr->R != this->begin_ptr)
    //         this->delCell();
    // }
}
/**
 * @brief change every single bit to oposite value
 */
void ALi::negate(){
    Cell* handle = this->begin_ptr;
    do{
        handle->var = ~handle->var;
        handle = handle->L;
    }while(handle != this->begin_ptr);
}
/**
 * @brief inverting value from (3) to (-3) and from (-10) to (10) etc. ...
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
    
    // # Print
    
    // #
/**
 * @brief print each byte of variable seperated by separator sign and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printBinary() const{
    const Cell* handle = this->begin_ptr->R;
    do{
        printf("%s%c", BPrint::binary_x64(handle->var, ULL_VAR_SEP).c_str(),this->separator);
        handle = handle->R;
    }while(handle != this->begin_ptr->R);
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
    const Cell* handle = this->begin_ptr;
    // change binary to binary-coded decimal
    do{
        handle = handle->R;
        CELL_TYPE mask = mask100;
        while(mask != 0){
            bcd.PLSB((handle->var & mask ? 1 : 0));
            const bool old_bcdsgn = bcd.sgn();
            Cell* bcdhandle = bcd.begin_ptr;
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
            }while(bcdhandle != bcd.begin_ptr);
            if(old_bcdsgn != bcd.sgn()) bcd.newCell(mask000);
            mask >>= 1;
        }
    }while(handle != this->begin_ptr);


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
    //         Cell* bcdhandle = bcd.begin_ptr;
    //         do{
    //             bcdhandle = bcdhandle->R;
    //             bcdhandle->var = BCDincrement(bcdhandle->var);
    //         }while(bcdhandle != bcd.begin_ptr);
    //         if(old_bcdsgn != bcd.sgn()) bcd.newCell(mask000);
    //         mask >>= 1;
    //     }
    // }while(handle != this->begin_ptr);

    // print binary-coded decimal
    std::string rev;
    while(bcd.begin_ptr->R->var != 0){ 
        const char nibble = bcd.begin_ptr->R->var & 0b00001111;
        rev = std::string(1,(nibble > 4 ? nibble+45 : nibble+48)) + rev; 
        bcd.begin_ptr->R->var >>= 4;
    }
    printf("%s",rev.c_str());
    bcd.delCell();
    if(bcd.is0()) return;

    do{
        std::string rev;
        for(int i=0; i<16; i++){
            const char nibble = bcd.begin_ptr->R->var & 0b00001111; 
            rev = std::string(1,(nibble > 4 ? nibble+45 : nibble+48)) + rev; 
            bcd.begin_ptr->R->var >>= 4;
        }
        printf("%s",rev.c_str());
    }while(bcd.delCell());

}
    // #
    
    // # Print Approximation
    
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
    
    // # Store
    
    // #
/**
 * @brief save variable to file in binary form
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::export_cells(const char* path) const{
    FILE* file = fopen(path,"wb");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    const Cell* handle = this->begin_ptr;
    do{
        fwrite(&handle->var, BYTES_PER_VAR, 1, file); // bufer, size of cell, cell amout, source
        handle = handle->L;
    }while(handle != this->begin_ptr);
    fclose(file);
}
/**
 * @brief copy value from source file to variable
 * @param path source where an readable binary file is stored
 */
void ALi::import_cells(const char* path){
    this->clear();
    FILE* file = fopen(path,"rb");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    fread(&this->begin_ptr->var, BYTES_PER_VAR, 1, file);
    char buffer;
    while(fread(&buffer, BYTES_PER_VAR, 1, file) != 0){
        this->newCell(buffer);
    }
    this->optymize();
    fclose(file);
}
    // #
    
    // # File Write
    
    // #
/**
 * @brief save variable to file in readable form allowing to add separator sign every 8 bits 
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFile_02(FILE* const file) const{
    Cell* handle = this->begin_ptr->R;
    do{
        std::string binary = BPrint::binary_x64(handle->var,ULL_VAR_SEP);
        fwrite(binary.c_str(), sizeof(char), binary.length(), file); // bufer, size of cell, cell amout, source
        handle = handle->R;
    }while(handle != this->begin_ptr->R);
}
/**
 * @brief save variable to file in readable form allowing to add separator sign every 8 bits 
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFile_10(FILE* const file) const{

}
/**
 * @brief save variable to file 
 * @param path path to file where variable should be stored
 * @param type type of file, readable('r') or binary('b')
 * @param append append or overwrite file if exist 
 */
void ALi::writeFile(const char* type, const char* path) const{
    FILE* file = fopen(path,"w");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    switch (type[0]){
        case 'b': this->writeFile_02(file); break;
        case 'd': this->writeFile_10(file); break;
        default: printf("writeFile: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",type[0]); return;
    }
    fclose(file);
}
    // #
    
    // # File Read
    
    // #
/**
 * @brief copy value from readable source file to variable and ignore others than '1' and '0' signs
 * @param path source where an readable binary file is stored
 */
void ALi::readFile_02(FILE* const file){
    std::string fdata;
    char buffer;
    while(fread(&buffer, 1, 1, file) != 0)
        if(buffer == '0' || buffer == '1')
            fdata += buffer;
    const char* cstrdata = fdata.c_str();
    this->begin_ptr->var = (*cstrdata=='0' ? mask000 : mask111);
    while(*cstrdata != '\0')
        this->PLSB((*(cstrdata++)=='0' ? 0 : 1)); // x++ increment after returning value
    this->optymize();
}
/**
 * @brief copy value from readable source file to variable and ignore others than '1' and '0' signs
 * @param path source where an readable binary file is stored
 */
void ALi::readFile_10(FILE* const file){

}
/**
 * @brief copy value from file to variable
 * @param path source where an readable binary file is stored
 * @param type type of file, readable('r') or binary('b')
 */
void ALi::readFile(const char* type, const char* path){
    FILE* file = fopen(path,"r");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    switch (type[0]){
        case 'b': this->readFile_02(file); break;
        case 'd': this->readFile_10(file); break;
        default: printf("readFile: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",type[0]); return;
    }
    fclose(file);
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
    
    // # Assignment
    
    // #
/**
 * @brief assigns existed ALi to variable
 * @param source ALi what will be assigned to variable
 */
void ALi::assignment(const ALi& source){
    // it will be nice to just drop this variable begin_ptr and focus on assigning 
    // clearig can be leaved for other cpu process
    // const int *  -  (pointer to const int)
    // int const *  -  (pointer to const int)
    // int * const  -  (const pointer to int)
    const Cell* srchandle = source.begin_ptr->L;
    this->clear();
    this->begin_ptr->var = source.begin_ptr->var;
    while (srchandle != source.begin_ptr){
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
    this->begin_ptr->var = source;
}
    // #
    
    // # Assignment string
    
    // #
/**
 * @brief 
 * @param source 
 */
void ALi::assignment_02(const std::string& source){
    this->clear();
    if(source[1] == '1')
        this->begin_ptr->var = mask111;

    for(char src: source){
        switch(src){
            case '0': this->PLSB(0); break;
            case '1': this->PLSB(1); break;
            default: break;
        }
    }
}
/**
 * @brief 
 * @param source 
 */
void ALi::assignment_10(const std::string& source){

}
/**
 * @brief 
 * @param source 
 * @example "b010101010101"
 * @example "d765238745629"
 * @example "b1110101010101"
 * @example "d-765238745629"
 */
void ALi::assignment_str(const std::string& source){
    std::string value(source);
    switch(source[0]){
        case 'b': this->assignment_02(value); break;
        case 'd': this->assignment_10(value); break;
        default: printf("print: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",source[0]); return;
    }
}
    // #
    
    // # Boolean
    
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
    const Cell* hL = this->begin_ptr;
    const Cell* hR = right.begin_ptr;
    do{
        if(hL->var != hR->var) return false;
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->begin_ptr);
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
    Cell* hL = this->begin_ptr;
    Cell* hR = right.begin_ptr;
    do{
        if(hL->var > hR->var) return true; // left is greater
        if(hL->var < hR->var) return false; // left is smaller
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->begin_ptr); // compare each cell, begin from the most significant
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
    Cell* hL = this->begin_ptr;
    Cell* hR = right.begin_ptr;
    do{
        if(hL->var > hR->var) return false; // left is greater
        if(hL->var < hR->var) return true; // left is smaller
        hL = hL->R;
        hR = hR->R;
    }while(hL != this->begin_ptr); // compare each cell, begin from the most significant
    return false; // both are equal => left is NOT smaller
}
    // #
    
    // # Addition
    
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
    Cell *handle = this->begin_ptr;
    // set handle on first cell that can store additional bit
    // 10101101  11111111  11111111  11111111  11111111 -> [10101101] 00000000  00000000  00000000  00000000
    // 11111111                                         -> [11111111]
    // 10101101  11111111  11100011  11111111  11111111 ->  10101101  11111111 [11100011] 00000000  00000000
    // 00000010                                         -> [00000010]
    // 01111111  11111111                               -> [01111111] 00000000
    // 01111111  11110011                               ->  01111111 [11110011]
    while(handle->var == mask111 && handle != this->begin_ptr->R){
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
    if(!this->sgn() && handle->var == mask011 && handle == this->begin_ptr->R){
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

    const Cell* const lgh = this->begin_ptr;
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

    const Cell* const rgh = right.begin_ptr;
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
    out.begin_ptr->var = ofldet;
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
 * @return ALi 
 */
ALi ALi::addition2(const ALi& right) const{
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

    const Cell* const lgh = this->begin_ptr;
    const Cell* lh = lgh->L;
    const bool lsign = this->sgn();
    Cell lmask{
        lmask.var = (lsign ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const Cell* const rgh = right.begin_ptr;
    const Cell* rh = rgh->L;
    const bool rsign = right.sgn();
    Cell rmask{
        rmask.var = (rsign ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };
    
    unsigned char carry = __builtin_add_overflow(lgh->var, rgh->var, &out.begin_ptr->var);
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    CELL_TYPE sum;
    while(lh != &lmask || rh != &rmask){
        carry = __builtin_add_overflow(lh->var,carry,&sum);
        carry += __builtin_add_overflow(rh->var,sum,&sum);
        out.newCell(sum);

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
    Cell* const lgh = lobj->begin_ptr;
    Cell* lh = lgh->L;
    const bool lsign = lobj->sgn();
    const CELL_TYPE lmask = (lsign ? mask111 : mask000);

    const ALi* const robj = &right;
    const Cell* const rgh = robj->begin_ptr;
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
/**
 * @brief 
 * @param right 
 */
void ALi::additionAssign2(const ALi& right){
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
    Cell* const lgh = lobj->begin_ptr;
    Cell* lh = lgh->L;
    const bool lsign = lobj->sgn();
    Cell lmask{
        lmask.var = (lsign ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const ALi* const robj = &right;
    const Cell* const rgh = robj->begin_ptr;
    const Cell* rh = rgh->L;
    const bool rsign = robj->sgn();
    Cell rmask{
        rmask.var = (rsign ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };
    
    unsigned char carry = 0;
    carry = __builtin_add_overflow(lgh->var,rgh->var,&lgh->var);
        
    while(lh != lgh || rh != rgh){ // continue if both are not their global handles !(lh == lgh && rh == rgh)

        if(lh == lgh) this->newCell(lmask.var + rh->var + carry);
        else lh->var += rh->var + carry;

        if(lh->var < rh->var || (lh->var <= rh->var && carry)) carry = 1;
        else carry = 0;

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;
    }
    // overflow can only appear when both operation argument sign values are equal
    if(lsign == rsign && lsign != this->sgn())
        this->newCell(lmask.var);
    this->optymize();
}
    // #
    
    // # Subtraction
    
    // #
/**
 * @brief increment ALi by one
 * 
 */
void ALi::decrement(){
    Cell *handle = this->begin_ptr;
    while(handle->var == mask000 && handle != this->begin_ptr->R){
        handle->var = mask111; // (1)00000000 -> (0)11111111
        handle = handle->L;
    }
    // if number is negative, overflow can occur cause we are adding two negative numbers
    if(this->sgn() && (handle->var == mask100 && handle == this->begin_ptr->R)){
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
    
    // # Multiplication
    
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

    
    ALi slider(*this);
    ALi multiplier(right); // factor
    slider.setSeparator(' ');
    multiplier.setSeparator(' ');

    slider.PLSB(0);
    // if(slider.length < multiplier.length){
    //     if(!slider.sgn()){ // positive
    //         while(slider.length < multiplier.length)
    //             slider.newCell(mask000);
    //     }
    //     else{ // negative
    //         while(slider.length < multiplier.length)
    //             slider.newCell(mask111);
    //     }
    // }
    // else{
    //     if(!multiplier.sgn()){ // positive
    //         while(slider.length > multiplier.length)
    //             multiplier.newCell(mask000);
    //     }
    //     else{ // negative
    //         while(slider.length > multiplier.length)
    //             multiplier.newCell(mask111);
    //     }
    // }
    // // both are the same length
    // unsigned long long expected_multiplier_length = slider.length + multiplier.length;
    // while()
    
    slider >> "b\n";
    multiplier >> "b\n\n";

    
    unsigned long long slider_length = slider.length*BITS_PER_VAR;
    if(!slider.sgn()){
        for(unsigned long long i=0; i<slider_length; i++){
            multiplier >> "b\n\n";
            multiplier.PLSB(0);
        }
    }
    else{
        // for(unsigned long long i=0; i<slider_length; i++){
        //     multiplier >> "b\n\n";
        //     multiplier.PLSB(1);
        // }
    }
    // slider >> "b\n\n";
    multiplier >> "b\n\n";
    
    for(unsigned long long i=0; i<slider_length; i++){
        switch (slider.begin_ptr->var & 0b11){
        case 1: // 01
            slider.additionAssign(multiplier);
            break;
        case 2: // 10
            slider.subtractionAssign(multiplier);
            break;
        // 00 & 11
        }
        slider.SHR();
    }

    return slider;


    // ALi _left(*this);
    // ALi _right(right);
    // ALi result;

    // _left.PLSB(0);
    // // _left.print('b',"\n");
    // for(unsigned long long i=0; i<_left.length*BITS_PER_VAR-1; i++){
    //     // _left.print('b',"\n");
    //     // result.print('b',"\n\n");

    //     switch (_left.begin_ptr->var & 0b11){
    //     case 1: // 01
    //         result.additionAssign(_right);
    //         break;
    //     case 2: // 10
    //         result.subtractionAssign(_right);
    //         break;
    //     // 00 & 11
    //     }
    //     // store lsb and shr result or shl _right
    //     _right.SHL();
    //     _left.SHR();
    //     _left.begin_ptr->R->var = _left.begin_ptr->R->var & mask011; // treat whole number as a unsigned shr
    // }

    // return result;
}
/**
 * @brief 
 * @param right 
 */
void ALi::multiplicationAssign(const ALi& right){

}
    // #
    
    // # Division

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
    
    // #
    
    // #
    
    // #
    
    // #
    
    // #
    
    // #

    
    // #
    
    // # Public
    
    // #
/**
 * @brief print variable as a binary or decimal
 * @param additionText "b...","d..." type and text what will be printed at the end of variable
 */
void ALi::print(const char* type_text) const{
    switch (type_text[0]){
        case 'b': this->printBinary(); break;
        case 'd': this->printDecimal(); break;
        default: printf("print: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",type_text[0]); return;
    }
    while(*type_text != '\0'){
        ++type_text;
        printf("%c",*type_text);
    }
}
/**
 * @brief printing binary variable in scientific notation format
 * @param type
 * @param additionText 
 * @param approximationPrecision simply how many digits (counting from left and without variable sign) will be printed
 */
void ALi::printApproximation(const char* type_text, unsigned long long approximationPrecision) const{
    // switch (type){
    // case 'b': this->printBinaryApproximation(approximationPrecision); break;
    // case 'd': this->printDecimalApproximation(approximationPrecision); break;
    // default: printf("unknown type!\n"); return;
    // }
    // printf("%s",additionText);
}
/**
 * @brief actions on files around variable
 * @param path path to file or direction
 * @param action type of action write('w') or read('r')
 * @param type type of file, readable('r') or binary('b')
 */
void ALi::file(const char* type_path){
    std::string str(type_path);
    const char action = str[0];
    const char type[] = {str[1],'\0'};
    str.erase(0,2);
    switch (action){
        case 'w': this->writeFile(type,str.c_str()); break;
        case 'r': this->readFile(type,str.c_str()); break;
        default: printf("file: unknown action: '%c'\nread: 'r'\n write: 'w'\n",type[0]); return;
    }
}
    // #
    
    // # Get / Set
    
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
    
    // # Operators
    
    // #
/**
 * @brief print() shortcut 
 * @brief "d\n" - decimal print with new line 
 * @param right "xtext" x-type of print (b/d), text-additional text
 */
void ALi::operator >> (const char* right) const{
    this->print(right);
}
/**
 * @brief read from char array 
 * @brief  "b0101010101" - positive binary value 
 * @brief  "b1101010101" - negative binary value 
 * @brief  "d7894535434" - positive decimal value 
 * @brief  "d-894535434" - negative decimal value 
 * @param right "xvalue" x-type of value (b/d), value-value
 */
void ALi::operator << (const char* right){
    this->assignment_str(right);
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
