#include "interpreter.h"
#include "commandTokens.h"
#include "exceptions.h"
#include "expressionElements.h"
#include "predefinedFuncs.h"
#include <cmath>

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
    const FunctionT* funcName = dynamic_cast<const FunctionT*>(map->list->token);
    const FunctionT* listName = dynamic_cast<const FunctionT*>(map->factor->token);

    if(!funcName || !listName)
    {
        RunTime("Could not deduce function.").print(out);
        return {};
    }

    // finding the pointers
    const UserFunc* mapPtr = nullptr;
    const ListOperation* listPtr = nullptr;

    int j = 0;
    for(auto it = this->funcs.begin(); it != this->funcs.end(); ++it)
    {
        const UserFunc* currentFunc = dynamic_cast<const UserFunc*>(*it);
        if(currentFunc)
        {
            const FunctionT* currentName = dynamic_cast<const FunctionT*>(currentFunc->token);
            if(currentName->name == funcName->name || currentName->name == listName->name)
            {
                if(currentName->name == funcName->name) // found the name of the function
                {
                    mapPtr = dynamic_cast<const UserFunc*>(currentFunc);
                    j++;
                }
                else 
                {
                    listPtr = dynamic_cast<const ListOperation*>(currentFunc->definition);
                    ++j;
                }
            }
        }

        if(j == 2)  // function and map argument found
        {
            break;
        }
    }

    if(j < 2)
    {
        RunTime("Could not find matching function definition.").print(out);
        return {};
    }

    std::vector<Element*> elements;

    if(listPtr && mapPtr)
    {
        for(auto it = listPtr->list.begin(); it != listPtr->list.end(); ++it)
        {
            if(!this->lookAt(*it, out)) // if contents of list make an error occur
            {
                return {};
            }

            this->args.push_back(this->results.top());
            this->results.pop();

            if(!lookAt(mapPtr->definition, out)) // if there is a problem with the definition
            {
                return {};
            }

            ++this->idx;
            elements.push_back(new FactorElement(new NumberT(this->results.top())));
            this->results.pop();

            --this->idx;
            this->args.pop_back();
        }
    }

    return elements;
}

 bool Interpreter::unary(const UnaryOperation* op, std::ostream& out)
 {
     const FunctionT* funcToken = dynamic_cast<const FunctionT*>(op->token);
     if(funcToken ->name == SQRT)
     {
        auto arg = this->results.top();
        this->results.pop();
        this->results.push(sqrt(arg));
        return true;
     }
     if(funcToken->name == SIN)
     {
        auto arg = this->results.top();
        this->results.pop();
        this->results.push(sin(arg));
        return true;
     }
    if(funcToken->name == COS)
    {
        auto arg = this->results.top();
        this->results.pop();
        this->results.push(cos(arg));
        return true;
    }

    for(auto it = this->funcs.begin(); it != this->funcs.end(); ++it)
    {
        const UserFunc* currentPtr = dynamic_cast<const UserFunc*>(*it);

        if(currentPtr)
        {
            const FunctionT* currentName = dynamic_cast<const FunctionT*>(currentPtr->token);

            if(currentName->name == dynamic_cast<const FunctionT*>(op->token)->name)
            {
                int size = this->args.size();
                int difference = size - this->idx;
                this->idx = size == 0 ? 0 : size;

                this->args.push_back(this->results.top()); // get argument for function
                this->results.pop();

                if(!lookAt(currentPtr->definition,out)) { return false;}

                this->idx -= difference;
                this->args.pop_back();
                return true;
            }
        }
    }

    RunTime("Could not find matching function definition.").print(out);
    return false;
 }

