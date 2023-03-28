#pragma once
#include "../include.hpp"

/*
    ALi
    AccurateLargeInteger
    accurate large integer
*/
class ALi{
protected:
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
    ALi(const ALi& source);
    ALi(const signed long long& source);
    ALi(const char* type, const char* sourcePath);
    ~ALi();

protected:
    void newCell(const CELL_TYPE&);
    const bool delCell();

protected: public:
    void shr_();
    void shr_ext();
    void shl_();
    void shl_ext();
    void PMSB(const bool& bit);
    void PLSB(const bool& bit);

    const bool sign() const;
    const bool is_0(const Cell* const handle = nullptr) const;

    const bool is_p1(const Cell* const handle = nullptr) const;
    const bool is_p2(const Cell* const handle = nullptr) const;
    const bool is_n1(const Cell* const handle = nullptr) const;
    const bool is_n2(const Cell* const handle = nullptr) const;

    void clear();
    void optymize();
    void negate();
    void invert();

protected: public:
    // might be a good idea to split the class to ALi and ALi_stdio (standard input output)
    // Print
    void printBinary() const;
    void printDecimal() const;
    
    // Print Approximation
    void printBinaryApproximation(ALi appPrec = 1) const;
    void printDecimalApproximation(ALi appPrec = 1) const;

    // Store
    void export_cells(const char* path) const;
    void import_cells(const char* path);

    // File Write
    void writeFile_02(FILE* const file) const;
    void writeFile_10(FILE* const file) const;
    void writeFile(const char* type, const char* path) const;

    // File Read
    void readFile_02(FILE* const file);
    void readFile_10(FILE* const file);
    void readFile(const char* type, const char* path);
    
protected: public:
    // Assignment
    void assignment(const ALi& source);
    void assignment(const signed long long& source);

    // Assignment String
    void assignment_str_02(std::string source);
    void assignment_str_10(std::string source);
    void assignment_str(const std::string& source);

    // Special
    ALi absoluteValue() const;
    void absoluteValueAssign();

    // Bolean
    const bool equal(const ALi& right) const;
    const bool greaterThan(const ALi& right) const;
    const bool smallerThan(const ALi& right) const;

    // Addition
    void increment_();
    void increment_ext();
    ALi addition_(const ALi& right) const;
    ALi addition_ext(const ALi& right) const;
    void additionAssign_(const ALi& right);
    void additionAssign_ext(const ALi& right);

    // Subtraction
    void decrement_();
    void decrement_ext();
    ALi subtraction_(const ALi& right) const;
    ALi subtraction_ext(const ALi& right) const;
    void subtractionAssign_(const ALi& right);
    void subtractionAssign_ext(const ALi& right);

    // Multiplication
    ALi multiplication(const ALi& right) const;
    void multiplicationAssign(const ALi& right);

    // Division
    ALi division(const ALi& right) const;
    void divisionAssign(const ALi& right);

    // Modulo
    ALi modulo(const ALi& right) const;
    void moduloAssign(const ALi& right);

    // Exponentiation
    ALi exponentiation(const ALi& right) const;
    void exponentiationAssign(const ALi& right);

public:
    // Public
    void print(const char* type_text) const;
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
    // in this class division will be integer type, to get rational number result before division object need to be casted to rational class object
    ALi  operator /  (const ALi& right) const;
    void operator /= (const ALi& right);

    ALi  operator %  (const ALi& right) const;
    void operator %= (const ALi& right);

    ALi  operator ^  (const ALi& right) const;
    void operator ^= (const ALi& right);
};

/*
TODO check if in any of previously implemeted methods contains "if" errors - exceptions are in wrong order




SPLITED IN TO MULTIPLE METHODS WILL BE GOOD IDEA
?TODO    add SHL method without handle_overflow (most left bit will be skipped)
?TODO    add SHR method without handle_overflow (lefts empty cells (mask000 or mask111))

?TODO    add addition without overflow detection (adding won't increase variable lenght)
?TODO    add subtraction without overflow detection (subtracting won't increase variable lenght)

TODO    printBinaryApproximation
TODO    printDecimalApproximation

TODO    writeFile_10
TODO    readFile_10
TODO    assignment_10

TODO    printApproximation
TODO    static separator

*/