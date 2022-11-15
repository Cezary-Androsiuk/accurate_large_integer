#include "AccurateLargeInteger.h"



/**
 * @brief Construct a new ALi object
 */
ALi::ALi(){
    // this->initialize();
    this->length = 1;
    this->globalHandle.L = &this->globalHandle;
    this->globalHandle.R = &this->globalHandle;
    this->globalHandle.var = 0;
    this->separator = '\0';
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
    this->assignment(source);
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
}










/**
 * @brief create new cell from the left side
 * @param standardSource: unsigned long long value that will be used to optional assigned value to new cell
 */
void ALi::newCell(const unsigned char &standardSource){
    Cell* handle = new Cell;
    handle->L = &this->globalHandle;
    handle->R = this->globalHandle.R;
    this->globalHandle.R->L = handle;
    this->globalHandle.R = handle;
    handle->var = static_cast<unsigned char>(standardSource);
    this->length++;
}

/**
 * @brief remove cell from the left, not including this->globalHandle
 * @return 1: if there are more cells that can be deleted
 * @return 0: if there are no more cells that can be deleted
 */
const bool ALi::delCell(){
    // not single dynamically alocated cell
    if(this->globalHandle.R == &this->globalHandle){
        // set value to 0 
        this->globalHandle.var = 0;
        return false;
    }
    // n dynamically alocated cells where n >= 1
    else{
        Cell* handle = this->globalHandle.R;
        handle->R->L = &this->globalHandle;
        this->globalHandle.R = handle->R;
        delete handle;
    }
    this->length--;
    return true;
}


// /**
//  * @brief create new cell from the right side
//  * only for readFileReadable
//  * @param standardSource: unsigned long long value that will be used to optional assigned value to new cell
//  */
// void ALi::newCellRight(UC standardSource){
//     // user choose if we need to add cell with zeros or ones if negative
//     Cell* handle = new Cell;
//     handle->R = &this->globalHandle;
//     handle->L = this->globalHandle.L;
//     this->globalHandle.L->R = handle;
//     this->globalHandle.L = handle;
//     handle->var = standardSource;
//     this->length++;
// }

// /**
//  * @brief remove cell from the right, not including this->globalHandle
//  * only for readFileReadable
//  * @return 1: if there are more cells that can be deleted
//  * @return 0: if there are no more cells that can be deleted
//  */
// const bool ALi::delCellRight(){
//     // not single dynamically alocated cell
//     if(this->globalHandle.R == &this->globalHandle){
//         // set value to 0 
//         this->globalHandle.var = 0;
//         return false;
//     }
//     // n dynamically alocated cells where n >= 1
//     else{
//         Cell* handle = this->globalHandle.L;
//         handle->L->R = &this->globalHandle;
//         this->globalHandle.L = handle->L;
//         delete handle;
//     }
//     this->length--;
//     return true;
// }










/**
 * @brief finding an index of the most significant bit
 * @return const _ULL: index of the most significant bit
 */
const unsigned long long ALi::MSB() const{
    printf("MSB is not finished\n");
    return -1;
    // if(this->globalHandle.R->var >= 0x80){ // is negative

    // }
    // else{ // is positive
    
    // }
    // // ! WAHT IF IS NEGATIVE 
    // _ULL index = 0;
    // while(this->beg.var != 0 || this->beg.L != &this->end){
    //     this->SHR();
    //     index++;
    // }
    // return index;
}

/**
 * @brief execute shift right operation
 */
void ALi::SHR(){
    // if there are no cells ok if there more cells ok cause allways start from the most left cell
    Cell* handle = this->globalHandle.R;
    char buffer[2] = {0,0};
    
    if(this->globalHandle.R->var >= 0b10000000) // is negative so the most left bit will be 1
        buffer[1] = 1;
    do{
        /*
            mov LSB to buffer[0]
            SHR
            set MSB to buffer[1]
            mov buffer[0] to buffer[1]
            set handle to cell on the right side
        */
        buffer[0] = handle->var & 0b00000001; // hold right bit of a cell
        handle->var >>= 1; // shr on cell
        // use buffer to assign most left bit
        // buffer can be 0b00000000(false) or 0b00000001(true)
        // use property of | and & operators 
        // | - where is 1 there will be allways 1 where 0 there nothing will change in those bits
        // & - where is 0 there will be allways 0 where 1 there nothing will change in those bits
        if(buffer[1]) handle->var |= 0b10000000;
        else handle->var &= 0b01111111;
        buffer[1] = buffer[0]; // hold bit to next cell
        handle = handle->R;
    } while (handle != this->globalHandle.R);
    

}

