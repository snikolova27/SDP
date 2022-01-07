#pragma once
#include <cstddef>
#include <vector>
#include <functional>

//#include "pair.h"

class Multiset
{
    private:
    std::vector<std::string> elements;
    std::vector <int> occurences;

    public:
    Multiset() = default;
    ~Multiset() = default;

    /// @brief Adds a single element
    /// @param elem - element to add
    void add(const std::string& elem);

    /// @brief Adds a single element multiple times
    /// @param elem - element to add
    /// @param times - how many times to add it
    void add(const std::string& elem, const std::size_t times = 1);

    /// @brief Returns if element has already been added - uses binary search
    bool isIn(const std::string& elem) const;   

    /// @brief Returns the occurences of an element in a given file
    int getOccurencesOfIn(const std::string& elem, bool file = true) const; //file = true would mean the first file, file = false => second file

    /// @brief Fill the vector of elements from given stream
    /// @param a -  stream
    void fill(std::istream& a);

//private:
    ///@brief Returns the index of the key in an array, -1 if not found
    /// @param v - vector to search in
    /// @param leftIdx - lower boundary
    /// @param rightIdx - upper boundary
    /// @param key - element we are searching for
    int binarySearch(const std::vector <std::string> v, int leftIdx, int rightIdx, const std::string& key) const;

    /// @brief Uses binary search to find the insertion position of an element, returns index 
    /// @param v - vector to search in
    /// @param elem - elem we want to add
    int findInsertionPos(const std::vector <std::string> v, const std::string& elem );

};
