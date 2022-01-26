#pragma once
#include <iostream>
#include "interpreter.h"
#include "analyzer.h"

/// @brief Helper function to check if a given character is a letter
/// @param ch - character to look at
bool isLetter(const char ch);

/// @brief Helper function to check if a given character is a digit
/// @param ch - character to look at
bool isDigit(const char ch);

/// @brief Driver function that uses all classes
void run(std::istream& in, std::ostream& out);