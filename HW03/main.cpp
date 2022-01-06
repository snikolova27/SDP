#include "include/interface.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    const int numOfArguments = argc;
    if( argc != 3)
    {
        throw std::invalid_argument("Unsuffient count of arguments");
    }

    std::cout << "You have entered " << 2 << " arguments. They are: " << std::endl;
    for(int i =1 ; i < numOfArguments; i++)
    {
        std::cout << argv [i] << std::endl;
    }

    const std::string fileName1 = argv[1];
    const std::string fileName2 = argv[2];

    std::ifstream file1 (fileName1);
    std::ifstream file2 (fileName2);

    Comparator c;
    c.compare(file1, file2);

    return 0;
}