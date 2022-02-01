#pragma once
#include "analyzer.h"
#include "predefinedFuncs.h"
#include "list.h"
#include <stack>

/// A class to interpret the functional language 
class Interpreter
{
    private:
    std::stack<double> results; // storing the results of predefined functions here
                                // using a stack because we will always be looking at the last value first

    std::vector<double> args; // storing the arguments of the user defined functions for easier work
                              // using a vector because of its constant access time at a given index
    int idx; // index to look at in the args vector for the arguments of the next user defined function call

    std::vector<const Element*> funcs; // storing pointers to the user defined functions here 
                                       // using a vector once again for the same reason as above

    std::stack<LinkedList> lists; // storing the lists here, will always return the last one 
    
    /// @brief Casting the element to every possible type so we figure out its type and
    /// calling the corresponding lookAt or funtion handling function
    /// @param syntaxTree - element to look at
    /// @param out - output and error stream
    bool lookAt(const Element* syntaxTree, std::ostream& out);

    /// @brief Put the value of the element in the stack of results. 
    /// Prints an error if the element is not a number
    /// @param element - element to look at
    /// @param out - output and error stream
    bool lookAtFactor(const FactorElement* element, std::ostream& out);

    /// @brief Gets the argument from the vector and puts it in the stack.
    /// @param element - argument to look at
    /// @param out - output and error stream
    bool lookAtArgument(const ArgElement* element, std::ostream& out);

    /// @param op - operation to look at
    /// @param out - output and error stream
    bool lookAtIf(const IfOperation* op, std::ostream& out);

    /// @brief Print the contents of a list
    /// @param list - list to look at
    /// @param out - output and error stream
    bool list(const ListOperation*& list, std::ostream& out);

    // -------- Handling functions --------

    ///@brief Look at an unary function. If it is not used defined - pushes the result
    /// to the stack, else - looks for the definition of the function and applies it
    /// @param op - function to look at
    /// @param out - output and error stream
    bool unary(const UnaryOperation* op, std::ostream& out);

    /// @brief Look at a binary function. Same thing as for an unary
    bool binary(const BinaryOperation* op, std::ostream& out);

    /// @brief Find a user defined function, transfer the corresponding arguments from the stack
    /// to the vector and apply it 
    bool user(const UserFunc* op, std::ostream& out);

    ///@brief Helper function for the destructor
    void deallocate();

    public:
    Interpreter();
    Interpreter(const Interpreter& other) = delete;
    Interpreter& operator =(const Interpreter& other) = delete;
    ~Interpreter();

    /// @brief Calls the lookAt method and from then outputs a result, an error or nothing if user defined function
    /// @param syntaxTree - element to look at
    /// @param out - output and error stream
    void interpret(const Element* syntaxTree, std::ostream& out);
};