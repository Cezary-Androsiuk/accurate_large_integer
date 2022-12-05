#pragma once
#include "../include.h"

/*
    ALi
    AccurateLargeInteger
    accurate large integer
    is negative if first bit from left is 1  

    todo addition
    todo subtraction

*/
class ALi{
protected:
    Cell *globalHandle; //   [...11010001] <-
    unsigned long long length;
    char separator; // '/0' means no separator others are printed in print method
public:
    ALi();
    ALi(const ALi& source);
    ALi(const signed long long& source);
    ALi(const char* sourcePath,const char& type);
    ~ALi();

protected:
    void newCell(const unsigned char&);
    const bool delCell();
    // void newCellRight(UC);      // was needed only for readFileReadable
    // const bool delCellRight();  // was needed only for readFileReadable

protected: public:
    // unsigned long long MSCell() const;
    // unsigned long long MSB() const;
    bool isPositive() const;
    void SHR();
    void SHL();
    void PLSB(const unsigned char& bit);

    void optymize();
    bool need_opt() const;
    void negate();
    void invert();
    void clear();

protected:
    void printBinary() const;
    void printDecimal() const;

    void printBinaryApproximation(unsigned long long appPrec) const;
    void printDecimalApproximation(unsigned long long appPrec) const;

    void writeFileBinary(const char* path, const bool& append) const;
    void writeFileReadable(const char* path, const bool& append) const;
    void writeFile(const char* path, const char& type, const bool& append) const;

    void readFileBinary(const char* path);
    void readFileReadable(const char* path);
    void readFile(const char* path, const char& type);
    
public:
    void assignment(const ALi& source);
    void assignment(const signed long long& source);

    const bool equal(const ALi& right) const;
    const bool greaterThan(const ALi& right) const;
    const bool smallerThan(const ALi& right) const;


    void increment();
    ALi addition(const ALi& right);
    void additionAssign(const ALi& right);

    void decrement();
    ALi subtraction(const ALi& right);
    void subtractionAssign(const ALi& right);


public:
    void print(const char& type, const char* additionText = "", unsigned long long alignment = 0) const;
    void printApproximation(const char& type, const char* additionText = "", unsigned long long approximationPrecision = 2) const;
    void file(const char* path, const char& action, const char& type);
    void setSeparator(const char& separatorSign = '\0');
    const char getSeparator() const;

    const bool isEmpty() const;
};

