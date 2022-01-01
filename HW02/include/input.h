#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using std::string;
using std::vector;

const string empty = "";
class Input
{
    private:
    //string input = "";
    vector<string> employees;
    vector<vector <int>> connections;
    string fileToReadFrom;

   

    /// TODO CONSOLE
    /// @brief Read input from the console 
    void readConsole(); 

    /// @brief Make the connection between a manager and a subordiante if possible
    /// @param parent - the manager
    /// @param child - the subordinate
    void addConnection(const string& parent, const string& child);

    /// @brief Get the id of an employee
    /// @param employee - employee to search for
    int getEmployeeId(const string& employee) const;

    /// @brief Checks if the employee already has a "manager"
    /// @param employee - name of employee
    bool hasBeenOnTheRigth(const string& employee) const;

    /// @brief Checks if given name is valid
    bool isNameValid(const string& name) const;

    /// @brief Extracts the one on the left from a given string
    string getManagerFrom(const string& str) const;

    /// @brief Extracts the one on the rigth from a given string
    string getSubordinateFrom(const string& str) const;

 
    public:
    //TODO: read input
    /// @brief Read input from file, if file is not valid  - read from the console
    void readInput(const string from);

    vector <string> getEmployees() const
    {
        return this->employees;
    }
    vector <vector <int>> getRelationships() const
    {
        return this->connections;
    }
      void printConnections();
};