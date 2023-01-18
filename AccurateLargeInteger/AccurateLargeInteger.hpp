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

    Cell* begin_ptr;//   [...11010001] <-
    unsigned long long length;
    char separator; // '/0' means no separator others are printed in print method
public:
    ALi();//
    ALi(const ALi& source);//
    ALi(const signed long long& source);//
    ALi(const char* type, const char* sourcePath);//
    ~ALi();//

protected:
    void newCell(const CELL_TYPE&);//
    const bool delCell();//

protected:
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

protected:
    // might be a good idea to split the class to ALi and ALi_stdio (standard input output)
    // Print
    void printBinary() const;//
    void printDecimal() const;//
    
    // Print Approximation
    void printBinaryApproximation(unsigned long long appPrec) const;//!
    void printDecimalApproximation(unsigned long long appPrec) const;//!

    // Store
    void export_cells(const char* path) const;//
    void import_cells(const char* path);//

    // File Write
    void writeFile_02(FILE* const file) const;//
    void writeFile_10(FILE* const file) const;//
    void writeFile(const char* type, const char* path) const;//

    // File Read
    void readFile_02(FILE* const file);
    void readFile_10(FILE* const file);
    void readFile(const char* type, const char* path);//
    
protected: public:
    // Assignment
    void assignment(const ALi& source);//
    void assignment(const signed long long& source);//

    // Assignment String
    void assignment_02(const std::string& source);
    void assignment_10(const std::string& source);
    void assignment_str(const std::string& source);

    // Bolean
    const bool equal(const ALi& right) const;//
    const bool greaterThan(const ALi& right) const;//
    const bool smallerThan(const ALi& right) const;//

    // Addition
    void increment();
    ALi addition(const ALi& right) const;
    ALi addition2(const ALi& right) const;
    void additionAssign(const ALi& right);
    void additionAssign2(const ALi& right);

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
    void print(const char* type_text) const;
    void printApproximation(const char* type_text, unsigned long long approximationPrecision = 2) const;
    void file(const char* type_path);
    
    // Get / Set
    void setSeparator(const char& separatorSign = '\0');
    const char getSeparator() const;
    const bool isEmpty() const;

    // Operators
    void operator >> (const char* right) const;
    void operator << (const char* right);

    void operator =  (const ALi& right);

    bool operator == (const ALi& right) const;
    bool operator != (const ALi& right) const;
    bool operator >  (const ALi& right) const;
    bool operator <= (const ALi& right) const;
    bool operator <  (const ALi& right) const;
    bool operator >= (const ALi& right) const;
    
    ALi  operator ++ (int);
    ALi  operator ++ ();
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