/**
 * @brief execute shift left operation
 */
void ALi::SHL(){
    // if there are no cells ok if there more cells ok cause allways start from the most right cell
    Cell* handle = &this->globalHandle; 
    char buffer[2] = {0,0};
    // where most left bit after SHL change their value then overflow has occurred
    // to prevent this just save MSB and compare it after SHL if both are different then extend value by new cell
    char sign = this->globalHandle.R->var & 0b10000000;
    do{
        /*
            mov MSB to buffer[0]
            SHL
            set LSB to buffer[1]
            mov buffer[0] to buffer[1]
            set handle to cell on the left side
        */
        buffer[0] = handle->var & 0b10000000; // hold left bit of a cell
        handle->var <<= 1; // shl on cell
        // use buffer to assign most right bit 
        // buffer can be 0b00000000(false) or 0b10000000(true)
        // use property of | and & operators 
        // | - where is 1 there will be allways 1 where 0 there nothing will change in those bits
        // & - where is 0 there will be allways 0 where 1 there nothing will change in those bits
        if(buffer[1]) handle->var |= 0b00000001;
        else handle->var &= 0b11111110;
        buffer[1] = buffer[0]; // hold bit to next cell
        handle = handle->L;
    }while(handle != &this->globalHandle);

    if((this->globalHandle.R->var & 0b10000000) != (sign & 0b10000000)){
        // overflow has occured!
        if(sign & 0b10000000) // check if value was positive or negative before SHL
            this->newCell(0b11111111); // was negative
        else 
            this->newCell(0b00000000); // was positive
    }
}










/**
 * @brief removes cells from begin which are not include any new information like 00000000 or 11111111
 */
void ALi::optymize(){
    if(this->globalHandle.R->var >= 0x80){ // is negative
        while(this->globalHandle.R->var == 0xFF && 
        this->globalHandle.R->R->var >= 0x80 &&
        this->globalHandle.R != &this->globalHandle)
            this->delCell();
        // should remove left cells untill patern ^11111111 1xxxxxxx xxxxxxxx* is repeating \
        and variable is not an ^11111111 number
    }
    else{ // is positive
        while(this->globalHandle.R->var == 00 && 
        this->globalHandle.R->R->var < 0x80 &&
        this->globalHandle.R != &this->globalHandle)
            this->delCell();
        // should remove left cells untill patern ^00000000 0xxxxxxx xxxxxxxx* is repeating \
        and variable is not an ^00000000 number
    }
}

/**
 * @brief change every single bit to oposite value
 */
void ALi::negate(){
    Cell* handle = &this->globalHandle;
    do{
        handle->var = ~handle->var;
        handle = handle->L;
    }while(handle != &this->globalHandle);
}

/**
 * @brief inverting value from (3) to (-3) and from (-10) to (10) ...
 */
void ALi::invert(){
    printf("invert is not finished\n");
    return;
    // this->negate();
    // if(this->globalHandle.var != 0b11111111){ // if is not full then add 1 and go on
    //     // now avoid changing this 0b01111111(127) to this 0b10000000(-128)
    //     if(this->globalHandle.R != &this->globalHandle) // there are more cells 
    //         this->globalHandle.var++;
    //     else{ // there no more cells
    //         if()
    //     }
    //     return;
    // }
    // else{
    //     Cell* handle = &this->globalHandle;
    //     // while is not the end and is 0b11111111
    //     do{
    //         handle->var = 0; // pretend adding was executed
    //         handle = handle->L;
    //     } while (handle != &this->globalHandle && handle->var == 0b11111111);
        
    //     if(handle != &this->globalHandle){ // cell with non full variable was foud 
    //         handle->var++;
    //     }
    //     else{

    //     }
    // }
}

/**
 * @brief remove all dynamic allocated cells 
 */
void ALi::clear(){
    while(this->delCell());
    this->globalHandle.var = 0;
}










