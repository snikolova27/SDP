#include "interface.h"

#include <cmath>
#include <istream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "wordCount.h"

/// @brief Return a vector of all words in a file
/// @param str - stream to read the words from
std::vector<std::string> readAllWords(std::istream & str)
{
    std::vector<std::string> result;
    std::string word;
    while (str >> word) result.emplace_back(word);
    return result;
}

/// @brief Return a sorted vector containing distinct words and their count 
/// @param sortedWords - vector of sorted words from which we will be extracting the words
std::vector<WordCount> countUnique(const std::vector<std::string> & sortedWords)
{
    std::vector<WordCount> result;
    if (sortedWords.empty())
    {
        return result;
    }

    WordCount current = { sortedWords[0], 1 };
    for (int i = 1; i < sortedWords.size(); i++)
    {
        const auto & s = sortedWords[i];
        if (s == current.word) current.count++;  //if word is already in, just increase the count of it
        else
        {
            result.push_back(current);
            current.word = s;
            current.count = 1;
        }
    }
    result.push_back(current);

    return result;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
    auto words1 = readAllWords(a);
    auto words2 = readAllWords(b);

    std::sort(words1.begin(), words1.end());
    std::sort(words2.begin(), words2.end());

    std::vector<WordCount> unique1 = countUnique(words1);
    std::vector<WordCount> unique2 = countUnique(words2);

    std::vector<WordCount> only1, only2, common;
    auto it1 = unique1.begin();
    auto it2 = unique2.begin();
    while (it1 < unique1.end() && it2 < unique2.end())  //looking at the two vectors at the same time, searching for common words
    {
        if (it1->word < it2->word) //if there is a word present in the first file, but not in the second
        {
            only1.push_back(*it1);
            ++it1;
        }
        else if (it1->word > it2->word) //if there is a word present in the second file, but not in the first
        {
            only2.push_back(*it2);
            ++it2;
        }
        else  //word is in both files
        {
            common.emplace_back<WordCount>({ it1->word, std::min(it1->count, it2->count) });  
            if (it1->count > it2->count) //if a word appears more in the first file, add it to only1 
                                        // with count equal to the difference between its count in the first file and in the second 
            {
                only1.emplace_back<WordCount>({it1->word, it1->count - it2->count});
            }
            else if (it1->count < it2->count)   //same thing as above but for the second file
            {
                only2.emplace_back<WordCount>({it1->word, it2->count - it1->count});
            }
            ++it1;
            ++it2;
        }
    }
    while (it1 < unique1.end())  //if there are any words left in the first file, that are not in the second
    {
        only1.push_back(*it1);
        ++it1;
    }
    while (it2 < unique2.end()) //same as above but for the second file
    {
        only2.push_back(*it2);
        ++it2;
    }

    ComparisonReport result;
    result.commonWords = WordsMultiset(common);
    result.uniqueWords[0] = WordsMultiset(only1);
    result.uniqueWords[1] = WordsMultiset(only2);
    return result;
}

int Comparator::getPercent(std::istream& a, std::istream& b, std::istream& a2) const
{
    double result = 0.0;
    auto words1 = readAllWords(a2);
    std::sort(words1.begin(), words1.end());

    int cnt1 = words1.size();
    std::vector<WordCount> unique1 = countUnique(words1);

    double cntCommon = 0;
    int size = unique1.size();

    Comparator cmp;
    const auto res = cmp.compare(a, b);
   
    for(int i = 0; i< size;i++)
    {
        auto& current = unique1[i].word;
        cntCommon+= res.commonWords.countOf(current);
    }

    result = cntCommon / cnt1;

    return (int) (result * 100);
}