#include "include/interface.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

void dumpWordCount(const std::vector<WordCount> & wordCounts)
{
    for (const auto & wc : wordCounts)
    {
        std::cout << wc.count << "\t" << wc.word << "\n";
    }
}

int main(int argc, char **argv)
{
    const int numOfArguments = argc;
    if(argc != 3)
    {
        std::cerr << "Usage: ./HW3 [file 1] [file 2]\n";
        return 1;
    }

    const std::string fileName1 = argv[1];
    const std::string fileName2 = argv[2];

    std::ifstream file1(fileName1);
    std::ifstream file2(fileName2);

    std::ifstream file1copy(fileName1);
    std::ifstream file2copy(fileName2);

    std::ifstream file1copy2(fileName1);
    std::ifstream file2copy2(fileName2);

    Comparator cmp;
    // const auto result = cmp.compare(file1, file2);

    // std::cout << "Words unique to " << fileName1 << ":\n";
    // const auto & wc1 = result.uniqueWords[0].getWordCounts();
    // dumpWordCount(wc1);

    // std::cout << "Words unique to " << fileName2 << ":\n";
    // const auto & wc2 = result.uniqueWords[1].getWordCounts();
    // dumpWordCount(wc2);

    // std::cout << "Words common to both files:\n";
    // const auto & wcc = result.commonWords.getWordCounts();
    // dumpWordCount(wcc);

    std::cout << "File1 / File2 : " << cmp.getPercent(file1, file2, file1copy) << " %" << std::endl;
    std::cout << "File2 / File1 : " << cmp.getPercent(file2copy, file1copy2, file2copy2) << " %" << std::endl;


    return 0;
}