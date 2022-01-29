#include <iostream>
#include "list.h"

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

int cntConsecutive(const int value, Node* start)
{
    if(start->value == value)
    {
        if(start->next)
        {
            return 1 + cntConsecutive(value, start->next);
        }
        //return 1;
        
    }
    return 0;
}
List formatList(List& list)
{
    Node* listHead = list.getHead();

    const int size = list.getSize();
  //  std::cout << size << std::endl;
    List result;
    int currentSize = 0;
    while(listHead && currentSize < size - 1)
    {
        int currentCnt = 1;
        int current = listHead->value;
        while(listHead->next->value == current && listHead->next && currentSize < size - 1)
            {
                //std::cout << "v while" << std::endl;
                currentCnt++;
                currentSize++;
               // std::cout << current << " -> " << currentCnt << std::endl;
                if(listHead->next && currentSize < size - 1)
                {
                   // std::cout << currentSize << std::endl;
                    //std::cout << "vlizam v if-a" << std::endl;
                    listHead = listHead->next;
                }
              
            }
        

        if(current != listHead->next->value && listHead->next && currentSize < size - 1)
        {
            listHead = listHead->next;
            currentSize++;
        }

        //std::cout << currentCnt  << " - " <<current<< std::endl;
        result.push(currentCnt);
        result.push(current);
        
      
    }

    return result;

}

int main()
{
    int size;
    std::cout << "Enter count of elements in list: ";
    std::cin >> size;
    while(size <=  0)
    {
        std::cout << "Size must be greater than 0! Try again" << std::endl;
        std::cin >> size;
    }

    List original;
    std::cout << "enter list elements: " << std::endl;
    for(int i = 0; i < size; i++)
    {
        int current;
        std::cin >> current;
        original.push(current);
    }
    std::cout << "You have entered: " << std::endl;
    printList(original);
    List result =formatList(original);
    std::cout << "List after formatting: " << std::endl;
    printList(result);
    return 0;
}