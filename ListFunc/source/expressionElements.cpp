#include "expressionElements.h"
#include "commandTokens.h"
#include <cstddef>
#include <ostream>

// -------- Base element --------

Element::Element(const Token* token) : token(nullptr)
{   
    // try to cast it to every token type so we figure out what it is

    const ArgumentT* argPtr = dynamic_cast<const ArgumentT*>(token);
    if(argPtr)
    {
        this->token = new ArgumentT(argPtr->value);
        return;
    }

    const FunctionT* funcPtr = dynamic_cast<const FunctionT*>(token);
    if(funcPtr)
    {
        this->token = new FunctionT(funcPtr->name);
        return;
    }

    const NumberT* numPtr = dynamic_cast<const NumberT*>(token);
    if(numPtr)
    {
        this->token = new NumberT(numPtr->value);
        return;
    }

    if(token)
    {
        this->token = new Token(token->type);
    }
}

void Element::deallocate()
{
    delete this->token;
    this->token = nullptr;
}

void Element::copy(const Element& other)
{
    // try to cast it to every token type so we figure out what it is - same thing as Element(const Token* token)

    const ArgumentT* argPtr = dynamic_cast<const ArgumentT*>(other.token);
    if(argPtr)
    {
        this->token = new ArgumentT(argPtr->value);
        return;
    }

    const FunctionT* funcPtr = dynamic_cast<const FunctionT*>(other.token);
    if(funcPtr)
    {
        this->token = new FunctionT(funcPtr->name);
        return;
    }

    const NumberT* numPtr = dynamic_cast<const NumberT*>(other.token);
    if(numPtr)
    {
        this->token = new NumberT(numPtr->value);
        return;
    }

    this->token = other.token ? new Token(other.token->type) : nullptr; // it is expected that we are never 
                                                                        // in the nullptr case
}

Element::Element(const Element& other)
{
    this->copy(other);
}

Element& Element::operator=(const Element& other)
{
    if(this != &other)
    {
        this->deallocate();
        this->copy(other);
    }
    return *this;
}

Element ::~Element()
{
    this->deallocate();
}

void Element::print(std::ostream& out) const
{
    this->token->print(out);
}

Element* Element::clone() const
{
    return new Element(*this);
}


//-------- Argument element --------
ArgElement::ArgElement(const Token* token) : Element(token){};
ArgElement::ArgElement(const ArgElement& other) : Element(other){}
ArgElement& ArgElement::operator= (const ArgElement& other) 
{
    Element::operator=(other);
    return *this;
}
ArgElement* ArgElement::clone() const 
{
    return new ArgElement(*this);
}   


// -------- Factor element --------
FactorElement::FactorElement(const Token* token) : Element(token){};
FactorElement::FactorElement(const ArgElement& other) : Element(other){}
FactorElement& FactorElement::operator= (const ArgElement& other) 
{
    Element::operator=(other);
    return *this;
}
FactorElement* FactorElement::clone() const 
{
    return new FactorElement(*this);
} 


// -------- Unary operation --------
UnaryOperation::UnaryOperation(const Token* token, const Element* argument) : Element(token), argument(argument) {}
UnaryOperation::UnaryOperation(const UnaryOperation& other) : Element(other.token)
{
    this->argument = other.argument;
}
UnaryOperation::~UnaryOperation()
{
   delete this->argument;
   this->argument = nullptr;
}
UnaryOperation& UnaryOperation::operator=(const UnaryOperation& other)
{
    if(this != &other)
    {
        delete this->argument;
        this->argument = nullptr;

        Element::operator=(other);
        this->argument = other.argument->clone();
    }
    return *this;
}
UnaryOperation* UnaryOperation::clone() const
{
    return new UnaryOperation(*this);
}

void UnaryOperation::print(std::ostream& out) const
{
    out << "(";
    Element::print(out);
    out << " " << this->argument << ")";
}


// -------- Binary operation --------
 BinaryOperation::BinaryOperation(const Token* token, const Element* left, const Element* right) : Element(token),
                                                                                         left(left), right(right) {};
 BinaryOperation::BinaryOperation(const BinaryOperation& other) : Element(other.token)
 {
     this->left = other.left;
     this->right = other.right;
 }
 BinaryOperation::~BinaryOperation()
 {
    this->deallocate();
 }
