#pragma once
#include "commandTokens.h"

// Will be used to store the different parts of an expression
class Part
{
    private:
    const Token* token;

    void copy(const Part& other);
    void deallocate();

    public:
    Part(const Token* token);
    Part(const Part& other);

    Part& operator = (const Part& other);

    virtual ~Part();

    virtual void print(std::ostream& out) const;

    virtual Part* clone() const; // Copying the value, not the pointer

};

