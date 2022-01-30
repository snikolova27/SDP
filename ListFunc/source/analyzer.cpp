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

Element* Analyzer::analyze(std::ostream& out)
{
    return this->idx == -1 ? nullptr : this->expression(out);
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

        // -------- USER FUNCTION --------
        if(idx == -1 || type != Type::OPENING_BRACKET)
        {
            if(type == Type::ARROW) // we have found a user defined function
            {
                this->next();
                return new UserFunc(op, nullptr, {});
            }

            if(type == Type::COMMA || type == Type::CLOSING_BRACKET)
            {
                this->next();
                return new UserFunc(op, nullptr, {});
            }

            if(idx != -1)
            {
                InvalidSyntax("An opening bracket '(' was expected or a list. Input was ").print(out);
                this->tokens[idx]->print(out);
                out << std::endl;
                return nullptr;
            }

            return new UserFunc(op, nullptr, {});
        }

        this->next(); // move onto next token

        if(idx == -1)   // we have reached the end of the input
        {
            InvalidSyntax("Input ends too soon.").print(out);
            return nullptr;
        }

        // -------- LEFT SIDE OF EXPRESSION --------
        Element* leftSide;

        switch(type)
        {
            case Type::NAME_OF_FUNCTION:
            {
                leftSide = this->expression(out); 
                this->next(); // move on
                break;
            }
            case Type::NUMBER:
            {
                leftSide = this->expression(out); // get the number
                break;
            }
            case Type::ARGUMENT:
            {
                leftSide = new ArgElement(this->tokens[idx]); // save the argument
                this->next(); // move on
                break;
            }
            default:
            {
                return new UserFunc(op, this->expression(out), {});
            }
        }

        if(idx == -1)
        {
             InvalidSyntax("A comma ',' was expected.").print(out);
             return nullptr;
        }

        if(type != Type::COMMA)
        {
            if(type == Type::CLOSING_BRACKET)   // found an unary operation
            {
                return new UnaryOperation(op, leftSide);
            }

            InvalidSyntax("A comma was expected ','. Input was: ").print(out);
            this->tokens[idx]->print(out);
            out << std::endl;
            return nullptr;
        }

        this->next(); // move onto next token

        // -------- RIGHT SIDE OF EXPRESSION --------
        Element* rightSide;
        
        switch(type)
        {
            case Type::NAME_OF_FUNCTION:
            {
                rightSide = this->expression(out); 
                this->next(); // move on
                break;
            }

            case Type::ARGUMENT:
            {
                rightSide = new ArgElement(this->tokens[idx]); // save the argument
                this->next(); // move on
                break;
            }
            default:
            {
                rightSide = this->factor(out);
            }
        }

        if(idx != -1 && type != Type::CLOSING_BRACKET)
        {
                if(type == Type::COMMA)
                {
                    FunctionT* funcPt = dynamic_cast<FunctionT*>(op);
                    if(funcPt && funcPt->name == IF) // found an if expression
                    {
                        this->next(); // move on
                        return new IfOperation(funcPt, leftSide, rightSide, this->expression(out));
                    }
                    else 
                    {
                        this->next(); // look at next token
                        std::vector<const Element*> args;
                        args.push_back(leftSide);
                        args.push_back(rightSide);
                        Element* nextArg = this->expression(out);

                        while(nextArg)  // find all arguments
                        {
                            args.push_back(nextArg);
                            this->next();
                            if(type == Type::COMMA)
                            {
                                this->next();
                            }
                            nextArg = this->expression(out);
                        }

                        return new UserFunc(op, nullptr, args);
                    }
                }
            
            InvalidSyntax("A closing bracket ')' was expected. Input was: ").print(out);
            this->tokens[idx]->print(out);
            out << std::endl;
            return nullptr;
        }
            return new BinaryOperation(op, leftSide, rightSide);  // we have found a binary operation
    }
   return this->factor(out);    // we have found a factor element
}