#pragma once
#include "../include.h"

/*
    ALi
    AccurateLargeInteger
    ALi
    accurate large integer
    is negative if first bit from right is 1  

    todo decimal
    todo push bit on right
    todo addition
    todo subtraction

*/

/**
 * @brief single cell containing:
 * var (unsigned char) store byte of information
 *      soon unsigned char will be changed to unsigned long long to save space like:
 *      if one cell can contain 8 times more bits(64 instead 8) then less cell are needed. 
 *      Cause every single cell one contains two pointers reduce 8 cells to just one remove 14 to Cell* saving a lot of space 
 * L (Cell*) pointer to cell on the left side 
 * R (Cell*) pointer to cell on the right side
 */
class Cell{
public:
    unsigned char var;
    Cell* L;
    Cell* R;
};

class ALi{
protected:
    Cell globalHandle; //   [...11010001] <-

    unsigned long long length;
    char separator;
public:
    ALi();
    ALi(const ALi& source);
    ALi(const signed long long& source);
    ALi(const char* sourcePath,const char& type);
    ~ALi();

protected:
    void newCell(const unsigned char&);
    const bool delCell();
    // void newCellRight(UC);      // only for readFileReadable
    // const bool delCellRight();  // only for readFileReadable

protected: public:
    unsigned char MSB() const;
    bool isPositive() const;
    void SHR();
    void SHL();
    void PLSB(const unsigned char& bit);

    void optymize();
    void negate();
    void invert();
    void clear();

protected:
    void printBinary(const char* additionText) const;
    void printDecimal(const char* additionText) const;

    void writeFileReadable(const char* path) const;
    void writeFileBinary(const char* path) const;
    void writeFile(const char* path, const char& type) const;

    void readFileReadable(const char* path);
    void readFileBinary(const char* path);
    void readFile(const char* path, const char& type);
    
    void assignment(const ALi& source);
    void assignment(const signed long long& source);


    void increment();
    ALi addition(const ALi& right);
    void addition_assign(const ALi& right);


public:
    void setSeparator(const char& separatorSign = '\0');
    // const char getSeparator() const;

    void print(const char& type, const char* additionText = "") const;
    void file(const char* path, const char& action, const char& type);
};

