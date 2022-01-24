#include "analyzer.h"
#include "commandTokens.h"
#include "exceptions.h"
#include "expressionElements.h"
#include <ostream>

Analyzer::Analyzer(const std::vector<Token*>& tokens) : tokens(tokens), idx(-1)
{
    this->next();
}

Analyzer::~Analyzer()
{
    this->deallocate();
}

void Analyzer::deallocate()
{
    const int size = this->tokens.size();
    for(int i = 0; i < size;i++)
    {
        delete this->tokens[i];
        this->tokens[i] = nullptr;
    }
}
void Analyzer::next()
{
    if(this->idx + 1 < this->tokens.size()) // if the next index is not out of bounds.
                                            // we increment the index and get the next token type
    {
        this->idx++;
        this->currentT = this->tokens[this->idx]->type;
    }
    else 
    {
        this->idx = -1;     // -1 is invalid index
    }
}

Element* Analyzer::factor(std::ostream& out)
{
    if(this->idx != -1)
    {
        FactorElement* factor = new FactorElement(this->tokens[this->idx]);
        this->next();
        return factor;
    }

    InvalidSyntax("A number was expected but it was not given").print(out); // print error if a factor is not found
    return nullptr; 
}

Element* Analyzer :: expression(std::ostream& out)
{
    auto& type = this->currentT;
    auto& idx = this->idx;

    if(type != Type::NUMBER)    // if it's not a number, then it must be a function name or a bracket
    {
        if(type != Type::NAME_OF_FUNCTION)  // if it's not a function name, then it must be a closing bracket
        {
            if(type == Type::CLOSING_BRACKET || type == Type::CLOSING_SQUARE_BRACKET)
            {
                this->next();
                return nullptr;
            }

            // it wasn't a closing bracket of any type so we throw an error
            InvalidSyntax("Function name was expected. Input was: ").print(out);
            this->tokens[idx]->print(out);
            out << std::endl;
            return nullptr;
        }

        const auto& op = this->tokens[idx]; // we save the parent token (it holds the operation)
                                                  // then we search for its arguments

        this->next(); // move onto the next token

        FunctionT* funcPtr = dynamic_cast<FunctionT*>(op); // will be used to figure out what the operation is

        // -------- LIST --------
        if(funcPtr && funcPtr->name == LIST)    // we are looking at a list function declaration
        {
            if(idx == -1 || type != Type::OPENING_BRACKET)  // idx == -1 means we have reached the end 
                                                                    // of the tokens vector, if the current type
                                                                    // is not an opening bracket that means we 
                                                                    // have found a syntax error
                                                                    // list(<args>) - the right syntax
            {
                InvalidSyntax("An opening bracket '(' was expected after calling the list function.").print(out);
                return nullptr;
            }

            this->next(); // move onto the next token since we have found an opening bracket for the list

            std::vector<Element*> args;  // store all the arguments for the list function

            // now search for the arguments
            do
            {
                if(idx == -1)   // we have reached the end of the tokens vector while searching for arguments
                                // but a closing bracket was not found so we throw an error
                {
                    InvalidSyntax("Invalid list declaration. Closing bracket is expected at the end of it.").print(out);
                    return nullptr;
                }
                else if(type == Type::COMMA)    // this is okay
                {
                    this->next(); // move onto the next token
                }

                args.push_back(this->expression(out)); // using recursion, find the arguments

                const int size = this->tokens.size();

                if(type == Type::CLOSING_BRACKET &&
                    (idx + 1 >= size || this->tokens[idx + 1]->type == Type::CLOSING_BRACKET
                    || idx + 2 >= size || this->tokens [idx + 2]->type == Type::NAME_OF_FUNCTION))
                {
                    if(idx + 1 < size && this->tokens[idx + 1]->type == Type::CLOSING_BRACKET)  
                    {
                        this->next();
                    }
                    break;
                }
                this->next();

                if( idx != -1 && type == Type::CLOSING_BRACKET)
                {
                    this->next();
                    if(idx == -1)   // we have reached the end
                    {
                        break; 
                    }
                    else if(type == Type::COMMA)
                    {
                        this->next();
                    }
                }
            }while (idx != -1 && 
                    (type == Type::NAME_OF_FUNCTION || type == Type::COMMA || type == Type::NUMBER));

            return new ListOperation(op, args); // return the list operation and its arguments
        } 
    }
    return nullptr;
}