/**
 * @brief print each byte of variable seperated by separator sign and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printBinary(const char* additionText) const{
    Cell* handle = this->globalHandle.R;
    do{
        printf("%s",toByte(handle->var).c_str());
        handle = handle->R;
        if(handle != this->globalHandle.R && this->separator != '\0') // slower but accurate and skips separator at the end
            printf("%c",this->separator);
    }while(handle != this->globalHandle.R);
    printf("%s",additionText);
}

/**
 * @brief print digit of variable seperated by separator sign every 3 digits and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printDecimal(const char* additionText) const{
    printf("printDecimal is not finished\n");
    return;
    printf("%s",additionText);
}


/**
 * @brief save variable to file in binary form
 * @param path path to file where variable should be stored
 */
void ALi::writeFileBinary(const char* path) const{
    FILE* file = fopen(path,"wb");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    Cell* handle = (Cell*) &this->globalHandle;
    do{
        fwrite(&handle->var, sizeof(char), 1, file); // bufer, size of cell, cell amout, source
        handle = handle->L;
    }while(handle != &this->globalHandle);
    fclose(file);
}

/**
 * @brief save variable to file in readable form allowing to add separator sign every 8 bits 
 * @param path path to file where variable should be stored
 */
void ALi::writeFileReadable(const char* path) const{
    FILE* file = fopen(path,"w");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    Cell* handle = this->globalHandle.R;
    do{
        fwrite(toByte(handle->var).c_str(), sizeof(char), 8, file); // bufer, size of cell, cell amout, source
        handle = handle->R;
        if(handle != NULL && this->separator != '\0')
            fwrite(&this->separator, sizeof(char), 1, file); // bufer, size of cell, cell amout, source
    }while(handle != this->globalHandle.R);
    fclose(file);
}

/**
 * @brief save variable to file 
 * @param path path to file where variable should be stored
 * @param type type of file, readable('r') or binary('b')
 */
void ALi::writeFile(const char* path, const char& type) const{
    switch (type){
        case 'b': this->writeFileBinary(path); break;
        case 'r': this->writeFileReadable(path); break;
        default: printf("unknown type!\nnot attempted to create file\n"); return;
    }
}


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
    fread(&this->globalHandle.var, 1, 1, file);
    char buffer;
    while(fread(&buffer, 1, 1, file) != 0){
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
    unsigned char var = 0;
    ALi tmp;
    while(fread(&buffer, 1, 1, file) != 0){
        if(buffer == '1' || buffer == '0'){
            if(buffer == '1')
                var |= 1 << i;
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
    this->globalHandle.var = tmp.globalHandle.R->var; // add 6 to globalhandle
    Cell* tmphandle = tmp.globalHandle.R->R;
    while(tmphandle != &tmp.globalHandle){ // iterate from 5 to 1 (including 1)
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










/**
 * @brief assigns existed ALi to variable
 * @param source ALi what will be assigned to variable
 */
void ALi::assignment(const ALi& source){
    this->clear();
    this->globalHandle.var = source.globalHandle.var;
    const Cell* handleSource = source.globalHandle.L;
    // const int *  -  (pointer to const int)
    // int const *  -  (pointer to const int)
    // int * const  -  (const pointer to int)
    while (handleSource != &source.globalHandle){
        this->newCell(handleSource->var);
        handleSource = handleSource->L;
    }
    this->optymize();
}

/**
 * @brief assigns source int to variable
 * @param source int what will be assigned to variable
 */
void ALi::assignment(const signed long long& source){
    signed long long sample = source;
    unsigned long long mask = 1;
    unsigned char byte;
    Cell* handle = &this->globalHandle;
    for(int i=0; i<8; i++){
        byte = 0;
        for(int j=0; j<8; j++){
            if(sample & mask)
                byte |= 0b10000000;
            if(j!=7) byte >>= 1;
            mask <<= 1;
        }
        handle->var = byte;
        if(i!=7) this->newCell(0);
        handle = handle->L; // after finish (for) handle == &this->globalHandle
    }
    this->optymize();
    // ~(static_cast<unsigned long long>(-1) >> 1); // most left bit 
    // use this to check if variable is negative and 
    // to avoid assign -1(1111...1111) as a one bit larger int but positive (0111...1111)
    // seems like problem solved by itself
}










/**
 * @brief print variable as a binary or decimal
 * @param type type of file, decimal('d') or binary('b')
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::print(const char& type, const char* additionText) const{
    switch (type){
        case 'b': this->printBinary(additionText); break;
        case 'd': this->printDecimal(additionText); break;
        default: printf("unknown type!\n"); return;
    }
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
        default: printf("unknown action!\nnot attempted to read/create file\n"); return;
    }
}


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
