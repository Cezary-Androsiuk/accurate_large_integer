#pragma once
#include "../include.hpp"

/*
    ALi
    AccurateLargeInteger
    accurate large integer
*/
class ALi{
protected: public:
    /**
     * @brief single cell containing:
     * var (CELL_TYPE) store byte of information
     *      soon unsigned char will be changed to unsigned long long to save space like:
     *      if one cell can contain 8 times more bits(64 instead 8) then less cell are needed. 
     *      Cause every single cell one contains two pointers reduce 8 cells to just one remove 14 to Cell* saving a lot of space 
     * L (Cell*) pointer to cell on the left side 
     * R (Cell*) pointer to cell on the right side
     */
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
    ALi addition(const ALi& right) const; //
    void additionAssign(const ALi& right);

    // Subtraction
    void decrement();
    ALi subtraction(const ALi& right) const;
    void subtractionAssign(const ALi& right);

    // Multiplication
    ALi multiplication(const ALi& right) const;
    void multiplicationAssign(const ALi& right);

    // Division
    ALi division(const ALi& right) const;
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
    void operator =  (const ALi& right);

    bool operator == (const ALi& right) const;
    bool operator != (const ALi& right) const;
    bool operator >  (const ALi& right) const;
    bool operator <= (const ALi& right) const;
    bool operator <  (const ALi& right) const;
    bool operator >= (const ALi& right) const;
    
    ALi  operator ++ (int);
    // ALi  operator ++ ();
    ALi  operator +  (const ALi& right) const;
    void operator += (const ALi& right);
    
    ALi  operator -- (int);
    ALi  operator -- ();
    ALi  operator -  (const ALi& right) const;
    void operator -= (const ALi& right);

    ALi  operator *  (const ALi& right) const;
    void operator *= (const ALi& right);
    // vvvv need to return accurate large rational number
    // ALi  operator /  (const ALi& right) const;
    // void operator /= (const ALi& right);
};

