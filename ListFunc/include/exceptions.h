#pragma once
#include <string>
#include <iostream>
const std::string INVALID_CHARACTER = "Invalid character";
const std::string INVALID_SYNTAX = "Invalid syntax";
const std::string RUN_TIME = "Run time error";

class Exception
{
    private:
    std::string type;  // what the type of the error is
    std::string description; //description of the error

    public:
    Exception(const std::string& type, const std::string& desc);
    virtual ~Exception() = default;     

    /// @brief Print error in given stream
    /// @param out - stream to print the error in
    virtual void print(std::ostream& out) const;

};

class InvalidCharacter : public Exception
{
    private:
    std::string input;
    unsigned index;  //the index at which the error has occured

    public:
    InvalidCharacter(const std::string& characters, const std::string& input, const unsigned index);

    /// @brief Print error in given stream
    /// @param out - stream to print the error in
    void print(std::ostream& out) const override; 
    
};

class InvalidSyntax : public Exception
{
    InvalidSyntax(const std::string& description);
};

class RunTime : public Exception
{
    RunTime(const std::string& description);
};