#include <exception>
#include <iostream>
#include "list.h"
#include <cmath>
const double ESP =  1e-5;

void printList(List& list)
{
    Node* head = list.getHead();
    while(head)
    {
        std::cout << head->value << " ";
        head = head->next;
    }
    std::cout << std::endl;
}
double fixList(const double prev, Node* currentNode)
{
    if(currentNode->next)
    {
        const double nextValue = currentNode->next->value;
        const double avg = (prev + nextValue) / 2;

        if(std::fabs(currentNode->value - avg)< ESP)
        {
            return -1;
        }
        return currentNode->value;
    }
    else
    {
        if(std::fabs((prev + ESP) / 2 - currentNode->value) < ESP)
        {
            return -1;
        }
        return currentNode->value;
    }
  
}

List fixList(Node* listHead)
{
    List result;
    double headValue = listHead->value;
    double next = listHead->next->value;
    //looking at the head of the list first
    if(std::fabs(headValue - (next + ESP) / 2 ) <=  ESP)
    {
        
    }
    else
    {
        result.push(headValue);
    }
    listHead = listHead->next;
    while(listHead)
    {
        //std::cout << headValue << " head value first" << std::endl;
        double current = fixList(headValue, listHead);
        if(current != -1)
        {
            result.push(current);
        }
        headValue = listHead->value;
       // std::cout << headValue << " head value after" << std::endl;
        listHead = listHead->next;
    }
    return result;
}

int main()
{
    List original;
    int size;
    std::cout << "How many elements will you enter? ";
    std::cin >> size;
    if(size > 0)
    {
        for(int i = 0; i < size; i++)
        {
            double current;
            std::cin >> current;
            original.push(current);
        }
           std::cout << "You have entered the following list: ";
           printList(original);

           List result = fixList(original.getHead());
           std::cout << "List after fixing: ";
           printList(result);
        
    }
    else 
    {
        std::cout << "Count of elements should be more than 0" << std::endl;
    }

 
    return 0;
}