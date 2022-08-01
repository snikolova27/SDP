#pragma once
#include <vector>
#include <string>

struct CommandTokens
{
    std::string command;
    std::vector<std::string> arguments;
};

CommandTokens tokenize(const std::string & command);