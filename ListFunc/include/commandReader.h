#pragma once
#include "exceptions.h"
#include "commandTokens.h"
#include "types.h"
#include "utilities.h"
#include <vector>

/// @brief Will be used to read the input and tokenize it so we could interpret the input later.
/// If there is an error of any kind, we will throw it.
/// The personalised errors are defined in exceptions.h 
class commandReader
{
    private:
    std::string input;

    public:
    commandReader(const std::string& input);

    // Won't be needed for the logic
    commandReader(const commandReader& other) = delete;
    commandReader& operator = (const commandReader& other) = delete;

    /// @brief Tokenizing the input, using a vector for constant access to its elements
    /// @param tokens - vector of tokens
    /// @param errors - stream in which we will be printing the errors if there are any
    bool tokenize(std::vector<Token*>& tokens, std::ostream& errors);
};