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
 * @param source ALi object which will be used as a source
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
 * @param type what value of the variable format will be expected [binary/decimal]
 * @param path path to file where value is stored in readable type
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
    ++this->length;
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
 * @brief 
 */
void ALi::shr_(){
    // works just like an >> operator for signed values
    Cell* handle = this->begin_ptr->R;
    bool buffer[2] = {0,this->sign()};
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
 * @brief execute shift right operation with variable controll systems
 * @example 0000 1010 -> 0000 0101
 * @example 0011 1010 -> 0001 1101
 * @example 1000 0000 -> 1100 0000
 * @example 1000 0111 -> 1100 0011
 */
void ALi::shr_ext(){
    // works just like an >> operator for signed values
    Cell* handle = this->begin_ptr->R;
    bool buffer[2] = {0,this->sign()};
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
 * @brief 
 */
void ALi::shl_(){
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
}
/**
 * @brief execute shift left operation with variable controll systems
 * @example 0011 0110 -> 0110 1100
 * @example 1101 0000 -> 1010 0000
 * @example 1011 0110 -> 1111 0110 1100
 * @example 1000 0000 -> 1111 0000 0000
 * @example 0111 0010 -> 0000 1110 0100
 */
void ALi::shl_ext(){
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

    // where most left bit after shl_ext changed their value, then overflow has occurred
    if((this->begin_ptr->R->var & mask100) != (buffer[1] ? mask100 : mask000)){
        if(buffer[1]) // check if value was positive or negative before shl_ext
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}
/**
 * @brief push the most significant bit
 * @param bit bit you want to add at the beginning
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
 * @param bit bit you want to add at the end
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

    // where most left bit after shl_ext changed their value, then overflow has occurred
    if((this->begin_ptr->R->var & mask100) != (buffer[1] ? mask100 : mask000)){
        if(buffer[1]) // check if value was positive or negative before shl_ext
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
 * @return true  sign bit is 1  =>  number is negative\
 * @return false sign bit is 0  =>  number is positive
 */
const bool ALi::sign() const{
    return (this->begin_ptr->R->var & mask100 ? true : false);
}
/**
 * @brief checks if value is 0
 * @return true if value is 0\
 * @return false if value is something other than 0
 */
const bool ALi::is_0(const Cell* const handle) const{
    if(handle == nullptr)
        return this->is_0(this->begin_ptr);
    else if(handle->var == mask000){
        if(handle->L != this->begin_ptr)
            return this->is_0(handle->L);
        else return true;
    }
    else return false;
}
    // #
    
    // #
    
    // #
/**
 * @brief checks if value is 1
 * @return true if value is 1\
 * @return false if value is something other than 1
 */
const bool ALi::is_p1(const Cell* const handle) const{
    // if(this->length == 1 && this->begin_ptr->var == mask001) return true;
    // else return false;

    if(handle == nullptr){
        if(this->begin_ptr->var == 1){
            if(this->begin_ptr->L != this->begin_ptr)
                return this->is_p1(this->begin_ptr->L);
            else return true;
        }
        else return false;
    }
    else{
        if(handle->var == mask000){
            if(handle->L != this->begin_ptr)
                return this->is_p1(handle->L);
            else return true;
        }
        else return false;
    }
}
/**
 * @brief checks if value is 2
 * @return true if value is 2\
 * @return false if value is something other than 2
 */
const bool ALi::is_p2(const Cell* const handle) const{
    if(handle == nullptr){
        if(this->begin_ptr->var == 2){
            if(this->begin_ptr->L != this->begin_ptr)
                return this->is_p2(this->begin_ptr->L);
            else return true;
        }
        else return false;
    }
    else{
        if(handle->var == mask000){
            if(handle->L != this->begin_ptr)
                return this->is_p2(handle->L);
            else return true;
        }
        else return false;
    }
}
/**
 * @brief checks if value is -1
 * @return true if value is -1\
 * @return false if value is something other than -1
 */
const bool ALi::is_n1(const Cell* const handle) const{
    if(handle == nullptr){
        if(this->begin_ptr->var == mask111){
            if(this->begin_ptr->L != this->begin_ptr)
                return this->is_n1(this->begin_ptr->L);
            else return true;
        }
        else return false;
    }
    else{
        if(handle->var == mask111){
            if(handle->L != this->begin_ptr)
                return this->is_n1(handle->L);
            else return true;
        }
        else return false;
    }
}
/**
 * @brief checks if value is -2
 * @return true if value is -2,
 * @return false if value is something other than -2
 */
const bool ALi::is_n2(const Cell* const handle) const{
    if(handle == nullptr){
        if(this->begin_ptr->var == mask110){
            if(this->begin_ptr->L != this->begin_ptr)
                return this->is_n2(this->begin_ptr->L);
            else return true;
        }
        else return false;
    }
    else{
        if(handle->var == mask111){
            if(handle->L != this->begin_ptr)
                return this->is_n2(handle->L);
            else return true;
        }
        else return false;
    }
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
    const Cell* const handle = this->begin_ptr->R; //last cell
    if(handle != this->begin_ptr &&
    ((handle->var == mask111 && handle->R->var & mask100) || (handle->var == mask000 && ~handle->R->var & mask100))){
            this->delCell();
            this->optymize();
    }
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
    this->increment_();
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
 * @brief prints value of the variable in binary format
 */
void ALi::printBinary() const{
    const Cell* handle = this->begin_ptr->R;
    do{
        printf("%s%c", BPrint::binary_x64(handle->var, ULL_VAR_SEP).c_str(),this->separator);
        handle = handle->R;
    }while(handle != this->begin_ptr->R);
}
/**
 * @brief prints value of the variable in decimal format
 */
void ALi::printDecimal() const{
    // if it is negative invert and print '-' sign
    if(this->sign()){
        printf("-");
        ALi tmp(*this);
        tmp.invert();
        tmp.printDecimal();
        return;
    }
    else if(this->is_0()){
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
            const bool old_bcdsign = bcd.sign();
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
                    for(int j=0; j<i; ++j) nibble <<= 4;
                    
                    result |= nibble;
                    ++i;
                }
                bcdhandle->var = result;
            }while(bcdhandle != bcd.begin_ptr);
            if(old_bcdsign != bcd.sign()) bcd.newCell(mask000);
            mask >>= 1;
        }
    }while(handle != this->begin_ptr);

    // print binary-coded decimal
    std::string rev;
    while(bcd.begin_ptr->R->var != 0){ 
        const char nibble = bcd.begin_ptr->R->var & 0b00001111;
        rev = std::string(1,(nibble > 4 ? nibble+45 : nibble+48)) + rev; 
        bcd.begin_ptr->R->var >>= 4;
    }
    printf("%s",rev.c_str());
    bcd.delCell();
    if(bcd.is_0()) return;

    do{
        std::string rev;
        for(int i=0; i<16; ++i){
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
 * @brief prints value of the variable in binary format, using scientific notation
 * @param appPrec precision (how many bytes should be printed after a dot)
 */
void ALi::printBinaryApproximation(ALi appPrec) const{
    if(appPrec.is_0() || appPrec.sign() || this->is_0()){
        // Zero precision? you will get what you want
        printf("0");
        return;
    }
    
    ALi currentValuePrecision(this->length);
    currentValuePrecision.multiplicationAssign(BITS_PER_VAR);
    
    const Cell* handle = this->begin_ptr->R;
    if(handle->var == mask000 || handle->var == mask111){
        currentValuePrecision.subtractionAssign_ext(BITS_PER_VAR);
        handle = handle->R;
    }
    CELL_TYPE mask = mask100;
    while((handle->var & mask ? true : false) == this->sign()){
        mask >>= 1;
        currentValuePrecision.decrement_ext();
    }
    // currentValuePrecision contains msb position

    if(appPrec.greaterThan(currentValuePrecision)){
        printf("current variable contains value which precision is ");
        currentValuePrecision.printDecimal();
        printf(" bits\ncan't get more precision like ");
        appPrec.printDecimal();
        printf(" bits\n");
        return;
    }

    if(appPrec.equal(currentValuePrecision)){
        this->printBinary();
        return;
    }
    
    printf("%s",(handle->var & mask ? "01" : "10"));
    mask >>= 1;
    appPrec.decrement_ext();
    currentValuePrecision.decrement_ext();
    if(!appPrec.is_0()) printf(".");

    while(!appPrec.is_0()){
        appPrec.decrement_ext();
        currentValuePrecision.decrement_ext();
        printf("%c",(handle->var & mask ? '1' : '0'));
        mask >>= 1;
        if(mask == mask000){
            handle = handle->R;
            mask = mask100;
        }
    }
    printf("e+");
    currentValuePrecision.printDecimal();
}
/**
 * @brief prints value of the variable in decimal format, using scientific notation
 * @param appPrec precision (how many bytes should be printed after a dot)
 */
void ALi::printDecimalApproximation(ALi appPrec) const{
    // precision is in default 1 result will be X * 10^n 
    // for 4 result will be XXXX * 10^n
    ALi currentValuePrecision(this->length);
    currentValuePrecision.multiplicationAssign(BITS_PER_VAR);

    if(appPrec.equal(currentValuePrecision)){
        this->printDecimal();
        printf(" * 10^0");
        return;
    }

    if(appPrec.greaterThan(currentValuePrecision)){
        printf("current variable contains value which precision is ");
        // ! the given appPrec will tell about digits no bits
        // currentValuePrecision.printDecimal();
        // printf(" bits\ncan't get more precision like ");
        // appPrec.printDecimal();
        // printf(" bits\n");
        return;
    }

    printf("printDecimalApproximation is not finished yet\n");
}
    // #
    
    // # Store
    
    // #
/**
 * @brief save value of the variable to file in machine code
 * @param path path to file where value of the variable should be saved
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
 * @brief copy value from source (machine code) file to the variable
 * @param path path to file from where value of the variable should be read
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
 * @brief save value of the variable to file in readable binary form with separator sign every 8 bits (using BPrint)
 * @param file already opened file to operate on
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
 * @brief save value of the variable to file in readable decimal form with separator sign every 8 bits (using BPrint)
 * @param file already opened file to operate on
 */
void ALi::writeFile_10(FILE* const file) const{
    printf("writeFile_10 is not finished yet\n");
}
/**
 * @brief save value of the variable to file in readable form with separator sign every 8 bits (using BPrint)
 * @param type in what format value of the variable should be saved [binary/decimal]
 * @param path path to file where value of the variable should be saved
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
 * @brief read value of the variable from file with readable binary form, only '0' and '1' will be read
 * @param file already opened file to operate on
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
 * @brief read value of the variable from file with readable decimal form, only ['0',...,'9'] will be read
 * @param file already opened file to operate on
 */
void ALi::readFile_10(FILE* const file){

}
/**
 * @brief read value of the variable from file with readable form, only significant characters will be interpreted
 * @param type what value of the variable format will be expected [binary/decimal]
 * @param path path to file from where value of the variable should be read
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
 * @brief build current variable using other ALi value
 * @param source ALi variable used as a source
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
 * @brief build current variable using integer value
 * @param source int variable used as a source
 */
void ALi::assignment(const signed long long& source){
    this->clear();
    this->begin_ptr->var = source;
}
    // #
    
    // # Assignment string
    
    // #
/**
 * @brief build current variable using std::strng with binary type value
 * @param source std::string with binary type value used as a source
 */
void ALi::assignment_str_02(std::string source){
    this->clear();

    for(const char c : std::string("123"))
        source.erase(remove(source.begin(), source.end(), c), source.end());

    if(source[0] == '1')
        this->begin_ptr->var = mask111;

    for(char c: source){
        switch(c){
            case '0': this->PLSB(0); break;
            case '1': this->PLSB(1); break;
            default: break;
        }
    }
}
/**
 * @brief build current variable using std::strng with decimal type value
 * @param source std::string with decimal type value used as a source
 */
void ALi::assignment_str_10(std::string source){
    this->clear();
    // if(source[0] == '-')
    
    std::string binarySource;

    int restOfDivision;
    while(source != "1"){
        restOfDivision = 0;
        // printf("%s\n",source.c_str());
        for(char& c: source){
            if(47 < c && c < 58){
                if(c % 2 == 0){
                    c = ((c - 48) / 2 + restOfDivision) + 48;
                    
                    restOfDivision = 0;
                }
                else{
                    c = (((c - 1) - 48) / 2 + restOfDivision) + 48;

                    restOfDivision = 5;
                }
            }
        }

        while(!(48 < source[0] && source[0] < 58)) // if first sign is not '1' - '9'
            source.erase(source.begin());

        // printf("%s - %c",source.c_str(), (restOfDivision ? '1' : '0'));

        binarySource += (restOfDivision ? '1' : '0');
    }
    binarySource += (restOfDivision ? '1' : '0');
    this->assignment_str_02(binarySource);

    // printf("assignment_10 is not finished yet\n");
}
/**
 * @brief build current variable using std::string his with type and value
 * @param type_source std::string containing type (b/d) and value used as a source
 * @example "b010101010101"
 * @example "d765238745629"
 * @example "b1110101010101"
 * @example "d-765238745629"
 */
void ALi::assignment_str(const std::string& type_source){
    switch(type_source[0]){
        case 'b': this->assignment_str_02(type_source.substr(1)); break;
        case 'd': this->assignment_str_10(type_source.substr(1)); break;
        default: printf("print: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",type_source[0]); return;
    }
}
    // #
    
    // # Special
    
    // #    
/**
 * @brief change value to non-negative
 * @return ALi non-negative value of the variable
 */
ALi ALi::absoluteValue() const{
    if(this->sign()){
        ALi out(*this);
        out.invert();
        return out;
    }
    else return *this;
}
/**
 * @brief change value of the variable to non-negative
 */
void ALi::absoluteValueAssign(){
    if(this->sign())
        this->invert();

}
    // #
    
    // # Boolean
    
    // #
/**
 * @brief check if left object is equal to right object
 * @param right ALi object
 * @return true left equal right\
 * @return false left not equal right
 */
const bool ALi::equal(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    if(this->sign() != right.sign()) return false;
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
 * @param right ALi object
 * @return true left is greater\
 * @return false left is not grater
 */
const bool ALi::greaterThan(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    /*
    test if  L > R
        sign gives 1 if is negative
    return + == return 0
    return - == return 1

    if(Lsign != Rsign){
        L+ > R- return true
        L- > R+ return false
        return true if L is positive, false if is not

        L+ > R- return -
        L- > R+ return +
        return Rsign
    }
    else if(Llen > Rlen){
        left is greater if left is longer?
        case where both are positive
            answer is true
        case where both are negative
            answer is false

        return true if L is positive, false if is not
        return !Lsign
    }
    else if(Llen < Rlen){
        left is greater if left is shorter?
        case where both are positive
            answer is false
        case where both are negative
            answer is true

        return false if L is positive, true if is not
        return Lsign
    }
    */
    const bool Lsign = this->sign();
    const bool Rsign = right.sign();
    if(Lsign != Rsign) return Rsign; 
    // signs are equal
    else if(this->length > right.length) return !Lsign;
    else if(this->length < right.length) return Lsign;
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
 * @param right ALi object
 * @return true left is smaller\
 * @return false left is not smaller
 */
const bool ALi::smallerThan(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    /*
    test if  L < R
        sign gives 1 if is negative
    return + == return 0
    return - == return 1

    if(Lsign != Rsign){
        L+ < R- return false
        L- < R+ return true
        return false if L is positive, true if is not

        L+ < R- return +
        L- < R+ return -
        return Lsign
    }
    else if(Llen > Rlen){
        left is smaller if left is longer?
        case where both are positive
            answer is false
        case where both are negative
            answer is true

        return false if L is positive, true if is not
        return Lsign
    }
    else if(Llen < Rlen){
        left is smaller if left is shorter?
        case where both are positive
            answer is true
        case where both are negative
            answer is false

        return true if L is positive, false if is not
        return !Lsign
    }
    */
    const bool Lsign = this->sign();
    const bool Rsign = right.sign();
    if(Lsign != Rsign) return Lsign;
    // signs are equal
    else if(this->length > right.length) return Lsign;
    else if(this->length < right.length) return !Lsign;
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
 * @brief increment value of the variable by one
 */
void ALi::increment_(){
    Cell *handle = this->begin_ptr;
    while(handle->var == mask111 && handle != this->begin_ptr->R){
        handle->var = mask000; // 11111111 -> (1)00000000
        handle = handle->L;
    }
    ++handle->var;
}
/**
 * @brief increment value of the variable by one
 */
void ALi::increment_ext(){
    // incrementing binary is acctually easy:
    // going through each bit from right
    // if is 1 then set to 0
    // if is 0 then set to 1 exit
    
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
    if(!this->sign() && handle->var == mask011 && handle == this->begin_ptr->R){
        this->newCell(mask000);
    }
    // 10101101  11111111  11111111  11111111  11111111 -> [10101110] 00000000  00000000  00000000  00000000
    // 11111111                                         -> [00000000]
    // 10101101  11111111  11100011  11111111  11111111 ->  10101101  11111111 [11100100] 00000000  00000000
    // 00000010                                         -> [00000011]
    // 01111111  11111111                               ->  00000000 [10000000] 00000000
    // 01111111  11110011                               ->  01111111 [11110100]
    ++handle->var;
    this->optymize();
}
/**
 * @brief adding up two ALi variable values
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 * @return ALi object
 */
ALi ALi::addition_(const ALi& right) const{
    if(right.is_0()){ // L + 0 = L
        return *this;
    }
    else if (right.is_p1()){ // L + 1 = ++L
        ALi out(*this);
        out.increment_ext();
        return out;
    }
    else if (right.is_n1()){ // L + -1 = --L
        ALi out(*this);
        out.decrement_ext();
        return out;
    }
    else if (this->is_0()){ // 0 + R = R
        return right;
    }
    else if (this->is_p1()){ // 1 + R = ++R
        ALi out(right);
        out.increment_ext();
        return out;
    }
    else if (this->is_n1()){ // -1 + R = --R
        ALi out(right);
        out.decrement_ext();
        return out;
    }

    
    ALi out;

    const Cell* const lgh = this->begin_ptr;
    const Cell* lh = lgh->L;
    Cell lmask{
        lmask.var = (this->sign() ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const Cell* const rgh = right.begin_ptr;
    const Cell* rh = rgh->L;
    Cell rmask{
        rmask.var = (right.sign() ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };

    unsigned char carry = __builtin_add_overflow(lgh->var, rgh->var, &out.begin_ptr->var);
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    CELL_TYPE sum;
    while(lh != &lmask || rh != &rmask){
        carry  = __builtin_add_overflow(lh->var,carry,&sum);
        carry += __builtin_add_overflow(rh->var,sum  ,&sum);
        out.newCell(sum);

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;
    }
    return out;
}
/**
 * @brief adding up two ALi variable values
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 * @return ALi object
 */
ALi ALi::addition_ext(const ALi& right) const{
    if(right.is_0()){ // L + 0 = L
        return *this;
    }
    else if (right.is_p1()){ // L + 1 = ++L
        ALi out(*this);
        out.increment_ext();
        return out;
    }
    else if (right.is_n1()){ // L + -1 = --L
        ALi out(*this);
        out.decrement_ext();
        return out;
    }
    else if (this->is_0()){ // 0 + R = R
        return right;
    }
    else if (this->is_p1()){ // 1 + R = ++R
        ALi out(right);
        out.increment_ext();
        return out;
    }
    else if (this->is_n1()){ // -1 + R = --R
        ALi out(right);
        out.decrement_ext();
        return out;
    }

    
    ALi out;

    const Cell* const lgh = this->begin_ptr;
    const Cell* lh = lgh->L;
    Cell lmask{
        lmask.var = (this->sign() ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const Cell* const rgh = right.begin_ptr;
    const Cell* rh = rgh->L;
    Cell rmask{
        rmask.var = (right.sign() ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };

    unsigned char carry = __builtin_add_overflow(lgh->var, rgh->var, &out.begin_ptr->var);
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    CELL_TYPE sum;
    while(lh != &lmask || rh != &rmask){
        carry  = __builtin_add_overflow(lh->var,carry,&sum);
        carry += __builtin_add_overflow(rh->var,sum  ,&sum);
        out.newCell(sum);

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;
    }
    // overflow can only appear when both operation argument sign values are equal
    if(this->sign() == right.sign() && this->sign() != out.sign())
        out.newCell(lmask.var);
    out.optymize();
    return out;
}
/**
 * @brief adding up two ALi variable values and assigns to the current one
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 */
void ALi::additionAssign_(const ALi& right){
    if(right.is_0()){ // L += 0 == L
        return;
    }
    else if (right.is_p1()){ // L += 1 == ++L
        this->increment_ext();
        return;
    }
    else if (right.is_n1()){ // L += -1 == --L
        this->decrement_ext();
        return;
    }
    else if (this->is_0()){ // 0 += R == R
        this->assignment(right);
        return;
    }
    else if (this->is_p1()){ // 1 += R == ++R
        this->assignment(right);
        this->increment_ext();
        return;
    }
    else if (this->is_n1()){ // -1 += R == --R
        this->assignment(right);
        this->decrement_ext();
        return;
    }
    
    // this->assignment(this->addition(right));

    ALi* const lobj = this; 
    Cell* const lgh = lobj->begin_ptr;
    const bool lsign = lobj->sign(); // cause it could be changed
    Cell* lh = lgh->L;
    Cell lmask{
        lmask.var = (lsign ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const ALi* const robj = &right;
    const Cell* const rgh = robj->begin_ptr;
    const Cell* rh = rgh->L;
    Cell rmask{
        rmask.var = (robj->sign() ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };

    unsigned char carry = __builtin_add_overflow(lgh->var, rgh->var, &lgh->var);
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    CELL_TYPE sum;
    while(lh != &lmask || rh != &rmask){
        carry  = __builtin_add_overflow(lh->var,carry,&sum);
        carry += __builtin_add_overflow(rh->var,sum  ,&sum);
        if(lh == &lmask)
            lobj->newCell(sum);
        else
            lh->var = sum;

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;

    }
}
/**
 * @brief adding up two ALi variable values and assigns to the current one
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 */
void ALi::additionAssign_ext(const ALi& right){
    if(right.is_0()){ // L += 0 == L
        return;
    }
    else if (right.is_p1()){ // L += 1 == ++L
        this->increment_ext();
        return;
    }
    else if (right.is_n1()){ // L += -1 == --L
        this->decrement_ext();
        return;
    }
    else if (this->is_0()){ // 0 += R == R
        this->assignment(right);
        return;
    }
    else if (this->is_p1()){ // 1 += R == ++R
        this->assignment(right);
        this->increment_ext();
        return;
    }
    else if (this->is_n1()){ // -1 += R == --R
        this->assignment(right);
        this->decrement_ext();
        return;
    }
    
    // this->assignment(this->addition(right));

    ALi* const lobj = this; 
    Cell* const lgh = lobj->begin_ptr;
    const bool lsign = lobj->sign(); // cause it could be changed
    Cell* lh = lgh->L;
    Cell lmask{
        lmask.var = (lsign ? mask111 : mask000),
        lmask.L = &lmask,
        lmask.R = &lmask
    };

    const ALi* const robj = &right;
    const Cell* const rgh = robj->begin_ptr;
    const Cell* rh = rgh->L;
    Cell rmask{
        rmask.var = (robj->sign() ? mask111 : mask000),
        rmask.L = &rmask,
        rmask.R = &rmask
    };

    unsigned char carry = __builtin_add_overflow(lgh->var, rgh->var, &lgh->var);
    if(lh == lgh) lh = &lmask;
    if(rh == rgh) rh = &rmask;

    CELL_TYPE sum;
    while(lh != &lmask || rh != &rmask){
        carry  = __builtin_add_overflow(lh->var,carry,&sum);
        carry += __builtin_add_overflow(rh->var,sum  ,&sum);
        if(lh == &lmask)
            lobj->newCell(sum);
        else
            lh->var = sum;

        lh = lh->L;
        rh = rh->L;
        if(lh == lgh) lh = &lmask;
        if(rh == rgh) rh = &rmask;

    }
    // overflow can only appear when both operation argument sign values are equal
    if(lsign == robj->sign() && lsign != this->sign())
        this->newCell(lmask.var);
    this->optymize();
}
    // #
    
    // # Subtraction
    
    // #
/**
 * @brief decrement value of the variable by one
 */
void ALi::decrement_(){
    Cell *handle = this->begin_ptr;
    while(handle->var == mask000 && handle != this->begin_ptr->R){
        handle->var = mask111; // (1)00000000 -> (0)11111111
        handle = handle->L;
    }
    handle->var--;
}
/**
 * @brief decrement value of the variable by one
 */
void ALi::decrement_ext(){
    Cell *handle = this->begin_ptr;
    while(handle->var == mask000 && handle != this->begin_ptr->R){
        handle->var = mask111; // (1)00000000 -> (0)11111111
        handle = handle->L;
    }
    // if number is negative, overflow can occur cause we are adding two negative numbers
    if(this->sign() && (handle->var == mask100 && handle == this->begin_ptr->R)){
        this->newCell(mask111);
    }
    handle->var--;
    this->optymize();
}
/**
 * @brief subtracting ALi variable value fron the current variable value
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 * @return ALi object
 */
ALi ALi::subtraction_(const ALi& right) const{
    if(right.is_0()){ // L - 0 = L
        return *this;
    }
    else if(right.is_p1()){ // L - 1 = --L
        ALi out(*this);
        out.decrement_ext();
        return out;
    }
    else if(right.is_n1()){ // L - -1 = ++L
        ALi out(*this);
        out.increment_ext();
        return out;
    }
    else if(this->is_0()){ // 0 - R = -R
        ALi out(right);
        out.invert();
        return out;
    }
    else if(this->is_p1()){ // 1 - R = -(--R)
        ALi out(right);
        out.decrement_ext();
        out.invert();
        return out;
    }
    else if(this->is_n1()){ // -1 - R = -(++R)
        ALi out(right);
        out.increment_ext();
        out.invert();
        return out;
    }
    else if(this->equal(right)){ // L - R = 0    L==R
        return ALi(0);
    }

    //############################# not large impact to efficiency (+x)-(+y) == (+x)+(-y)
    ALi right_(right);
    right_.invert();

    return this->addition_(right_);
    //#############################
}
/**
 * @brief subtracting ALi variable value fron the current variable value
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 * @return ALi object
 */
ALi ALi::subtraction_ext(const ALi& right) const{
    if(right.is_0()){ // L - 0 = L
        return *this;
    }
    else if(right.is_p1()){ // L - 1 = --L
        ALi out(*this);
        out.decrement_ext();
        return out;
    }
    else if(right.is_n1()){ // L - -1 = ++L
        ALi out(*this);
        out.increment_ext();
        return out;
    }
    else if(this->is_0()){ // 0 - R = -R
        ALi out(right);
        out.invert();
        return out;
    }
    else if(this->is_p1()){ // 1 - R = -(--R)
        ALi out(right);
        out.decrement_ext();
        out.invert();
        return out;
    }
    else if(this->is_n1()){ // -1 - R = -(++R)
        ALi out(right);
        out.increment_ext();
        out.invert();
        return out;
    }
    else if(this->equal(right)){ // L - R = 0    L==R
        return ALi(0);
    }

    //############################# not large impact to efficiency (+x)-(+y) == (+x)+(-y)
    ALi right_(right);
    right_.invert();

    return this->addition_ext(right_);
    //#############################
}
/**
 * @brief subtracting ALi variable value fron the current variable value and assigns to the current one
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 */
void ALi::subtractionAssign_(const ALi& right){
    if(right.is_0()){ // L -= 0 == L
        return;
    }
    else if(right.is_p1()){ // L -= 1 == --L
        this->decrement_ext();
        return;
    }
    else if(right.is_n1()){ // L -= -1 == ++L
        this->increment_ext();
        return;
    }
    else if(this->is_0()){ // 0 -= R == -R
        this->assignment(right);
        this->invert();
        return;
    }
    else if(this->is_p1()){ // 1 -= R == -(--R)
        this->assignment(right);
        this->decrement_ext();
        this->invert();
        return;
    }
    else if(this->is_n1()){ // -1 -= R == -(++R)
        this->assignment(right);
        this->increment_ext();
        this->invert();
        return;
    }
    else if(this->equal(right)){ // L -= R == 0    L==R
        this->clear();
        return;
    }
    
    //############################# not large impact to efficiency (+x)-=(+y) == (+x)+=(-y)
    ALi right_(right);
    right_.invert();

    this->additionAssign_(right_);
    //#############################
}
/**
 * @brief subtracting ALi variable value fron the current variable value and assigns to the current one
 * @param right ALi object
 * @param handle_overflow ( = true) boolean variable describe what to do when overflow occur (true: increase length of variable / false: keep length fixed and switch value sign)
 */
void ALi::subtractionAssign_ext(const ALi& right){
    if(right.is_0()){ // L -= 0 == L
        return;
    }
    else if(right.is_p1()){ // L -= 1 == --L
        this->decrement_ext();
        return;
    }
    else if(right.is_n1()){ // L -= -1 == ++L
        this->increment_ext();
        return;
    }
    else if(this->is_0()){ // 0 -= R == -R
        this->assignment(right);
        this->invert();
        return;
    }
    else if(this->is_p1()){ // 1 -= R == -(--R)
        this->assignment(right);
        this->decrement_ext();
        this->invert();
        return;
    }
    else if(this->is_n1()){ // -1 -= R == -(++R)
        this->assignment(right);
        this->increment_ext();
        this->invert();
        return;
    }
    else if(this->equal(right)){ // L -= R == 0    L==R
        this->clear();
        return;
    }
    
    //############################# not large impact to efficiency (+x)-=(+y) == (+x)+=(-y)
    ALi right_(right);
    right_.invert();

    this->additionAssign_ext(right_);
    //#############################
}
    // #
    
    // # Multiplication
    
    // #
/**
 * @brief multiplies current variable value by ALi variable value
 * @param right ALi object
 * @return ALi object
 */
ALi ALi::multiplication(const ALi& right) const{
    if(right.is_0()){ // L * 0 = 0
        return 0;
    }
    else if(right.is_p1()){ // L * 1 = L
        return *this;
    }
    else if(right.is_p2()){ // L * 2 = L.shl_ext()
        ALi out(*this);
        out.shl_ext();
        return out;
    }
    else if(right.is_n1()){ // L * -1 = -L
        ALi out(*this);
        out.invert();
        return out;
    }
    else if(right.is_n2()){ // L * -2 = -L.shl_ext()
        ALi out(*this);
        out.shl_ext();
        out.invert();
        return out;
    }
    else if(this->is_0()){ // 0 * R = 0
        return 0;
    }
    else if(this->is_p1()){ // 1 * R = R
        return right;
    }
    else if(this->is_p2()){ // 2 * R = R.shl_ext()
        ALi out(right);
        out.shl_ext();
        return out;
    }
    else if(this->is_n1()){ // -1 * R = -R
        ALi out(right);
        out.invert();
        return out;
    }
    else if(this->is_n2()){ // -2 * R = -R.shl_ext()
        ALi out(right);
        out.shl_ext();
        out.invert();
        return out;
    }

    ALi slider(*this);
    ALi factor;

    slider.PLSB(0); // comparison bit

    // align
    while(factor.length < slider.length)
        factor.newCell(mask000);
    
    const Cell* rhandle = right.begin_ptr;
    do{
        factor.newCell(rhandle->var);
        rhandle = rhandle->L;
    }while(rhandle != right.begin_ptr);

    unsigned long long slider_length = slider.length * BITS_PER_VAR - 1;
    unsigned long long i=0;
    while(i < slider_length){
        switch (slider.begin_ptr->var & 0b11){
        case 1: // 01
            slider.additionAssign_(factor);
            break;
        case 2: // 10
            slider.subtractionAssign_(factor);
            break;
        // 00 & 11
        }
        slider.shr_ext();
        ++i;
    }
    slider.shr_ext();

    // idk why for negative left, do not work and needs +1
    if(this->sign()) slider.increment_ext();

    return slider;
}
/**
 * @brief multiplies current variable value by ALi variable value and assigns to the current one
 * @param right ALi object
 */
void ALi::multiplicationAssign(const ALi& right){
    if(right.is_0()){ // L *= 0 == 0
        this->clear();
        return;
    }
    else if(right.is_p1()){ // L *= 1 == L
        return;
    }
    else if(right.is_p2()){ // L *= 2 == L.shl_ext()
        this->shl_ext();
        return;
    }
    else if(right.is_n1()){ // L *= -1 == -L
        this->invert();
        return;
    }
    else if(right.is_n2()){ // L *= -2 == -L.shl_ext()
        this->shl_ext();
        this->invert();
        return;
    }
    else if(this->is_0()){ // 0 *= R == 0
        return;
    }
    else if(this->is_p1()){ // 1 *= R == R
        this->assignment(right);
        return;
    }
    else if(this->is_p2()){ // 2 *= R == R.shl_ext()
        this->assignment(right);
        this->shl_ext();
        return;
    }
    else if(this->is_n1()){ // -1 *= R == -R
        this->assignment(right);
        this->invert();
        return;
    }
    else if(this->is_n2()){ // -2 *= R == -R.shl_ext()
        this->assignment(right);
        this->shl_ext();
        this->invert();
        return;
    }
    else if(this == &right){ // L and R are the same object
        ALi left_cpy(right);
        this->multiplicationAssign(left_cpy);
        return;
    }

    ALi factor;
    const bool lsign = this->sign();

    this->PLSB(0); // comparison bit

    // align
    while(factor.length < this->length)
        factor.newCell(mask000);
    
    const Cell* rhandle = right.begin_ptr;
    do{
        factor.newCell(rhandle->var);
        rhandle = rhandle->L;
    }while(rhandle != right.begin_ptr);

    unsigned long long slider_length = this->length * BITS_PER_VAR - 1;
    unsigned long long i=0;
    while(i < slider_length){
        switch (this->begin_ptr->var & 0b11){
        case 1: // 01
            this->additionAssign_(factor);
            break;
        case 2: // 10
            this->subtractionAssign_(factor);
            break;
        // 00 & 11
        }
        this->shr_ext();
        ++i;
    }
    this->shr_ext();

    // idk why for negative left, do not work and needs +1
    if(lsign) this->increment_ext();
}
    // #
    
    // # Division

    // #
/**
 * @brief divides current variable value by ALi variable value, rounds to integer
 * @param right ALi object
 * @return ALi object
 */
ALi ALi::division(const ALi& right) const{
    if(right.is_0()){ // L / 0 = !
        printf("Zero division!\nreturned: 0\n");
        return 0;
    }
    else if(right.is_p1()){ // L / 1 = L
        return *this;
    }
    else if(right.is_p2()){ // L / 2 = L.shr_ext()
        ALi out(*this);
        out.shr_ext();
        return out;
    }
    else if(right.is_n1()){ // L / -1 = -L
        ALi out(*this);
        out.invert();
        return out;
    }
    else if(right.is_n2()){ // L / -2 = -L.shr_ext()
        ALi out(*this);
        out.shr_ext();
        out.invert();
        return out;
    }
    else if(this->is_0()){ // 0 / R = 0
        return 0;
    }
    else if(this->absoluteValue().equal(right.absoluteValue())){ // |L / R| = 1    |L| == |R| 
        if(this->sign() == right.sign())                         // L / R = 1      L == R
            return 1;
        else                                                     // L / R = -1     L == -(R)
            return -1;
    }
    else if(this->absoluteValue().smallerThan(right.absoluteValue())){ // L / R = 0   L < R
        return 0;
    }


    ALi slider;
    ALi factor(right);
    factor.invert();
    ALi out;
    const Cell* hdl = this->begin_ptr->R;
    do{
        unsigned long long mask = mask100;
        do{
            if(hdl->var & mask) slider.PLSB(1);
            else                slider.PLSB(0);
            if(!slider.smallerThan(right)){
                slider.additionAssign_(factor);
                out.PLSB(1);
            }
            else 
                out.PLSB(0);
            mask >>= 1;
        }while(mask > 0);
        hdl = hdl->R;
    }while(hdl != this->begin_ptr);



    // ALi slider(*this);
    // ALi out;
    
    // while(!slider.is_0()){
    //     slider.optymize();
    //     slider.newCell(mask000);
    //     unsigned long long len = (slider.length-1) * 8;
    //     for(int i=0; i<len; ++i){
    //         if(slider.begin_ptr->R->var >= 10){
    //             slider.begin_ptr->R->var -= 10;
    //             slider.PLSB(mask001);
    //         }
    //         else slider.PLSB(mask000);
    //     }
    //     if(slider.begin_ptr->R->var >= 10){
    //         out.additionAssign(slider.begin_ptr->R->var - 10);
    //         slider.PLSB(mask001);
    //     } 
    //     else{
    //         out.additionAssign(slider.begin_ptr->R->var);
    //         slider.PLSB(mask000);
    //     }
    //     slider.delCell();
    // }`

//*    PART FROM OLD PRINT DECIMAL METHOD
//*    I CAN REUSE DIVISION ALGORITHM
//
//     ALi slider(*this);
//     slider.setSeparator(' ');
//     std::string decimal = "";
//     unsigned long long bitlength = 8;
//     while(slider.begin_ptr.var != 0 || slider.length != 1){
//         slider.optymize();
//         slider.newCell(0);
//         bitlength = (slider.length-1) * 8;
//         for(int i=0; i<bitlength; ++i){
//             if(slider.begin_ptr.R->var >= 10){
//                 slider.begin_ptr.R->var -= 10;
//                 slider.PLSB(1);
//             }
//             else slider.PLSB(0);
//         }
//         if(slider.begin_ptr.R->var >= 10){
//             decimal += (char)(slider.begin_ptr.R->var - 10);
//             slider.PLSB(1);
//         } 
//         else{
//             decimal += (char)(slider.begin_ptr.R->var);
//             slider.PLSB(0);
//         }
//         slider.delCell();
//     }

    return out;
}
/**
 * @brief divides current variable value by ALi variable value, rounds to integer and assigns to the current one
 * @param right ALi object
 */
void ALi::divisionAssign(const ALi& right){
    if(right.is_0()){ // L /= 0 == !
        printf("Zero division!\nassigned: 0\n");
        this->clear();
        return;
    }
    else if(right.is_p1()){ // L /= 1 == L
        return;
    }
    else if(right.is_p2()){ // L /= 2 == L.shr_ext()
        this->shr_ext();
        return;
    }
    else if(right.is_n1()){ // L /= -1 == -L
        this->invert();
        return;
    }
    else if(right.is_n2()){ // L /= -2 == -L.shr_ext()
        this->shr_ext();
        this->invert();
        return;
    }
    else if(this->is_0()){ // 0 /= R == 0
        this->clear();
        return;
    }
    else if(this->absoluteValue().equal(right.absoluteValue())){ // |L /= R| == 1    |L| == |R| 
        if(this->sign() == right.sign()){                        // L /= R == 1      L == R
            this->assignment(1);
            return;
        }
        else{                                                    // L /= R == -1     L == -(R)
            this->assignment(-1);
            return;
        }
    }
    else if(this->absoluteValue().smallerThan(right.absoluteValue())){ // L /= R == 0   L < R
        this->clear();
        return;
    }
    
    // There is possibility to use only factor and *this objects but to do so i need to use "for" what i really don't want to do because of iterator limits.
    // this variable msb could be used to slider.plsb(...)
    // check if slider is larger or equal to factor, \
        if so add factor to slider (do it without overflow handle actions) and then this->plsb(1)\
        else this->plsb(0)
    // after plsb operation, this variable length need to keep the same lenght as before (no mather if overflow occur)
    // to avoid useless + and - cell operations i should implement //! PLSB() method without overflow handle and change current PLSB() method name to something like PLSB_hdl_ofl()
    // problem is that algorithm need to be executed (number of bits) times 
    ALi slider;
    ALi factor(right);
    factor.invert();
    ALi out;
    const Cell* hdl = this->begin_ptr->R;
    do{
        unsigned long long mask = mask100;
        do{
            if(hdl->var & mask) slider.PLSB(1);
            else                slider.PLSB(0);
            if(!slider.smallerThan(right)){
                slider.additionAssign_(factor);
                out.PLSB(1);
            }
            else 
                out.PLSB(0);
            mask >>= 1;
        }while(mask > 0);
        hdl = hdl->R;
    }while(hdl != this->begin_ptr);
    this->assignment(out);
}
    // #
    
    // # Modulo

    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::modulo(const ALi& right) const{
    printf("modulo is not finished yet\n");
    if(right.is_0()){ // L % 0 = !
        printf("Zero division!\nreturned: 0\n");
        return 0;
    }
    else if(this->is_0()){ // 0 % R = 0
        return 0;
    }
    else if(right.is_p1()){ // L % 1 = 0
        return 0;
    }
    else if(right.is_n1()){ // L % -1 = 0
        return 0;
    }
    else if(right.is_p2()){ // L % 2 = L.LSB()
        return this->begin_ptr->var & mask001;
    }
    else if(right.is_n2()){ // L % -2 = L.LSB()
        return this->begin_ptr->var & mask001;
    }
    else if(!this->absoluteValue().greaterThan(right.absoluteValue())){ // |L| < |R| => L % R = L
        return *this;
    }

    ALi slider;
    ALi factor(right);
    factor.invert();
    const Cell* hdl = this->begin_ptr->R;
    do{
        unsigned long long mask = mask100;
        do{
            if(hdl->var & mask) slider.PLSB(1);
            else                slider.PLSB(0);
            if(!slider.smallerThan(right)){
                slider.additionAssign_(factor);
            }
            mask >>= 1;
        }while(mask > 0);
        hdl = hdl->R;
    }while(hdl != this->begin_ptr);

    return slider;
}
/**
 * @brief 
 * @param right 
 */
void ALi::moduloAssign(const ALi& right){
    printf("moduloAssign is not finished yet\n");
    if(right.is_0()){ // L %= 0 == !
        printf("Zero division!\nassigned: 0\n");
        this->clear();
        return;
    }
    else if(this->is_0()){ // 0 %= R == 0
        return;
    }
    else if(right.is_p1()){ // L %= 1 == 0
        this->clear();
        return;
    }
    else if(right.is_n1()){ // L %= -1 == 0
        this->clear();
        return;
    }
    else if(right.is_p2()){ // L %= 2 == L.LSB()
        this->assignment(this->begin_ptr->var & mask001);
        return;
    }
    else if(right.is_n2()){ // L %= -2 == L.LSB()
        this->assignment(this->begin_ptr->var & mask001);
        return;
    }
    else if(!this->absoluteValue().greaterThan(right.absoluteValue())){ // |L| < |R| => L %= R == L
        return;
    }


    // just like divisionAssign method, there is a way by with what cost :c 
    // if i will find better divisionAssign algorithm then i can change this moduloAssign algorithm

    ALi slider;
    ALi factor(right);
    factor.invert();
    const Cell* hdl = this->begin_ptr->R;
    do{
        unsigned long long mask = mask100;
        do{
            if(hdl->var & mask) slider.PLSB(1);
            else                slider.PLSB(0);
            if(!slider.smallerThan(right)){
                slider.additionAssign_(factor);
            }
            mask >>= 1;
        }while(mask > 0);
        hdl = hdl->R;
    }while(hdl != this->begin_ptr);

}
    // #
    
    // # Exponentiation

    // #
/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::exponentiation(const ALi& right) const{
    if(this->is_0()){ // 0 ^ R = 0
        return 0;
    }
    else if(this->is_p1()){ // 1 ^ R = 1
        return 1;
    }
    else if(this->is_n1()){ // R,LSB() = 0 => -1 ^ R = 1    R.LSB() = 1 => -1 ^ R = -1    
        if(right.begin_ptr->var & mask001){ 
            return -1;
        }
        else{
            return 1;
        }
    }
    else if(right.is_0()){ // L ^ 0 = 1
        return 1;
    }
    else if(right.is_p1()){ // L ^ 1 = L
        return *this;
    }
    else if(right.is_p2()){ // L ^ 2 = L * L
        return this->multiplication(*this);
    }
    else if(right.sign()){ // L ^ R = 0     R < 0
        return 0;
    }
    
    ALi out(*this);
    ALi notEvenOut(1);
    ALi exponent(right);
    while(!exponent.is_p1()){
        if(exponent.begin_ptr->var & mask001){
            notEvenOut.multiplicationAssign(out);
            exponent.decrement_ext();
        }
        out.multiplicationAssign(out);
        exponent.shr_ext();
    }
    out.multiplicationAssign(notEvenOut);
    return out;
}
/**
 * @brief 
 * @param right 
 */
void ALi::exponentiationAssign(const ALi& right){
    if(this->is_0()){ // 0 ^= R == 0
        return;
    }
    else if(this->is_p1()){ // 1 ^= R == 1
        return;
    }
    else if(this->is_n1()){ // R.LSB() = 0 => -1 ^= R == 1    R.LSB() = 1 => -1 ^= R == -1    
        if(right.begin_ptr->var & mask001){ 
            return;
        }
        else{
            this->invert();
            return;
        }
    }
    else if(right.is_0()){ // L ^= 0 == 1
        this->assignment(1);
        return;
    }
    else if(right.is_p1()){ // L ^= 1 == L
        return;
    }
    else if(right.is_p2()){ // L ^= 2 == L * L
        this->multiplicationAssign(*this);
        return;
    }
    else if(right.sign()){ // L ^= R == 0     R < 0
        this->clear();
        return;
    }

    ALi notEvenOut(1);
    ALi exponent(right);
    while(!exponent.is_p1()){
        // out >> "d X\n";
        if(exponent.begin_ptr->var & mask001){
            notEvenOut.multiplicationAssign(*this);
            exponent.decrement_ext();
        }
        this->multiplicationAssign(*this);
        exponent.shr_ext();
    }
    this->multiplicationAssign(notEvenOut);
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
 * @brief print variable as a binary or decimal (also with approximations)\
 * @brief print("d\n");\
 * @brief print("b\n");\
 * @brief print("ad\n");\
 * @brief print("ab\n");\
 * @brief print("a12d\n");\
 * @brief print("a12b\n");
 * @param additionText type and text what will be printed at the end of variable
 */
void ALi::print(const char* type_text) const{
    switch (*type_text){
        case 'b': this->printBinary(); break;
        case 'd': this->printDecimal(); break;
        case 'a':{
            type_text++;
            std::string number;
            while('0' <= *type_text && *type_text <= '9'){
                number += *type_text;
                type_text++;
            }
            switch (*type_text){
                case 'b': this->printBinaryApproximation(std::atoi(number.c_str())); break;
                case 'd': this->printDecimalApproximation(std::atoi(number.c_str())); break;
                default: printf("print: unknown approximation type: '%c'\nbinary: 'b'\n decimal: 'd'\n",*type_text); return;
            }
        }
        default: printf("print: unknown type: '%c'\nbinary: 'b'\n decimal: 'd'\n",*type_text); return;
    }
    while(*type_text != '\0'){
        type_text++;
        printf("%c",*type_text);
    }
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
    return this->is_0();
}
    // #
    
    // # Operators
    
    // #
/**
 * @brief print() shortcut \
 * @brief print variable as a binary or decimal (also with approximations)\
 * @brief >> "d\n";\
 * @brief >> "b\n";\
 * @brief >> "ad\n";\
 * @brief >> "ab\n";\
 * @brief >> "a12d\n";\
 * @brief >> "a12b\n";
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
    this->increment_ext();
    return out;
}
ALi  ALi::operator ++ (){
    this->increment_ext();
    return *this;
}
ALi  ALi::operator +  (const ALi& right) const{
    return this->addition_ext(right);
}
void ALi::operator += (const ALi& right){
    this->additionAssign_ext(right);
}

ALi  ALi::operator -- (int){
    this->decrement_ext();
    return *this;
}
ALi  ALi::operator -- (){
    this->decrement_ext();
    return *this;
}
ALi  ALi::operator -  (const ALi& right) const{
    return this->subtraction_ext(right);
}
void ALi::operator -= (const ALi& right){
    this->subtractionAssign_ext(right);
}

ALi  ALi::operator *  (const ALi& right) const{
    return this->multiplication(right);
}
void ALi::operator *= (const ALi& right){
    this->multiplicationAssign(right);
}

ALi  ALi::operator /  (const ALi& right) const{
    return this->division(right);
}
void ALi::operator /= (const ALi& right){
    this->divisionAssign(right);
}

ALi  ALi::operator %  (const ALi& right) const{
    return this->modulo(right);
}
void ALi::operator %= (const ALi& right){
    this->moduloAssign(right);
}

ALi  ALi::operator ^  (const ALi& right) const{
    return this->exponentiation(right);
}
void ALi::operator ^= (const ALi& right){
    this->exponentiationAssign(right);
}