bool Interpreter::binary(const BinaryOperation* op, std::ostream& out)
{
    // first we get the arguments of the binary operation
    double rightSide = this->results.top();
    this->results.pop();
    double leftSide = this->results.top();
    this->results.pop();

    const FunctionT* funcToken = dynamic_cast<const FunctionT*>(op->token);

    // checking to see if it's any of the predefined functions
  
    auto& name = funcToken->name;
    if(name == ADD)
    {
        this->results.push(leftSide + rightSide);
        return true;
    }
    if(name == SUB)
    {
        this->results.push(leftSide - rightSide);
        return true;
    }
    if(name == MUL)
    {
        this->results.push(leftSide * rightSide);
        return true;
    }
    if(name == DIV)
    {
        if( rightSide == 0)
        {
            RunTime("Division by 0 is not allowed.").print(out);
            return false;
        }
        this->results.push(leftSide / rightSide);
        return true;
    }
    if(name == POW)
    {
        this->results.push(pow(leftSide, rightSide));
        return true;
    }
    if(name == EQ)
    {
        this->results.push(leftSide == rightSide);
        return true;
    }
    if(name == LE)
    {
        this->results.push(leftSide < rightSide);
        return true;
    }
    if(name == NAND)
    {
        this->results.push(!leftSide || !rightSide);
        return true;
    }

    // search for the function in the user defined functions
    for(auto it = this->funcs.begin(); it != this->funcs.end(); ++it)
    {
        const UserFunc* currentPtr = dynamic_cast<const UserFunc*>(*it);
        if(currentPtr)
        {
            const FunctionT* currentName = dynamic_cast<const FunctionT*>(currentPtr->token);
            auto& name = currentName->name;
            if(name == dynamic_cast<const FunctionT*>(op->token)->name)
            {
                int size = this->args.size();
                int difference = size - this->idx;
                this->idx = size == 0 ? 0 : size;

                this->args.push_back(leftSide);
                this->args.push_back(rightSide);

                if(!lookAt(currentPtr->definition, out)) {return false;}

                this->idx -= difference;
                this->args.pop_back();
                this->args.pop_back();

                return true;
                
            }
        }
    }

    RunTime("Could not find matching function definition.").print(out);
    return false;
}

 bool Interpreter::user(const UserFunc* op, std::ostream& out)
 {
     for(auto it = this->funcs.begin(); it != this->funcs.end(); ++it)
     {
         const UserFunc* currentPtr = dynamic_cast<const UserFunc*>(*it);

         if(currentPtr)
         {
             const FunctionT* currentName = dynamic_cast<const FunctionT*>(op->token);
             auto& name = currentName->name;
             if(name == dynamic_cast<const FunctionT*>(op->token)->name)
             {
                 if(op->definition)
                 {
                     RunTime("A function with the same name already exists.").print(out);
                     return false;
                 }
                 if(op->args.size() == 0)
                 {
                     return this->lookAt(currentPtr->definition, out);
                 }
                 for( auto it2 = op->args.begin(); it2 != op->args.end(); ++it2)
                 {
                     if(!this->lookAt(*it, out)) {return false;}
                 }

                 int size = this->args.size();
                 int difference = size - this->idx;
                 this->idx = size == 0 ? 0 : size;

                 size = this->results.size()  + idx;

                 if( size > this->args.size())
                 {
                     this->args.resize(size);
                 }

                 while(!this->results.empty() && size)
                 {
                     // putting the function arguments in the args vector
                     this->args[--size] = this->results.top();
                     this->results.pop();
                 }

                 if(!this->lookAt(currentPtr->definition, out)) {return false;}

                this->idx -= difference;
                this->args.clear();
                return true;
             }

         }
     }

     if(!op->definition)
     {
         RunTime("Expected '->'").print(out);
         return false;
     }

     const FunctionT* funcName = dynamic_cast<const FunctionT*>(op->token);
     const FunctionT* funcDef = dynamic_cast<const FunctionT*>(op->definition->token);

     if(funcName && funcDef && funcName->name == funcDef->name)
     {
         RunTime("Function calls itself which would lead to stack overflow so it will not be created.").print(out);
         return false;
     }

     this->funcs.push_back(op);
     return true;
 }

 bool Interpreter::lookAt(const Element* syntaxTree, std::ostream& out)
 {
     const FactorElement* factorPtr = dynamic_cast<const FactorElement*>(syntaxTree);
     if(factorPtr)
     {
         return this->lookAtFactor(factorPtr,out);
     }

     const ArgElement* argPtr = dynamic_cast<const ArgElement*>(syntaxTree);
     if(argPtr)
     {
         return this->lookAt(argPtr,out);
     }

     const UnaryOperation* unary = dynamic_cast<const UnaryOperation*>(syntaxTree);
     if(unary)
     {
         if(!lookAt(unary->argument,out)) {return false;}
         return this->unary(unary, out);
     }

    const BinaryOperation* binary = dynamic_cast<const BinaryOperation*>(syntaxTree);
    if(binary)
    {
        if(CONCAT == dynamic_cast<const FunctionT*>(binary->token)->name)
        {
            const ListOperation* list1 = dynamic_cast<const ListOperation*>(binary->left);
			const ListOperation* list2 = dynamic_cast<const ListOperation*>(binary->right);

			std::vector<Element*> result = list1->list;

			for (auto it = list2->list.begin(); it!= list2->list.end(); ++it)
			{
				result.push_back((*it)->clone());
			}

            // try to create the final list
			ListOperation* newList = new ListOperation(new Token(list1->token->type), result);
			bool check = this->lookAt(newList, out);
			delete newList;
			for (Element* a : result)
			{
				a = nullptr;
			}
			return check;
        }

        if(!this->lookAt(binary->left,out) || !this->lookAt(binary->right,out) || !this->binary(binary, out)) { return false;}

        return true;
    }

    const IfOperation* ifPtr = dynamic_cast<const IfOperation*>(syntaxTree);
    if(ifPtr)
    {
        return this->lookAtIf(ifPtr, out);
    }

    const ListOperation* listPtr = dynamic_cast<const ListOperation*>(syntaxTree);
    if(listPtr)
    {
        return this->list(listPtr, out);
    }

    const MapOperation* mapPtr = dynamic_cast<const MapOperation*>(syntaxTree);
    if(mapPtr)
    {
        ListOperation* listN = new ListOperation(new Token(mapPtr->factor->token->type), this->map(mapPtr, out));
        bool check = this->lookAt(listN, out);
        delete listN;
        return check; 
    }

    const UserFunc* userFunc = dynamic_cast<const UserFunc*>(syntaxTree);
    if(userFunc)
    {
        if(!this->user(userFunc, out)) {return false;}
        return true;
    }

    RunTime("Could not find matching function definition").print(out);
    return false;
 }

void Interpreter::interpret(const Element* syntaxTree, std::ostream& out)
{
    if(!this->lookAt(syntaxTree,out))
    {
        this->idx = 0;
        this->args.clear();
        while(!this->results.empty())
        {
            this->results.pop();
        }
        return;
    }

    if(!this->results.empty())
    {
        out << this->results.top();
        this->results.pop();
    }

    if(!this->results.empty() || !this->args.empty() || this->idx != 0)
    {
        RunTime("Argument was unexpected.").print(out);
    }
}