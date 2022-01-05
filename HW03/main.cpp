#include "include/interface.h"
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
    return 0;
}