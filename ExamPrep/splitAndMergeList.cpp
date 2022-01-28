#include "list.h"
#include <iostream>
#include <vector>
#include <algorithm>

List separateAndMerge(const List& l)
{
    std::vector<int> even;
    std::vector<int> uneven;
    Node* head = l.getHead();
    while(head)
    {
        if(head->value % 2 == 0)
        {
            even.push_back(head->value);
            head = head->next;
        }
        else 
        {
            uneven.push_back(head->value);
            head = head->next;
        }
    }

    std::sort(even.begin(), even.end());
    std::sort(uneven.begin(), uneven.end());
    std::reverse(uneven.begin(), uneven.end());

    List evenL;
    for(int i = 0; i < even.size(); i++)
    {
        evenL.push(even[i]);
    }
    for(int i = 0; i < uneven.size();i++)
    {
        evenL.push(uneven[i]);
    }
  
    return evenL;
}

int main()
{
    List original;
    int size ;
    std::cout << "How many numbers in the list? ";
    std::cin >> size;
    for(int i = 0; i < size;i++)
    {
        int current;
        std::cin >> current;
        original.push(current);
    }

    List res = separateAndMerge(original);
    Node* headRes = res.getHead();
    while(headRes)
    {
        std::cout << headRes->value << " ";
        headRes = headRes->next;
    }
    
    return 0;
}