BinaryOperation& BinaryOperation:: operator=(const BinaryOperation& other)
{
    if( this != &other)
    {
      this->deallocate();
      Element::operator=(other);
      this->left = other.left->clone();
      this->right = other.left->clone();
    }
    return *this;
}
BinaryOperation* BinaryOperation::clone() const
{
    return new BinaryOperation(*this);
}

void BinaryOperation::print(std::ostream& out) const
{
    out << "(";
    Element::print(out);
    out << " ";
    this->left->print(out);
    out << " ";
    this->right->print(out);
    out << ")";
}

void BinaryOperation::deallocate()
{
    delete this->left;
    this->left = nullptr;

    delete this->right;
    this->right = nullptr;
}

// --------If statement --------
IfOperation::IfOperation(const Token* token, const Element* left, const Element* right, const Element* condition) : 
                        Element(token), left(left), right(right), condition(condition) {}
IfOperation::IfOperation(const IfOperation& other) : Element(other.token)
{
    this->copy(other);
}
IfOperation::~IfOperation()
{
    this->deallocate();
}
void IfOperation::copy(const IfOperation& other)
{
    this->left = other.left->clone();
    this->right = other.right->clone();
    this->condition = other.condition->clone();
}
void IfOperation::deallocate()
{
    delete this->left;
    this->left = nullptr;

    delete this->right;
    this->right = nullptr;

    delete this->condition;
    this->condition = nullptr;
}
IfOperation& IfOperation :: operator=(const IfOperation& other)
{
    if(this != &other)
    {
        this->deallocate();
        Element::operator=(other);
        this->copy(other);
    }
    return *this;
}
IfOperation* IfOperation::clone() const 
{
    return new IfOperation(*this);
}
void IfOperation::print(std::ostream& out) const
{
    out <<"( ";
    Element::print(out);
    this->left->print(out);
    out << " ";
    this->condition->print(out);
    out << " ";
    this->right->print(out);
    out << " )";
}

// --------List operation --------
ListOperation::ListOperation(const Token* token, const std::vector <Element*>& elements) : Element(token), list(elements) {}
ListOperation::~ListOperation()
{
    this->deallocate();
}
void ListOperation::print(std::ostream& out) const
{
    out << "(";
    Element::print(out);
    out << " ";
    const int size = this->list.size();
    for(int i = 0; i < size;i++)
    {
        out << this->list[i] << " ";
    }
    out << ")";

}

void ListOperation::deallocate()
{
    const int size = this->list.size();
    for(int i = 0; i < size; i++)
    {   
        delete this->list[i];
        this->list[i] = nullptr;
    }
}

// --------User defined functions --------
UserFunc::UserFunc(const Token* token, const Element* def, const std::vector<const Element*>& args) :
                                                Element(token), definition(def), args(args) {}

UserFunc::UserFunc(const UserFunc& other) : Element(other.token)
{
    this->copy(other);
}
UserFunc& UserFunc::operator=(const UserFunc& other)
{
    if( this != &other)
    {
        this->deallocate();
        Element::operator=(other);
        this->copy(other);
    }
    return *this;
}
UserFunc::~UserFunc()
{
    this->deallocate();
}

void UserFunc::copy(const UserFunc& other)
{
   this->definition = other.definition->clone();
   const int size = other.args.size();
   for(int i = 0; i < size;i++)
   {
       this->args.push_back(other.args[i]->clone());
   }
}

void UserFunc::deallocate()
{
    const int size = this->args.size();
    for(int i = 0; i < size; i++)
    {
        delete this->args[i];
        this->args[i]=nullptr;
    }
}

void UserFunc::print(std::ostream& out) const
{
    out << "(";
    Element::print(out);
    out << ' ';
    if(this->definition) this->definition->print(out);
    else {out << "()";}

    const int size = this->args.size();
    if(size)
    {
        out << " ";
        for( int i = 0; i < size; i++)
        {
            this->args[i]->print(out);
            out << " ";
        }
    }
    out << ")";
}

UserFunc* UserFunc::clone() const
{
    return new UserFunc(*this);
}