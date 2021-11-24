#include <iostream>
#include <string>
#include <stack>

char oppositeBracket(const char &ch)
{
    if (ch == ')')
    {
        return '(';
    }
    if (ch == ']')
    {
        return '[';
    }
    if (ch == '}')
    {
        return '{';
    }
    return '\0';
}

bool isOpening(const char &ch)
{
    return ch == '(' || ch == '{' || ch == '[';
}

bool isClosing(const char &ch)
{
    return ch == ')' || ch == '}' || ch == ']';
}

std::string isBalanced(std::string s)
{
    std::size_t len = s.size();
    std::stack<char> brackets;

    for (std::size_t i = 0; i < len; i++)
    {
        if (isOpening(s[i]))
        {
            brackets.push(s[i]);
        }
        else if (isClosing(s[i]))
        {
            if (!brackets.empty() && brackets.top() == oppositeBracket(s[i]))
            {
                brackets.pop();
            }
            else
            {
                return "NO";
            }
        }
    }

    if (!brackets.empty())
    {
        return "NO";
    }
    
    return "YES";
}

int main()
{
    std::cout << isBalanced("{[()]}") << std::endl;
    std::cout << isBalanced("{[(])}") << std::endl;
    std::cout << isBalanced("{{[[(())]]}}") << std::endl;
    return 0;
}
