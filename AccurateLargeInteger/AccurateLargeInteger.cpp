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
    this->globalHandle->var = mask000; // initialized will be 0
    this->separator = '\0'; // '/0' means no separator others are printed in print method
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
    delete this->globalHandle;
}










/**
 * @brief create new cell from the left side
 * @param standardSource: unsigned long long value that will be used to optional assigned value to new cell
 */
void ALi::newCell(const unsigned char &standardSource){
    Cell* handle = new Cell;
    handle->L = this->globalHandle;
    handle->R = this->globalHandle->R;
    this->globalHandle->R->L = handle;
    this->globalHandle->R = handle;

    handle->var = static_cast<unsigned char>(standardSource);
    this->length++;
}

/**
 * @brief remove cell from the left, not including this->globalHandle
 * @return 1: if there are more cells that can be deleted
 * @return 0: if there are no more cells that can be deleted
 */
const bool ALi::delCell(){
    // all cells that can be deleted, was already deleted
    if(this->globalHandle->R == this->globalHandle){
        this->globalHandle->var = mask000;
        return false;
    }
    // n dynamically alocated cells where n >= 1
    Cell* handle = this->globalHandle->R;
    handle->R->L = this->globalHandle;
    this->globalHandle->R = handle->R;
    delete handle;
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








//! WITHOUT OPTYMALIZE METHOD

// /**
//  * @brief finding an index of the most significant bit
//  * @return unsigned long long : index of the cell with the most significant bit
//  * globalHandle have index 0
//  */
// unsigned long long ALi::MSCell() const{
//     // printf("MSCell is not finished\n");
//     // exit(0);
//     // return -1;

//     Cell* handle = this->globalHandle->R;
//     unsigned long long i = this->length;
//     if(this->isPositive()){
//         // find cell with mscell (first that contain 1)
//         while(handle->var == mask000 && handle != this->globalHandle){
//             handle = handle->R;
//             i--;
//         }
//     }
//     else{
//         // if is negative 
//         // 1110 1111 == -17
//         // find cell with mscell (first that contain 0)
//         while(handle->var == mask111 && handle != this->globalHandle){
//             handle = handle->R;
//             i--;
//         }
//     }
//     return i-1;
// }

// /**
//  * @brief 
//  * @return unsigned long long: 
//  */
// unsigned long long ALi::MSB() const{

// }


/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool ALi::isPositive() const{
    if(this->globalHandle->R->var & mask100) return false;
    else return true; 
}

/**
 * @brief execute shift right operation
 */
void ALi::SHR(){
    // if there are no cells ok if there more cells ok cause allways start from the most left cell
    Cell* handle = this->globalHandle->R;
    unsigned char buffer[2] = {0,0};
    
    if(this->globalHandle->R->var >= mask100) // is negative so the most left bit will be 1
        buffer[1] = 1;
    do{
        /*
            mov LSB to buffer[0]
            SHR
            set MSB to buffer[1]
            mov buffer[0] to buffer[1]
            set handle to cell on the right side
        */
        buffer[0] = handle->var & mask001; // hold right bit of a cell
        handle->var >>= 1; // shr on cell
        // use buffer to assign most left bit
        // buffer can be 0b00000000(false) or 0b00000001(true)
        // use property of | and & operators 
        // | - where is 1 there will be allways 1 where 0 there nothing will change in those bits
        // & - where is 0 there will be allways 0 where 1 there nothing will change in those bits
        if(buffer[1]) handle->var |= mask100;
        else handle->var &= mask011;
        buffer[1] = buffer[0]; // hold bit to next cell
        handle = handle->R;
    } while (handle != this->globalHandle->R);
}

/**
 * @brief execute shift left operation
 */
void ALi::SHL(){
    // if there are no cells ok if there more cells ok cause allways start from the most right cell
    Cell* handle = this->globalHandle; 
    unsigned char buffer[2] = {0,0};
    // where most left bit after SHL change their value then overflow has occurred
    // to prevent this just save MSB and compare it after SHL if both are different then extend value by new cell
    char sign = this->globalHandle->R->var & mask100;
    do{
        /*
            mov MSB to buffer[0]
            SHL
            set LSB to buffer[1]
            mov buffer[0] to buffer[1]
            set handle to cell on the left side
        */
        buffer[0] = handle->var & mask100; // hold left bit of a cell
        handle->var <<= 1; // shl on cell
        // use buffer to assign most right bit 
        // buffer can be 0b00000000(false) or 0b10000000(true)
        // use property of | and & operators 
        // | - where is 1 there will be allways 1 where 0 there nothing will change in those bits
        // & - where is 0 there will be allways 0 where 1 there nothing will change in those bits
        if(buffer[1]) handle->var |= mask001;
        else handle->var &= mask110;
        buffer[1] = buffer[0]; // hold bit to next cell
        handle = handle->L;
    }while(handle != this->globalHandle);

    if((this->globalHandle->R->var & mask100) != (sign & mask100)){
        // overflow has occured!
        if(sign & mask100) // check if value was positive or negative before SHL
            this->newCell(mask111); // was negative
        else 
            this->newCell(mask000); // was positive
    }
}

/**
 * @brief push least significant bit
 * @param bit if 0, 0 will be pushed if 1, 1 will be pushed on lsb
 */
void ALi::PLSB(const unsigned char& bit){
    this->SHL();
    this->globalHandle->var |= (bit & mask001 ? mask001 : mask000);
}










/**
 * @brief removes cells from begin which are not include any new information like 00000000 or 11111111
 */
void ALi::optymize(){
    if(this->globalHandle->R->var >= mask100){ // is negative
        while(this->globalHandle->R->var == mask111 && 
        this->globalHandle->R->R->var >= mask100 &&
        this->globalHandle->R != this->globalHandle)
            this->delCell();
        // should remove left cells untill patern ^11111111 1xxxxxxx xxxxxxxx* is repeating \
        and variable is not an ^11111111 number
    }
    else{ // is positive
        while(this->globalHandle->R->var == mask000 && 
        this->globalHandle->R->R->var < mask100 &&
        this->globalHandle->R != this->globalHandle)
            this->delCell();
        // should remove left cells untill patern ^00000000 0xxxxxxx xxxxxxxx* is repeating \
        and variable is not an ^00000000 number
    }

/*  methods using optimization 04.12.2022
    # ALi(const ALi& source);
    # ALi(const signed long long& source);
    # ALi(const char* sourcePath,const char& type);
    readFileBinary(const char* path);
    readFileReadable(const char* path);
    # readFile(const char* path, const char& type);
    assignment(const ALi& source);
    assignment(const signed long long& source);
*/
}

/**
 * @brief check if variable need optymization
 * if first and second cells are (0 and 0) or (256^sizeof(Cell::var)-1 and 256^sizeof(Cell::var)-1)
 * then number need an optimization
 * @return true need optymization
 * @return false is already optymized
 */
bool ALi::need_opt() const{
    if((this->globalHandle->R->var == mask000 && this->globalHandle->R->var == mask000)||
    (this->globalHandle->R->var == mask111 && this->globalHandle->R->var == mask111))
        return true;
    else return false;
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

/**
 * @brief remove all dynamic allocated cells 
 */
void ALi::clear(){
    while(this->delCell());
    this->globalHandle->var = mask000;
}










/**
 * @brief print each byte of variable seperated by separator sign and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printBinary() const{
    Cell* handle = this->globalHandle->R;
    do{
        printf("%s",toByte(handle->var).c_str());
        handle = handle->R;
        if(handle != this->globalHandle->R && this->separator != '\0') // slower but accurate and skips separator at the end
            printf("%c",this->separator);
    }while(handle != this->globalHandle->R);
}

/**
 * @brief print digit of variable seperated by separator sign every 3 digits and adding after all addition text
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::printDecimal() const{
    // if it is negative invert and print '-' sign
    if(!this->isPositive()){
        printf("-");
        ALi tmp(*this);
        tmp.invert();
        return;
    }

    if(this->isEmpty()){
        printf("0");
        return;
    }

    #define divby 10

    ALi slider(*this);
    Stacker<char> decimal(new char('!'));
    unsigned long long sliderBitsCount = 0;
    while(slider.globalHandle->var != mask000 || slider.length != mask001){ // iterate through each decimal digit
        slider.optymize(); // to keep as short as possible
        if(slider.globalHandle->L != slider.globalHandle || slider.globalHandle->var >= divby){
            sliderBitsCount = BITS_PER_VAR * (slider.length);
            slider.newCell(mask000); // as a buffer
            for(int i=0; i<sliderBitsCount; i++){ // iterate through each SHL using PLSB
                if(slider.globalHandle->R->var >= divby){
                    slider.globalHandle->R->var -= divby;
                    slider.PLSB(mask001);
                }
                else slider.PLSB(mask000);
            }
        }
        if(slider.globalHandle->R->var >= divby){
            decimal.push(slider.globalHandle->R->var - divby);
            slider.PLSB(mask001);
        } 
        else{
            decimal.push(slider.globalHandle->R->var);
            slider.PLSB(mask000);
        }
        slider.delCell();
    }

    while(!decimal.isEmpty()){
        printf("%d",*decimal.top());
        decimal.pop();
    }
}


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


/**
 * @brief save variable to file in binary form
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFileBinary(const char* path, const bool& append) const{
    FILE* file = fopen(path,(append ? "ab" : "wb"));
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    Cell* handle = (Cell*) &this->globalHandle;
    do{
        fwrite(&handle->var, sizeof(char), 1, file); // bufer, size of cell, cell amout, source
        handle = handle->L;
    }while(handle != this->globalHandle);
    fclose(file);
}

/**
 * @brief save variable to file in readable form allowing to add separator sign every 8 bits 
 * @param path path to file where variable should be stored
 * @param append append or overwrite file if exist 
 */
void ALi::writeFileReadable(const char* path, const bool& append) const{
    FILE* file = fopen(path,"w");
    if(file == NULL){
        printf("File \"%s\" not found!\n",path);
        return;
    }
    Cell* handle = this->globalHandle->R;
    do{
        fwrite(toByte(handle->var).c_str(), sizeof(char), 8, file); // bufer, size of cell, cell amout, source
        handle = handle->R;
        if(handle != NULL && this->separator != '\0')
            fwrite(&this->separator, sizeof(char), 1, file); // bufer, size of cell, cell amout, source
    }while(handle != this->globalHandle->R);
    fclose(file);
}

/**
 * @brief save variable to file 
 * @param path path to file where variable should be stored
 * @param type type of file, readable('r') or binary('b')
 * @param append append or overwrite file if exist 
 */
void ALi::writeFile(const char* path, const char& type, const bool& append) const{
    switch (type){
        case 'b': this->writeFileBinary(path,append); break;
        case 'r': this->writeFileReadable(path,append); break;
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
    fread(&this->globalHandle->var, 1, 1, file);
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










/**
 * @brief assigns existed ALi to variable
 * @param source ALi what will be assigned to variable
 */
void ALi::assignment(const ALi& source){
    this->clear();
    this->globalHandle->var = source.globalHandle->var;
    const Cell* handleSource = source.globalHandle->L;
    // const int *  -  (pointer to const int)
    // int const *  -  (pointer to const int)
    // int * const  -  (const pointer to int)
    while (handleSource != source.globalHandle){
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
    unsigned long long mask = mask001;
    unsigned char byte;
    Cell* handle = this->globalHandle;
    for(int i=0; i<8; i++){
        byte = 0;
        for(int j=0; j<8; j++){
            if(sample & mask)
                byte |= mask100;
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
 * @brief check if left object is equal to right object
 * @param right object
 * @return true left equal right
 * @return false left not equal right
 */
const bool ALi::equal(const ALi& right) const{
    // assume that both are optymized otherways it is harder...
    if(this->length != right.length) return false;
    Cell* hL = this->globalHandle->R;
    Cell* hR = right.globalHandle->R;
    while(hL != this->globalHandle){
        if(hL->var != hR->var) return false;
        hL = hL->R;
        hR = hR->R;
    }
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
    const bool Lplus = this->isPositive();
    const bool Rplus = right.isPositive();
    // if(Lplus && !Rplus) return true; // left is positive right is negative => left is greater
    // if(!Lplus && Rplus) return false; // left is negative right is positive => left is smaller
    if(Lplus != Rplus) return Lplus;
    // now signs are equal

    // if(Lplus){
    //     // compare positive numbers
    //     // like ealier both args must be optymized
    //     if(this->length > right.length) return true; // left is longer => left is greater
    //     else if(this->length < right.length) return false; // left is shorter => left is smaller
    //     Cell* hL = this->globalHandle->R;
    //     Cell* hR = right.globalHandle->R;
    //     while(hL != this->globalHandle){
    //         if(hL->var > hR->var) return true; // left have greater cell => left is greater
    //         if(hL->var < hR->var) return false; // left have smaller cell => left is smaller
    //         hL = hL->R;
    //         hR = hR->R;
    //     }
    // }
    // else{
    //     // compare negative numbers
    //     // like ealier both args are optymized
    //     if(this->length > right.length) return false; // left is longer => left is smaller
    //     else if(this->length < right.length) return true; // left is shorter => left is greater
    //     Cell* hL = this->globalHandle->R;
    //     Cell* hR = right.globalHandle->R;
    //     while(hL != this->globalHandle){
    //         if(hL->var > hR->var) return true; // left have greater cell => left is closer to 0 => left is greater
    //         if(hL->var < hR->var) return false; // left have smaller cell => left is further from 0 => left is smaller
    //         hL = hL->R;
    //         hR = hR->R;
    //     }
    // }
    
    // is possible to find common block of code
    if(this->length > right.length) return Lplus; // left is longer => left is smaller
    else if(this->length < right.length) return !Lplus; // left is shorter => left is greater

    Cell* hL = this->globalHandle->R;
    Cell* hR = right.globalHandle->R;
    while(hL != this->globalHandle){ // compare each cell starts from the most significant
        if(hL->var > hR->var) return true;
        if(hL->var < hR->var) return false;
        hL = hL->R;
        hR = hR->R;
    }
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
    const bool Lplus = this->isPositive();
    const bool Rplus = right.isPositive();
    // if(!Lplus && Rplus) return true; // left is negative right is positive => left is smaller
    // if(Lplus && !Rplus) return false; // left is positive right is negative => left is greater
    if(Lplus != Rplus) return !Lplus;
    // now signs are equal

    // if(Lplus){
    //     // compare positive numbers
    //     // like ealier both args must be optymized
    //     if(this->length < right.length) return true; // left is shorter => left is smaller
    //     else if(this->length > right.length) return false; // left is longer => left is greater
    //     Cell* hL = this->globalHandle->R;
    //     Cell* hR = right.globalHandle->R;
    //     while(hL != this->globalHandle){
    //         if(hL->var > hR->var) return false; // left have greater cell => left is greater
    //         if(hL->var < hR->var) return true; // left have smaller cell => left is smaller
    //         hL = hL->R;
    //         hR = hR->R;
    //     }
    // }
    // else{
    //     // compare negative numbers
    //     // like ealier both args must be optymized
    //     if(this->length < right.length) return false; // left is shorter => left is greater
    //     else if(this->length > right.length) return true; // left is longer => left is smaller
    //     Cell* hL = this->globalHandle->R;
    //     Cell* hR = right.globalHandle->R;
    //     while(hL != this->globalHandle){
    //         if(hL->var > hR->var) return false; // left have greater cell => left is closer to 0 => left is greater
    //         if(hL->var < hR->var) return true; // left have smaller cell => left is futher from 0 => left is smaller
    //         hL = hL->R;
    //         hR = hR->R;
    //     }
    // }
    
    // is possible to find common block of code
    if(this->length < right.length) return Lplus;
    else if(this->length > right.length) return !Lplus;

    Cell* hL = this->globalHandle->R;
    Cell* hR = right.globalHandle->R;
    while(hL != this->globalHandle){ // compare each cell starts from the most significant
        if(hL->var > hR->var) return false;
        if(hL->var < hR->var) return true;
        hL = hL->R;
        hR = hR->R;
    }
    return false; // both are equal => left is NOT smaller
}










/**
 * @brief increment ALi by one
 * 
 */
void ALi::increment(){
    if(this->isPositive()){
        Cell *handle = this->globalHandle;
        while(handle->var == mask111 && handle != this->globalHandle->R){
            handle->var = mask000; // 11111111 -> (1)00000000
            handle = handle->L;
        }
        // handle is pointing at first not full cell

        if(handle->var == mask011 && handle == this->globalHandle->R){ 
            // is the last cell and looks like 0b01111111 (on msb contains sign bit)
            this->newCell(mask000); // keep sign bit
            handle->var++;
        }
        else{ // is the casual cell or last cell with variable less than 0b01111111
            handle->var ++;
        }
    }
    else{
        Cell *handle = this->globalHandle;
        while(handle->var == mask000 && handle != this->globalHandle->R){
            handle->var = mask111; // 00000000 -> (-1)11111111
            handle = handle->L;
        }
        // handle is pointing at first not full cell

        if(handle->var == mask100 && handle == this->globalHandle->R){ 
            // is the last cell and looks like 0b10000000 (on msb contains sign bit)
            this->newCell(mask111); // keep sign bit
            handle->var = mask011;
        }
        else{ // is the casual cell or last cell with variable greater than 0b10000000
            handle->var ++;
        }
    }
}

/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::addition(const ALi& right){
    ALi result;
    if(right.isEmpty()) return *this; // both are 0 or right is 0
    if(this->isEmpty()) return right; // left is 0
    CELL_TYPE carry = 0;
    
    // iterate always for the longer one so expect 1234+123 but not 123+1234
    // it is easier to just on begine know which is which
    // not "const pointers to objects" but "pointers to const objects"
    ALi const* greater = this; 
    ALi const* smaller = &right;

    // corect (switch sides if needed) cause the greater handle, need to hold the longer (not greater) value for properly working 
    if(this->length < right.length){
        greater = &right;
        smaller = this;
    }
    Cell* grHandle = greater->globalHandle;
    Cell* smHandle = smaller->globalHandle;
    
    bool smHandleReachedEnd = false;
    do{ 
        // i want to keep this comment XD
        // seams that if it will be const type then compiler might use (i hope) more efficient algorithm
        // cause he already know that this value won't be changed in future
        // if i want to ignore this idea just replace pointer to const value with non const standard value 
        // const CELL_TYPE* cellsum; 
        // if(smHandleReachedEnd) sum = new const CELL_TYPE(grHandle->var + (smaller->isPositive() ? mask000 : mask111) + carry);
        // else cellsum = new const CELL_TYPE(grHandle->var + smHandle->var + carry);
        
        // hold the cells addition sum
        const CELL_TYPE cellsum = grHandle->var + (smHandleReachedEnd ? (smaller->isPositive() ? mask000 : mask111) : smHandle->var) + carry;
        
        // if this is first iteration of while, add holded cells addition sum to globalHandle else create new one
        // thanks to this avoid 8 SHR at the end, but also i thnk for larger numbers this "if" can be more significant than 8 SHR at the end
        // but again for large numbers there is more to shift... idk what will be more efficient, i will stay with this "if" usage istead SHR
        if(grHandle == greater->globalHandle)
            result.globalHandle->var = cellsum;
        else
            result.newCell(cellsum);
        
        // if overflow occur, then carry this additional bit to next cell
        // overflow has occured for [a + b = c] when [c < a] and [c < b]
        // in other words current operated right and left cells are smaller than current operated result cell
        // + need to consider extra case, idk how to change this two condition in to one
        if((result.globalHandle->R->var < grHandle->var && result.globalHandle->R->var < smHandle->var) ||
        (result.globalHandle->R->var <= grHandle->var && result.globalHandle->R->var <= smHandle->var && carry == 1)) carry = 1;
        else carry = 0;

        // change handle to next cell
        grHandle = grHandle->L;
        // keep smaller handle at the last cell if was ended before greater handle
        // this is handling on the begine of while loop by add mask000 or mask111
        if(smHandle->L != smaller->globalHandle)
            smHandle = smHandle->L;
        else 
            smHandleReachedEnd = true;
    }while(grHandle != greater->globalHandle);

    // overflow can only appear when adding two positive or two negative values
    if(greater->isPositive() == smaller->isPositive()){
        // and if they were an positive numbers and result finally is negative then add new cell with mask000
        // but if they were an negative numbers and result finally is positive then add new cell with mask111
        if(greater->isPositive() != result.isPositive()){ // overflow accured 
            if(greater->isPositive()) result.newCell(mask000);
            else result.newCell(mask111);
        } 
    }
    result.optymize();
    // if numbers have an oposite signs then just ignore last carry sign 

    return result;
}

/**
 * @brief 
 * @param right 
 */
void ALi::additionAssign(const ALi& right){

}


/**
 * @brief increment ALi by one
 * 
 */
void ALi::decrement(){
    printf("decrement not finished yet\n");
    return;
    if(this->isPositive()){
        Cell *handle = this->globalHandle;
        while(handle->var == mask111 && handle != this->globalHandle->R){
            handle->var = mask000; // 11111111 -> (1)00000000
            handle = handle->L;
        }
        // handle is pointing at first not full cell

        if(handle->var == mask011 && handle == this->globalHandle->R){ 
            // is the last cell and looks like 0b01111111 (on msb contains sign bit)
            this->newCell(mask000); // keep sign bit
            handle->var++;
        }
        else{ // is the casual cell or last cell with variable less than 0b01111111
            handle->var ++;
        }
    }
    else{
        Cell *handle = this->globalHandle;
        while(handle->var == mask000 && handle != this->globalHandle->R){
            handle->var = mask111; // 00000000 -> (-1)11111111
            handle = handle->L;
        }
        // handle is pointing at first not full cell

        if(handle->var == mask100 && handle == this->globalHandle->R){ 
            // is the last cell and looks like 0b10000000 (on msb contains sign bit)
            this->newCell(mask111); // keep sign bit
            handle->var = mask011;
        }
        else{ // is the casual cell or last cell with variable greater than 0b10000000
            handle->var ++;
        }
    }
}

/**
 * @brief 
 * @param right 
 * @return ALi 
 */
ALi ALi::subtraction(const ALi& right){
    //! carry bit was changed, now consider about negation each bit while adding (without duplicate entire object)
    ALi result;
    if(right.isEmpty()) return *this; // both are 0 or right is 0
    if(this->isEmpty()) return right; // left is 0
    CELL_TYPE carry = 1;
    
    // iterate always for the longer one so expect 1234+123 but not 123+1234
    // it is easier to just on begine know which is which
    // not "const pointers to objects" but "pointers to const objects"
    ALi const* greater = this; 
    ALi const* smaller = &right;

    // corect (switch sides if needed) cause the greater handle, need to hold the longer (not greater) value for properly working 
    if(this->length < right.length){
        greater = &right;
        smaller = this;
    }
    Cell* grHandle = greater->globalHandle;
    Cell* smHandle = smaller->globalHandle;
    
    bool smHandleReachedEnd = false;
    do{ 
        // i want to keep this comment XD
        // seams that if it will be const type then compiler might use (i hope) more efficient algorithm
        // cause he already know that this value won't be changed in future
        // if i want to ignore this idea just replace pointer to const value with non const standard value 
        // const CELL_TYPE* cellsum; 
        // if(smHandleReachedEnd) sum = new const CELL_TYPE(grHandle->var + (smaller->isPositive() ? mask000 : mask111) + carry);
        // else cellsum = new const CELL_TYPE(grHandle->var + smHandle->var + carry);
        
        // hold the cells addition sum
        const CELL_TYPE cellsum = grHandle->var + (smHandleReachedEnd ? (smaller->isPositive() ? mask000 : mask111) : smHandle->var) + carry;
        
        // if this is first iteration of while, add holded cells addition sum to globalHandle else create new one
        // thanks to this avoid 8 SHR at the end, but also i thnk for larger numbers this "if" can be more significant than 8 SHR at the end
        // but again for large numbers there is more to shift... idk what will be more efficient, i will stay with this "if" usage istead SHR
        if(grHandle == greater->globalHandle)
            result.globalHandle->var = cellsum;
        else
            result.newCell(cellsum);
        
        // if overflow occur, then carry this additional bit to next cell
        // overflow has occured for [a + b = c] when [c < a] and [c < b]
        // in other words current operated right and left cells are smaller than current operated result cell
        // + need to consider extra case, idk how to change this two condition in to one
        if((result.globalHandle->R->var < grHandle->var && result.globalHandle->R->var < smHandle->var) ||
        (result.globalHandle->R->var <= grHandle->var && result.globalHandle->R->var <= smHandle->var && carry == 1)) carry = 1;
        else carry = 0;

        // change handle to next cell
        grHandle = grHandle->L;
        // keep smaller handle at the last cell if was ended before greater handle
        // this is handling on the begine of while loop by add mask000 or mask111
        if(smHandle->L != smaller->globalHandle)
            smHandle = smHandle->L;
        else 
            smHandleReachedEnd = true;
    }while(grHandle != greater->globalHandle);

    // overflow can only appear when adding two positive or two negative values
    if(greater->isPositive() == smaller->isPositive()){
        // and if they were an positive numbers and result finally is negative then add new cell with mask000
        // but if they were an negative numbers and result finally is positive then add new cell with mask111
        if(greater->isPositive() != result.isPositive()){ // overflow accured 
            if(greater->isPositive()) result.newCell(mask000);
            else result.newCell(mask111);
        } 
    }
    result.optymize();
    // if numbers have an oposite signs then just ignore last carry sign 

    return result;
}

/**
 * @brief 
 * @param right 
 */
void ALi::subtractionAssign(const ALi& right){

}


















/**
 * @brief print variable as a binary or decimal
 * @param type type of file, decimal('d') or binary('b')
 * @param additionText default is "" text what will be printed at the end of variable
 */
void ALi::print(const char& type, const char* additionText, unsigned long long alignment) const{
    switch (type){
        case 'b': 
            for(unsigned long long i=0; i<alignment; i++){
                printf("%s",(this->isPositive() ? "00000000" : "11111111"));
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
void ALi::printApproximation(const char& type, const char* additionText = "", unsigned long long approximationPrecision = 2) const{
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
        case 'w': this->writeFile(path,type,false); break;
        case 'a': this->writeFile(path,type,true); break;
        case 'r': this->readFile(path,type); break;
        default: printf("unknown action!\nnot attempted to write/append/read a file\n"); return;
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
// const char ALi::getSeparator() const{
//     return this->separator;
// }


/**
 * @brief check if is empty
 * @return true ALi is empty
 * @return false ALi containing something
 */
const bool ALi::isEmpty() const{
    // if(this->globalHandle.L == &this->globalHandle && this->globalHandle.var == 0) return true;
    if(this->length == 1 && this->globalHandle->var == 0) return true;
    else return false;
}