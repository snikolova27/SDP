#include "include/interface.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> nums = {1, 5, 6};
    const int v = 7;
    auto it = std::lower_bound(nums.begin(), nums.end(), v);
    nums.insert(it, v);
    for (int k : nums) std::cout << k << " ";
    std::cout << "\n";

    WordsMultiset wl;

    wl.add("w1");
    wl.add("w3");
    wl.add("w2");
    wl.add("w1");
    
    auto wcs = wl.getWordCounts();
    for (auto & wc : wcs) std::cout << wc.count << "\t" << wc.word << "\n";

    std::cout << wl.countOfUniqueWords() << "\n";

    std::cout << wl.contains("w2") << "\n";

    const size_t cnt3 = wl.countOf("w2");
    const size_t cnt2 = wl.countOf("w3");
	
    std::cout << cnt2 << " " << cnt3 << "\n";

    return 0;
}