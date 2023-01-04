#pragma once
#include "../include.hpp"

/*
    ALi
    AccurateLargeInteger
    accurate large integer
*/
class ALi{
protected: public:
    // class Cell{ // i like more classes than structs 
    // public:
    //     CELL_TYPE var;
    //     Cell* L;
    //     Cell* R;
        
    //     // addition usage
    //     // Cell(){}
    //     // Cell(CELL_TYPE v, Cell* l, Cell* r){var=v;L=l;R=r;}
    //     // ~Cell(){}
    // };

    struct Cell{
        CELL_TYPE var;
        Cell* L;
        Cell* R;
    };

    Cell *globalHandle; //   [...11010001] <-
    unsigned long long length;
    char separator; // '/0' means no separator others are printed in print method
public:
    ALi();//
    ALi(const ALi& source);//
    ALi(const signed long long& source);//
    ALi(const char* sourcePath,const char& type);//
    ~ALi();//

protected: public:
    void newCell(const CELL_TYPE&);//
    const bool delCell();//

protected: public:
    void SHR();//
    void SHL();//
    void PMSB(const bool& bit);//
    void PLSB(const bool& bit);//

    const bool sgn() const;//
    const bool is0() const;//
    const bool is1() const;//

    void clear();//
    void optymize();//
    void negate();//
    void invert();//

protected: public:
    void printBinary() const;//
    void printDecimal() const;//

    void printBinaryApproximation(unsigned long long appPrec) const;//!
    void printDecimalApproximation(unsigned long long appPrec) const;//!

    void writeFileBinary(const char* path) const;//
    void writeFileReadable(const char* path) const;//
    void writeFile(const char* path, const char& type) const;//

    void readFileBinary(const char* path);//
    void readFileReadable(const char* path);//
    void readFile(const char* path, const char& type);//
    
protected: public:
    // Assignment
    void assignment(const ALi& source);//
    void assignment(const signed long long& source);//

    // Bolean
    const bool equal(const ALi& right) const;//
    const bool greaterThan(const ALi& right) const;//
    const bool smallerThan(const ALi& right) const;//

    // Addition
    void increment();//*
    ALi addition(const ALi& right); //
    void additionAssign(const ALi& right);

    // Subtraction
    void decrement();
    ALi subtraction(const ALi& right);
    void subtractionAssign(const ALi& right);

    // Multiplication
    ALi multiplication(const ALi& right);
    void multiplicationAssign(const ALi& right);

    // Division
    ALi division(const ALi& right);
    void divisionAssign(const ALi& right);

public:
    // Public
    void print(const char& type, const char* additionText = "", unsigned long long alignment = 0) const;
    void printApproximation(const char& type, const char* additionText = "", unsigned long long approximationPrecision = 2) const;
    void file(const char* path, const char& action, const char& type);
    // Get / Set
    void setSeparator(const char& separatorSign = '\0');
    const char getSeparator() const;
    const bool isEmpty() const;

    // Operators
};

