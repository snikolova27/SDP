#include "exceptions.h"

// -------- Base exception --------
Exception::Exception(const std::string& type, const std::string& desc) : type(type), description(desc){};

void Exception :: print(std::ostream & out) const
{
    out << this->type << " - " << this->description << std::endl; 
}

// -------- Invalid chatacter exception --------
InvalidCharacter ::InvalidCharacter(const std::string& characters, const std::string& input, const unsigned index) : Exception(INVALID_CHARACTER, "'" + characters + " '"),
                                                                                                                    input(input), 
                                                                                                                    index(index){};
void InvalidCharacter :: print(std::ostream & out) const
{
    out << "You have entered: "  <<std::endl << this->input << std::endl;
    int i = 0;
    while(i < this->index) 
    {
        out << " ";
    }
    out << "^" << std::endl;

    Exception::print(out);
}

// -------- Invalid syntax expression --------
InvalidSyntax::InvalidSyntax(const std::string& description) : Exception(INVALID_SYNTAX, description) {};

RunTime :: RunTime(const std::string& description) : Exception(RUN_TIME,description) {};