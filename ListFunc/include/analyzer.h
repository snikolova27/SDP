#pragma once
#include "commandTokens.h"
#include "expressionElements.h"
#include "types.h"
#include <iostream>

const std::string LIST = "list";
const std::string MAP = "map";
const std::string IF = "if"; 

// Used to analyze the different parts of an expression and figure out their roles
class Analyzer
{
    private:
    std::vector<Token*> tokens; // using a vector because of its constant element access time since we are 
                                // looking at its elements while analyzing the input
    Type currentT;              // type of current token
    int idx;                    // index of that current token

    /// @brief Increments the current index and if it is valid - set the current type to the type at thqat index
    void next();

    /// @brief Helper function for the destructor
    void deallocate();

    /// @brief Returns a factor Element if the current index is valid, otherwise print an error in the stream
    /// @param - out - error stream
    Element* factor(std::ostream& out);

    // The expression consists of terms which in out case are function names and factors 
    /// @brief Returns the found expression, if an error occurs - print it in the stream
    /// @param out - error stream
    Element* expression(std::ostream& out);

    public:
    explicit Analyzer(const std::vector<Token*>& tokens);
    Analyzer(const Analyzer& other) = delete;   // not needed for the logic
    Analyzer& operator =(const Analyzer& other) = delete;  
    ~Analyzer();

    /// @brief Looks at the current index. If it is -1 -> returns nullptr, calls expression() otherwise.
    /// If an error occurs, we print it in the given stream
    /// @param out - error stream
    Element* analyze(std::ostream& out);
};