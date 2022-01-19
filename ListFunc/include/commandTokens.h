#pragma once
#include "types.h"
#include <iostream>
#include <ostream>

// Base class for tokens
class Token
{
    private:
    Type type;

    public:
    Token(const Type type);

    virtual void print(std::ostream& out) const;
};

// Class for function tokens
class FunctionT : public Token
{
    private:
    std::string name;

    public:
    FunctionT(const std::string& name);

    void print(std::ostream& out) const override;
} ;

// Class for number tokens
class NumberT : public Token
{
    private:
    double value;

    public:
    NumberT(const double value);

    void print(std::ostream& out) const override;
};

// Class for argument tokens
class ArgumentT : public Token
{
    private:
    unsigned value;

    public:
    ArgumentT(const unsigned value);
    
    void print(std::ostream& out) const override;    
};

