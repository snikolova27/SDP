#pragma once
#include "commandTokens.h"
#include <vector>

// Will be used to store the different element of an expression
class Element
{
    private:
    void copy(const Element& other);
    void deallocate();

    public:
    const Token* token;
    explicit Element(const Token* token); // explicit so that the class cannot be initialized 
                                          // using this constructor and a non-Token value
    Element(const Element& other);

    Element& operator= (const Element& other);

    virtual ~Element();

    virtual void print(std::ostream& out) const;

    virtual Element* clone() const; // Copying the value, not the pointer

};


// Class to store an element that's an argument
class ArgElement : public Element   
{
    public:
    explicit ArgElement(const Token* token);  // same thing as base class
    ArgElement(const ArgElement& other);
    ArgElement& operator= (const ArgElement& other);

    ArgElement* clone() const override;    
};

// Class to store an element that's a factor
class FactorElement : public Element
{
    public:
    explicit FactorElement(const Token* token);  // same thing as base class
    FactorElement(const ArgElement& other);
    FactorElement& operator= (const ArgElement& other);

    FactorElement* clone() const override;    
};

// Class to store an unary operation and its argument
class UnaryOperation : public Element
{
    private:
    const Element* argument;

    public:
    UnaryOperation(const Token* token, const Element* argument);
    UnaryOperation(const UnaryOperation& other);
    ~UnaryOperation();
    UnaryOperation& operator=(const UnaryOperation& other);

    UnaryOperation* clone() const override;

    void print(std::ostream& out) const override;
};

// Class to store a binary operation and its arguments
class BinaryOperation : public Element
{
    private:
    const Element* left;
    const Element* right;
    void freePointers();

    public:
    BinaryOperation(const Token* token, const Element* left, const Element* right);
    BinaryOperation(const BinaryOperation& other);
    ~BinaryOperation();
    BinaryOperation& operator=(const BinaryOperation& other);

    BinaryOperation* clone() const override;

    void print(std::ostream& out) const override;
};

// Class to store an if expression and its arguments
class IfOperation : public Element
{
    private:
    const Element* left;
    const Element* right;
    const Element* condition;

    void copy(const IfOperation& other);
    void deallocate();
    
    public:
    IfOperation(const Token* token, const Element* left, const Element* right, const Element* condition);
    IfOperation(const IfOperation& other);
    ~IfOperation();
    IfOperation& operator=(const IfOperation& other);

    IfOperation* clone() const override;

    void print(std::ostream& out) const override;
};

// Class that stores a list
class ListOperation : public Element
{
    private:
    std::vector<Element*> list; 

    void deallocate();

    public:
    ListOperation(const Token* token, const std::vector<Element*>& elements);
    ~ListOperation();

    void print(std::ostream& out) const override;
};

class MapOperation : public Element
{
    private:
    const Element* factor;
    const Element* list;

    public:
    MapOperation(const Token* token, const Element* factor, const Element* list);
    ~MapOperation();

    void print(std::ostream& out) const override;
};

// Class to store user defined functions and their arguments
class UserFunc : public Element
{
    private:
    const Element* definition;
    std::vector<const Element*> args;

    void copy(const UserFunc& other);
    void deallocate();

    public:
    UserFunc(const Token* token, const Element* def, const std::vector<const Element*>& args);
    UserFunc(const UserFunc& other);
    UserFunc& operator=(const UserFunc& other);
    ~UserFunc();

    UserFunc* clone() const override;

    void print(std::ostream& out) const override;
};