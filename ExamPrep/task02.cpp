#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "binaryTree.h"

std::vector<int> readNums(std::istream & str)
{
    std::vector<int> result;
    int num;;
    while (str >> num) result.emplace_back(num);
    return result;
}

int main(int argc, char **argv)
{
    const std::string name = argv[1];
    std::ifstream file(name);

    std::vector<int> nums = readNums(file);
    BST tree;
    for(int i = 0;i < nums.size(); i++)
    {
        std::cout << nums[i] << " ";
        tree.insert(nums[i]);
        
    }
    std::cout << std::endl;
    std::vector <int> even = tree.getEvenChildren(tree.getRoot()); 
    for(int i = 0; i < even.size(); i++)
    {
        std::cout <<even [i] << " ";
    }

    // ADD ERASE AND FIND FUNCTION!

   

    return 0;
}