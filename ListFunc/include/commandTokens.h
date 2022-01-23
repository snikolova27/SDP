#pragma once
#include "types.h"
#include <iostream>
#include <ostream>

// Base class for tokens
class Token
{
    public:
    Type type;
    Token(const Type type);

    virtual void print(std::ostream& out) const;
};

// Class for function tokens
class FunctionT : public Token
{
    public:
    std::string name;
    FunctionT(const std::string& name);

    void print(std::ostream& out) const override;
} ;

// Class for number tokens
class NumberT : public Token
{
    public:
    double value;
    NumberT(const double value);

    void print(std::ostream& out) const override;
};

// Class for argument tokens
class ArgumentT : public Token
{
    public:
    unsigned value;
    ArgumentT(const unsigned value);
    
    void print(std::ostream& out) const override;    
};

