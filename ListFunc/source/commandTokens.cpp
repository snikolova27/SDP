#include "commandTokens.h"
#include <ostream>

// -------- Base token --------
Token :: Token(const Type type) : type(type) {};

void Token :: print(std::ostream& out) const
{
    switch(this->type)
    {
        case Type::NAME_OF_FUNCTION:
        {
            out << "NAME OF FUNCTION ";
            break;
        } 

        case Type::NUMBER:
        {
            out << "NUMBER ";
            break;
        }
        case Type::ARGUMENT:
        {
            out << "ARGUMENT ";
            break;
        }
        case Type::OPENING_BRACKET:
        {
            out << "OPENING BRACKET ";
            break;
        }
        case Type::CLOSING_BRACKET:
        {
            out << "CLOSING BRACKET ";
            break;
        }

        case Type::COMMA:
        {
            out << "COMMA ";
            break;
        }
        case Type::ARROW:
        {
            out << "ARROW ";
            break;
        }
        default:
        {
            out << "UNKNOWN TOKEN";
        }
    }
}

// -------- Function token --------
FunctionT::FunctionT(const std::string& name) : Token(Type::NAME_OF_FUNCTION), name(name){}

void FunctionT::print(std::ostream &out) const
{
    Token::print(out);
    out << ": " << this->name << std::endl;
}

// -------- Number token --------
NumberT :: NumberT(const double value) : Token(Type::NUMBER), value(value) {}

void NumberT::print(std::ostream& out) const
{
    Token::print(out);
    out << ": " << this->value << std::endl;
}

// -------- Argument token --------
ArgumentT :: ArgumentT(const unsigned value) : Token(Type::ARGUMENT), value(value) {}

void ArgumentT::print(std::ostream& out) const
{
    Token::print(out);
    out << ": " << this->value << std::endl;
}