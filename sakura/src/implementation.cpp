#include "interface.h"
#include "wordCount.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

bool WordsMultiset::contains(const std::string& word) const
{        
    return std::binary_search(wordCounts.begin(), wordCounts.end(), WordCount{ word });
}

void WordsMultiset::add(const std::string& word, size_t times)
{
    WordCount wc { word, times };
    auto it = std::lower_bound(wordCounts.begin(), wordCounts.end(), wc);
    if (it == wordCounts.end())  // if word is not yet present, add it
    {
        wordCounts.emplace_back(wc);
    } 
    else if (it->word == word) // if word is present, increase its count
    {
        it->count += times;
    } 
    else
    {
        wordCounts.insert(it, wc); //insert it at its proper place to keep the vector sorted
    } 
}

size_t WordsMultiset::countOf(const std::string& word) const
{
    const auto it = std::lower_bound(wordCounts.begin(), wordCounts.end(), WordCount{ word });
    if (it == wordCounts.end()) // if word is not found
    {
        return 0;
    } 
    if (it->word != word) 
    {
        return 0;
    } 
    return it->count; 
}

size_t WordsMultiset::countOfUniqueWords() const
{
    return wordCounts.size();
}

std::multiset<std::string> WordsMultiset::words() const
{
    std::multiset<std::string> result;
    for (const auto & wc : wordCounts)
    {
        for (size_t i = 0; i < wc.count; i++) result.insert(wc.word);
    }
    return result;
}

std::string WordsMultiset::wordAt(const int idx) const
{
    if(idx >= this->wordCounts.size() || idx < 0)
    {
        throw std::invalid_argument("Invalid index");
    }
    return this->wordCounts[idx].word;
}