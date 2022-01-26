#pragma once

#include <string>

/// @brief Used to store a word and how many times it occurs in a file
struct WordCount
{
    std::string word;
    size_t count = 0;

    bool operator<(const WordCount & other) const
    {
        return word < other.word;
    }
};