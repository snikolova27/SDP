#include "list.h"
#include <exception>
#include <iostream>

List getLongestList(const List& l)
{
    int maxLen = 0;
    int maxSum = 0;
    Node* maxHead = nullptr;

    Node* start = l.getHead();

    while(start)
    {
        Node* next = start->next;
        if(start->next)
        {
            if(start->next->value > start->value)
            {
                Node* currentHead = start;
                int currentLen = 2;
                int currentSum = start->value + start->next->value;
                start = next;
                while(start->next && start->next->value > start->value)
                {
                    currentLen++;
                    currentSum+= start->next->value;
                    start = start->next;
                  
                }
                if(currentLen > maxLen || currentSum > maxSum)
                {
                    maxLen = currentLen;
                    maxSum = currentSum;
                    maxHead = currentHead;
                }

            }
            else if( start->next->value < start->value)
            {
                Node* currentHead = start;
                int currentLen = 2;
                int currentSum = start->value +start->next->value;
                start = next;
                while(start->next && start->next->value < start->value)
                {
                    currentLen++;
                    currentSum+= start->next->value;
                    start = start->next;
                }
                if(currentLen > maxLen || currentSum > maxSum)
                {
                    maxLen = currentLen;
                    maxSum = currentSum;
                    maxHead = currentHead;
                }
            }
            else 
            {
                if(maxLen == 0 && start->value > maxSum)
                {
                    maxLen = 1;
                    maxSum = start->value;
                    maxHead = start;
                }
                start = start->next;
            }
        }
        else 
        {
         start = start->next;
        }
    } 

    List result;
    for(int i = 0; i < maxLen; i++)
    {
        result.push(maxHead->value);
        maxHead = maxHead->next;
    } 

    return result;
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

    Node* head = original.getHead();
    while(head)
    {
        std::cout << "element: " <<  head->value << std::endl;
        head = head->next;
    }
    std::cout << std::endl;

    List result = getLongestList(original);
    Node* headRes = result.getHead();
    std::cout << "In main: " << std::endl;
    while(headRes)
    {
        std::cout << headRes->value << " " ;
        headRes = headRes->next;
    }
    std::cout << std::endl;
    return 0;
}