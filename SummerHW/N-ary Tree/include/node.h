#pragma once
#include <vector>

struct Node{
    std::vector <Node*> children;
    int value;

    Node(int value){
        this->value = value;
    }
};

