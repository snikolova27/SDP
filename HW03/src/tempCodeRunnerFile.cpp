 bool Multiset::isIn(const std::string& elem) const   //implements binary search
{        
        const int size = this->elements.size();
        const int index = this->binarySearch(this->elements, 0, size - 1, elem);
        return (index != -1);  //index -1 would mean that the element is not present
}