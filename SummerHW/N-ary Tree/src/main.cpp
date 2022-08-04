#include <iostream>
#include "../include/n-aryTree.h"

int main(int argc, char* argv[]){
    std::string name = "test";
    std::string filePath = "./test.txt";
    N_aryTree tree(name, filePath);
    tree.print(std::cout);


}