#include "interpreter.h"
#include "commandTokens.h"
#include "exceptions.h"
#include "expressionElements.h"

Interpreter::Interpreter() : idx(0) {}

Interpreter::~Interpreter() 
{
    this->deallocate();
}

void Interpreter::deallocate()
{
    const int size = this->funcs.size();
    for(int i = 0; i < size; i++)
    {
        delete this->funcs[i];
        this->funcs[i] = nullptr;
    }
}

bool Interpreter::lookAtFactor(const FactorElement* element, std::ostream& out)
{
    // trying to get the number from the element
    const NumberT* num = dynamic_cast<const NumberT*>(element->token); 
    if(num)
    {
        this->results.push(num->value); // if everything is okay, add it to the stack
        return true;
    }
    RunTime("A number was expected").print(out); // print error otherwise
    return false;
}

bool Interpreter::lookAtArgument(const ArgElement* element, std::ostream& out)
{
    // trying to get the arguments for a function call
    const ArgumentT* arg = dynamic_cast<const ArgumentT*>(element->token);
    if(arg)
    {
        if(arg->value >= this->args.size()) 
        {
            RunTime("Not enough arguments provided for function call").print(out);
            return false;
        }

        this->results.push(this->args[this->idx + arg->value]); // push the argument in the stack
        return true;
    }

    RunTime("An argument was expected").print(out);
    return false;
}

bool Interpreter::lookAtIf(const IfOperation* op, std::ostream& out)
{
    if(!this->lookAt(op->condition, out)) {return false;}
    if(this->results.top() == 0)
    {
        return this->lookAt(op->right,out);
    }

    return this->lookAt(op->left,out);
}

bool Interpreter::list(const ListOperation* list, std::ostream& out)
{
    out << '[';
    auto it = list->list.begin();
    for(it;it + 1 != list->list.end(); ++it)
    {
        if(!this->lookAt(*it, out)) {return false;}
        out << this->results.top() << ", ";
        this->results.pop();
    }
    if(!this->lookAt(*it,out)) {return false;}

    out << this->results.top() << "]";
    this->results.pop();
  
    return true;
}

std::vector<Element*>  Interpreter::map(const MapOperation* map, std::ostream& out)
{
    
}

