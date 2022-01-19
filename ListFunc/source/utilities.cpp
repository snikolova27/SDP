#include "utilities.h"

bool isCharacter(const char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isDigit(const char ch)
{
    return ch >= '0' && ch <= '9';
}