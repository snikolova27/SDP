#include "cmpHash.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

ComparisonReport compareHash(const std::vector<std::string> & words1, const std::vector<std::string> & words2)
{
    struct Count
    {
        size_t cnt1 = 0;
        size_t cnt2 = 0;
    };

    std::unordered_map<std::string, Count> hashMap;
    for (const auto & s : words1) hashMap[s].cnt1++;
    for (const auto & s : words2) hashMap[s].cnt2++;

    std::vector<WordCount> wc1, wc2, wcc;
    for (auto it = hashMap.begin(); it != hashMap.end(); it++)
    {
        const auto & word = it->first;
        const auto count = it->second;
        if (count.cnt1 > count.cnt2) wc1.push_back({word, count.cnt1 - count.cnt2});
        else if (count.cnt2 > count.cnt1) wc2.push_back({word, count.cnt2 - count.cnt1});
        
        const size_t commonCnt = std::min(count.cnt1, count.cnt2);
        if (commonCnt) wcc.push_back({word, commonCnt});
    }

    std::sort(wc1.begin(), wc1.end());
    std::sort(wc2.begin(), wc2.end());
    std::sort(wcc.begin(), wcc.end());

    ComparisonReport result;
    result.uniqueWords[0] = WordsMultiset(wc1);
    result.uniqueWords[1] = WordsMultiset(wc2);
    result.commonWords = WordsMultiset(wcc);
    return result;
}