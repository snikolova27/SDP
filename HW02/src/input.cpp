#include "../include/input.h"
#include <cstddef>
#include <fstream>
#include <regex>
#include <stdexcept>

int Input::getEmployeeId(const string& employee) const
{
   std::size_t size = this->employees.size();
   for (std::size_t i = 0; i < size; i++)
   {
       if ( employee == this->employees[i])
       {
           return i;
       }
   }
   return -1; // would mean employee was not found
}

bool Input::hasBeenOnTheRigth(const string& employee) const
{
    int id = getEmployeeId(employee);
    if (id == -1) //would mean employee is not yet in the list
    {
        return false;
    }

    std::size_t size = this->employees.size();
    for(std::size_t i = 0 ; i < id; i++)
    {
        std::size_t innerSize = this->connections[i].size();
        for(std::size_t j = 0; j < innerSize; j++)
        {
            if ( this->connections[i][j] == id)  //means that employee already has a manager
            {
                return true;
            }
        }
    }

    for( std::size_t k = id + 1; k < size; k++)
    {
        std::size_t innerSize = this->connections[k].size();
        for(std::size_t j = 0; j < innerSize; j++)
        {
            if ( this->connections[k][j] == id)
            {
                return true;
            }
        }
    }

    return false;
}

void Input::addConnection(const string& parent, const string& child)
{
    int idParent = getEmployeeId(parent);
    int idChild = getEmployeeId(child);

    if(! (idParent >= 0 || idChild >= 0 || idParent != idChild))
    {
        throw std::invalid_argument ("Bad indices!");
    }
    if(!hasBeenOnTheRigth(parent) && parent != TheBoss)
    {
        throw std::invalid_argument("Cannot add a subordinate to a person who has no manager - it's not in the hierarchy yet");
    }
    this->connections[idParent].push_back(idChild);
}


bool Input::isNameValid(const string& name) const
{
    std::size_t len = name.size();
    for( std::size_t i = 0; i < len; i++)
    {
        if( name [i] == ' ' || name [i] == '-')
        {
            return false;
        }
    }
    return true;
}

string Input::getManagerFrom(const string& str) const
{
    std::size_t len = str.size();
    string manager = "";
    for ( std::size_t i = 0; i < len; i++)
    {
        if( str [i] == '-')
        {
            return manager;
        }
        if( str[i] != ' ' && str[i] != '-')
        {
            manager.push_back(str[i]);
        }
    }
    return manager;
}
   
string Input::getSubordinateFrom(const string& str) const
{
    std::size_t len = str.size();
    string subordinate = "";
    std::size_t startId = 0;
    for ( std::size_t i = 0; i < len; i++)
    {
        if( str [i] == '-')
        {
            startId = i;
        }
    }
    for ( std::size_t k = startId; k < len ; k++)
    {
        if(str[k] == '\n')
        {
            return subordinate;
        }
        if( str[k] != ' ' && str[k] != '-')
        {
            subordinate.push_back(str [k]);
        }
    }
    return subordinate;
}

void Input::readInput(const string from)
{
    this->employees.push_back("Uspeshnia");
    if (from == empty) //if no file name has been given
    {
        this->readConsole(); 
    }
    else 
    {
        std::ifstream file (from);
        if(file.is_open())
        {
            string line;
            while(std::getline (file, line))
            {
                string manager = getManagerFrom(line);
                string subordinate = getSubordinateFrom(line);
                if(!isNameValid(manager) || !isNameValid(subordinate))
                {
                    file.close();
                    throw std::invalid_argument ("Input is not correct");
                }
                int idM = getEmployeeId(manager);
                int idS = getEmployeeId(subordinate);

                if(idM == -1 && manager != TheBoss)
                {
                    file.close();
                    throw std::invalid_argument("Cannot add a manager if they don't exist as an employee yet");
                }
                if( idS == -1 && !hasBeenOnTheRigth(subordinate))
                {
                    this->employees.push_back(subordinate);
                    this->addConnection(manager, subordinate);
                }
                else if (idS != -1 && hasBeenOnTheRigth(subordinate))
                {
                    file.close();
                    throw std::invalid_argument("Subordinate already has a manager");
                }
            }
            file.close();
        }
    }
}

void Input::readConsole()
{

}
void Input::printConnections()
{
    int size = this->connections.size();
    for (int i=0; i< size;i++)
    {
        int inner = this->connections[i].size();
        for( int k = 0; k< inner;k++)
        {
            std::cout << i << " - " << this->connections[i][k] << std::endl;
        }
    }
}