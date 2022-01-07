#include "multiSet.h"

 bool Multiset::isIn(const std::string& elem) const   //implements binary search
{        
        const int size = this->elements.size();
        const int index = this->binarySearch(this->elements, 0, size - 1, elem);
        return (index != -1);  //index -1 would mean that the element is not present
}

int Multiset::binarySearch(const std::vector <std::string> v, int leftIdx, int rightIdx, const std::string& key) const
{
    if (rightIdx >= leftIdx) 
    {
        int middle = leftIdx + (rightIdx - leftIdx) / 2;

        // If the element is present it the middle
        if (v[middle] == key) return middle;

        // If element is smaller than mid, search in the left subvector
        if (v[middle] > key) return binarySearch(v, leftIdx, middle - 1, key);

        // Search in the right subvector otherwise
        return binarySearch(v, middle + 1, rightIdx, key);
    }

    return -1;  // element is not present 
}

int Multiset::findInsertionPos(const std::vector <std::string> v, const std::string& elem ) //binary search inside
{
        int end =  this->elements.size() - 1;
        int start = 0;
       
       while (start <= end)
       {
           int middle = (start + end) / 2;

           if( v [middle] == elem) return middle;
           else if(v[middle] < elem) start = middle + 1;  //look at the right subvector
           else end = middle - 1;  // look at the left subvector
       }
       return end + 1; //insert position
}

