#pragma once
#include "commandTokens.h"
#include <vector>

// Will be used to store the different element of an expression
class Element
{
    private:
    /// @brief Helper function for the copy constructor
    void copy(const Element& other);

    /// @brief Helper function for the destructor
    void deallocate();

    public:
    const Token* token;
    explicit Element(const Token* token); // explicit so that the class cannot be initialized 
                                          // using this constructor and a non-Token value
    Element(const Element& other);

    Element& operator= (const Element& other);

    virtual ~Element();

    /// @brief Prints the tokens in a given stream
    virtual void print(std::ostream& out) const;

    /// @brief Copies the value, not the pointer
    virtual Element* clone() const;
};


// Class to store an element that's an argument
class ArgElement : public Element   
{
    public:
    explicit ArgElement(const Token* token);  // same thing as base class
    ArgElement(const ArgElement& other);
    ArgElement& operator= (const ArgElement& other);

    /// @brief Copies the value, not the pointer
    ArgElement* clone() const override;    
};

// Class to store an element that's a factor
class FactorElement : public Element
{
    public:
    explicit FactorElement(const Token* token);  // same thing as base class
    FactorElement(const ArgElement& other);
    FactorElement& operator= (const ArgElement& other);

    /// @brief Copies the value, not the pointer
    FactorElement* clone() const override;    
};

// Class to store an unary operation and its argument
class UnaryOperation : public Element
{
    public:
    const Element* argument;

    UnaryOperation(const Token* token, const Element* argument);
    UnaryOperation(const UnaryOperation& other);
    ~UnaryOperation();
    UnaryOperation& operator=(const UnaryOperation& other);

    /// @brief Copies the value, not the pointer
    UnaryOperation* clone() const override;

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};

// Class to store a binary operation and its arguments
class BinaryOperation : public Element
{
    private:
    /// @brief Helper function for the destructor
    void deallocate();

    public:
    const Element* left;
    const Element* right;
    
    BinaryOperation(const Token* token, const Element* left, const Element* right);
    BinaryOperation(const BinaryOperation& other);
    ~BinaryOperation();
    BinaryOperation& operator=(const BinaryOperation& other);

   /// @brief Copies the value, not the pointer
    BinaryOperation* clone() const override;

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};

// Class to store an if expression and its arguments
class IfOperation : public Element
{
    private:
    /// @brief Helper function for the copy constructor
    void copy(const IfOperation& other);

    /// @brief Helper function for the destructor
    void deallocate();
    
    public:
    const Element* left;
    const Element* right;
    const Element* condition;

    IfOperation(const Token* token, const Element* left, const Element* right, const Element* condition);
    IfOperation(const IfOperation& other);
    ~IfOperation();
    IfOperation& operator=(const IfOperation& other);

    /// @brief Copies the value, not the pointer
    IfOperation* clone() const override;

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};

// Class that stores a list
class ListOperation : public Element
{
    private:
    /// @brief Helper function for the destructor
    void deallocate();

    public:
    std::vector<Element*> list; 

    ListOperation(const Token* token, const std::vector<Element*>& elements);
    ~ListOperation();

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};

class MapOperation : public Element
{
    public:
    const Element* factor;
    const Element* list;

    MapOperation(const Token* token, const Element* factor, const Element* list);
    ~MapOperation();

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};

// Class to store user defined functions and their arguments
class UserFunc : public Element
{
    private:
    /// @brief Helper function for the copy constructor
    void copy(const UserFunc& other);

    /// @brief Helper function for the destructor
    void deallocate();

    public:
    const Element* definition;
    std::vector<const Element*> args;  // using a vector because of its constant access time
    
    UserFunc(const Token* token, const Element* def, const std::vector<const Element*>& args);
    UserFunc(const UserFunc& other);
    UserFunc& operator=(const UserFunc& other);
    ~UserFunc();

    /// @brief Copies the value, not the pointer
    UserFunc* clone() const override;

    /// @brief Prints the tokens in a stream
    void print(std::ostream& out) const override;
};