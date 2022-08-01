#pragma once
#include "node.h"
#include <exception>
#include <list>
#include <string>
#include <iostream>

class N_aryTree{
    private:
    Node* root;
    std::string name;

    // @Brief check if a character is a digit
    bool isDigit(char character);

  // @Brief convert a character into an int
    int convertToInt(char character);

    //@Brief free allocated memory for the tree
    void cleanTree();

    // @Brief check if the tree contains such a node already
    bool contains(Node* node, int value);

    // @Brief add a node to a specified root
    bool add(Node* toAdd, Node* root);

    // @Brief load tree from a given string
    bool readTree(const std::string& sourceString);

    public:
    // @Brief create tree from a given string
    N_aryTree(const std::string& name, const std::string& sourceString);

    //@Brief create an empty tree with a given name and root value
    N_aryTree(std::string& name,  int rootValue);

    ~N_aryTree();

    // @Brief check if the tree contains such a value
    bool contains(int value);
    
    // @Brief check if the tree contains another tree
    bool contains(N_aryTree tree);

    // @Brief print tree
    void print(std::ostream& streamToPrint);

    // @Brief save tree in given file
    void saveTree(std::string& filePath);

    // @Brief add value to tree 
    bool add(int value);

    void bfs(Node* node,std::ostream& streamToPrint);
};