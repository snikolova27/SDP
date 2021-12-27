#include "include/input.h"

int main()
{
    string fileName = "test1.txt";
    Input in;
    in.readInput(fileName);
    in.printConnections();
    return 0;
}