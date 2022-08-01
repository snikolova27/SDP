#include "../include/commandTokenizer.h"
#include <stdexcept>


inline bool isWhitespace(const char c)
{
    return c == ' ' || c == '\t';
}

CommandTokens tokenize(const std::string &inputStr)
{
    CommandTokens result;

    const size_t len = inputStr.length();
    size_t idx = 0;

    // Find command
    while (idx < len && isWhitespace(inputStr[idx]))
    {
        idx++;
    }

    // Check why we stopped - end of string or we reached the beggining of the command?
    if (idx == len)
    {
        throw std::invalid_argument("Empty command string");
    }

    const size_t cmdStartIdx = idx;
    if (inputStr[cmdStartIdx] == '"')
    {
        throw std::invalid_argument("Command cannot start with \": " + inputStr);
    }

    // Find end of command
    while (idx < len && !isWhitespace(inputStr[idx]))
    {
        idx++;
    }
    if (idx > 0 && inputStr[idx - 1] == '\"')
    {
        throw std::invalid_argument("Command cannot end with \": " + inputStr);
    }

    // Command is valid, convert to uppercase and assing to output
    {
        std::string inCommand = inputStr.substr(cmdStartIdx, idx - cmdStartIdx);
        for (size_t i = 0; i < inCommand.size(); i++)
        {
            inCommand[i] = std::toupper(inCommand[i]);
        }
        result.command = inCommand;
    }

    // Find arguments
    while (idx < len)
    {
        // Find start of argument
        while (idx < len && isWhitespace(inputStr[idx]))
        {
            idx++;
        }
        if (idx == len)
        {
            break; // end of string
        }

        if (inputStr[idx] == '"')
        {
            // Find matching "
            int closingIdx = idx + 1;
            while (closingIdx < len && inputStr[closingIdx] != '"')

            {
                closingIdx++;
            }
            if (closingIdx == len)
            {
                throw std::invalid_argument("Unmatched quoted argument: " + inputStr);
            }

            std::string argument = inputStr.substr(idx + 1, (closingIdx - 1) - (idx + 1) + 1);
            result.arguments.push_back(argument);
            idx = closingIdx + 1;
        }
        else
        {
            int endIdx = idx + 1;
            while (endIdx < len && !isWhitespace(inputStr[endIdx]) && inputStr[endIdx] != '\"')
            {
                endIdx++;
            }
            if (endIdx < len && inputStr[endIdx] == '\"')
            {
                throw std::invalid_argument("Found closing quote without a starting one: " + inputStr);
            }

            std::string argument = inputStr.substr(idx, endIdx - idx);
            result.arguments.push_back(argument);
            idx = endIdx + 1;
        }
    }

    return result;
}