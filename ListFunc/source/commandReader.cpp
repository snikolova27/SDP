#include "commandReader.h"

commandReader::commandReader(const std::string& input) : input(input){};

bool commandReader::tokenize(std::vector<Token*>& tokens, std::ostream& errors)
{
    int cntBrackets = 0; // +1 if we come across an opening bracket '(', -1 if we come across a closing bracket ')'
    int cntSquareBrackets = 0; // same thing but for square brackets

    auto& input = this->input;

    for(auto it = input.begin(); it != input.end(); ++it)
    {   
        while(*it == ' ' || *it == '\t' )  // skipping spaces and tabs
        {
            ++it;
        }

        if(isLetter(*it))  // we are searching for a name of a function or a list, they only consist of letters
        {
            std::string name;
            do 
            {
                name += *it;
                ++it;
            }while (isLetter(*it) && it != input.end());   // searching while we the current character is a letter 
                                                              // and while we haven't reached the end

            tokens.emplace_back(new FunctionT(name));  // store the name we found in the tokens vector
        }

        else if(isDigit(*it) || *it == '-') // we are searching for a number, could be a negative, could be a double/integer
        {
           bool isNegative = false; // we will use it to mark if the number is negative, will be used for the result in the end

            if(*it == '-')
            {
                isNegative = true;
                ++it;
            }

            double num = 0;    // storing the number here

            bool point = false;  // will be used to mark if the number is a double

            double powerOf10 = 1;  // will be used to divide the number at the end

            do 
            {
                if(*it == '.')  // we have found a point
                {
                    if(point)  // if we have already found a point before the current one
                               // that means input is invalid
                    {
                        std::string ch;
                        ch += *it;
                        int index = it - input.begin();

                        InvalidCharacter(ch,input,index).print(errors);  // print the error in the errors stream
                        tokens.clear(); // clean all tokens
                        return false; // input is not valid
                    }

                    point = true; // this is the first point we encounter
                }
                else
                {
                    num = num * 10 + (*it - '0');  // (*it - '0') so that we get the number, not the character
                    if(point)   // if we will have to divide the number later, we increase the power of 10
                    {
                        powerOf10 *= 10;
                    }

                }
                ++it;

            }while (it != input.end() && (isDigit(*it) || *it == '.')); // searching while we haven't reached the end
                                                                        // and the current character is a digit or a point

            tokens.emplace_back(new NumberT(isNegative ? (num / powerOf10) * (-1) : (num / powerOf10))); // store the number 
                                                                                                         // in the tokens vector
        }

        if( it == input.end())  // we have reached the end of the input
        {
            break;
        }

        while(*it == ' ' || *it == '\t')    // skipping spaces and tabs once again
        {
            ++it;
        }

        switch(*it)
        {
            case '#':   // searching for a number after this symbol
            {
                ++it; // incrementing to the next character in the input
                
                if(it == input.end() || !isDigit(*it))  // if we have reached the end or the current character is not a digit
                                                        // that means the syntax is not valid so we throw an error
                {
                    std::string ch;
                    ch += *it;
                    int index = it - input.begin();

                    InvalidCharacter(ch,input,index).print(errors); // print the error in the errors stream
                    tokens.clear(); // clean all tokens
                    return false; // input is not valid
                }

                unsigned arg = 0; // should be an integer >= 0
                do 
                {
                    arg = arg * 10 + (*it - '0'); // same thing we did while searching for a number
                    ++it;
                }while (it != input.end() && isDigit(*it));

                tokens.emplace_back(new ArgumentT(arg)); // store the argument in the tokens vector
                --it; // decrement so that we don't skip a character
                break;
            }


            // in the next 4 cases we are handling brackets
            
            case '(':
            {
                cntBrackets++;  // we have found an opening bracket
                tokens.emplace_back(new Token(Type::OPENING_BRACKET)); // store the bracket in the tokens vector
                break;
            }

            case ')':
            {
                if(cntBrackets == 0)    // would mean that we don't have a corresponding opening bracket for this closing one
                                        // so we have to throw an error
                {
                    InvalidSyntax("No matching opening bracket '(' for a closing bracket ')'").print(errors); // print the error in the errors stream
                    tokens.clear(); // clean all tokens
                    return false; // input is not valid
                }

                cntBrackets--; 

                tokens.emplace_back(new Token(Type::CLOSING_BRACKET)); // store the bracket in the tokens vector
                break;
            }

            case '[':
            {
                cntSquareBrackets++;
                tokens.emplace_back(new Token(Type::OPENING_SQUARE_BRACKET)); // store the bracket in the tokens vector
                break;
            }

            case ']':
            {
                if(cntSquareBrackets == 0)  // would mean that we don't have a corresponding opening bracket for this closing one
                                            // so we have to throw an error
                
                {
                    InvalidSyntax("No matching opening bracket '[' for a closing bracket ']'").print(errors); // print the error in the errors stream
                    tokens.clear(); // clean all tokens
                    return false; // input is not valid

                }

                cntSquareBrackets--;

                tokens.emplace_back(new Token(Type::CLOSING_SQUARE_BRACKET)); // store the bracket in the tokens vector
                break;
            }

            case ',':
            {
                tokens.emplace_back(new Token(Type::COMMA)); // store the comma in the tokens vector
                break;
            }

            case '-':
            {
                ++it;
                while(*it == ' ' || *it == '\t')    // skipping spaces and tabs once again
                {
                    ++it;
                }
       
                if(*it != '>')  // cannot make an arrow, we throw an error
                {
                    errors << *it << std::endl;
                    InvalidSyntax("No matching operator for '-'").print(errors); // print the error in the errors stream
                    tokens.clear(); // clean all tokens
                    return false; // input is not valid
                }
                tokens.push_back(new Token(Type::ARROW)); // store the arrow in the tokens vector
                break;
            }

            default:    // that means the language doesn't recognize the symbol as valid
                        // so we throw an error
            {
                std::string ch;
                ch += *it;
                int index = it - input.begin();

                InvalidCharacter(ch,input,index).print(errors); // print the error in the errors stream
                tokens.clear(); // clean all tokens
                return false; // input is not valid
            }

        }
    }

    if(cntBrackets != 0)    // there is an opening bracket ( without a corresponding closing one
    {
        Exception(INVALID_SYNTAX, "Expeced a closing bracket ')'").print(errors); // print the error in the errors stream 
        tokens.clear(); // clean all tokens
        return false; // input is not valid
    }

    if(cntSquareBrackets != 0)    // there is an opening bracket [ without a corresponding closing one
    {
        Exception(INVALID_SYNTAX, "Expeced a closing bracket ']'").print(errors); // print the error in the errors stream 
        tokens.clear(); // clean all tokens
        return false; // input is not valid
    }

    return true;
}

