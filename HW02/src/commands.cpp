#include "commands.h"
#include "commandTokenizer.h"
#include <stdexcept>

void CommandProcessor::process_save_command(const SaveCommand& cmd)
{

}
void CommandProcessor::process_load_command(const LoadCommand& cmd)
{

}
void CommandProcessor::process_find_command(const FindCommand& cmd)
{

}
void CommandProcessor::process_suboridnates_command(const SubordinatesCommand& cmd)
{

}
void CommandProcessor::process_manager_command(const ManagerCommand& cmd)
{

}
void CommandProcessor::process_fire_command(const FireCommand& cmd)
{

}
void CommandProcessor::process_salary_command(const SalaryCommand& cmd)
{

}
void CommandProcessor::process_employees_command(const NumEmployeesCommand& cmd)
{

}
void CommandProcessor::process_overloaded_command(const OverloadedCommand& cmd)
{

}
void CommandProcessor::process_incorporate_command(const IncorporateCommand& cmd)
{

}
void CommandProcessor::process_modernize_command(const ModernizeCommand& cmd)
{

}
void CommandProcessor::process_join_command(const JoinCommand& cmd)
{

}
void CommandProcessor::process_hire_command(const HireCommand& cmd)
{

}

void CommandProcessor::process_command(const std::string& cmd_str)
{

    const CommandTokens tokens = tokenize(cmd_str);
    // Tokenizer always returns lower-case commands
    if(tokens.command == "help")
    {
        this->help(std::cout);
    }

    else if(tokens.command == "exit")
    {
        this->exit();
    }

    else if (tokens.command == "save")
    {
        SaveCommand cmd(tokens.arguments);
        this->process_save_command(cmd);
    }
    else if (tokens.command == "load")
    {
        LoadCommand cmd(tokens.arguments);
        this->process_load_command(cmd);
    }
    else if (tokens.command == "find")
    {
        FindCommand cmd(tokens.arguments);
        this->process_find_command(cmd);
    }
    else if (tokens.command == "num_subordinates")
    {
        SubordinatesCommand cmd(tokens.arguments);
        this->process_subordinates_command(cmd);
    }
    else if (tokens.command == "manager")
    {
        RemoveCommand cmd(tokens.arguments);
        this->processRemoveCmd(cmd);
    }
    else if (tokens.command == "fire")
    {
        SaveCommand cmd(tokens.arguments);
        this->processSaveCmd(cmd);
    }
    else if (tokens.command == "hire")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "salary")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "num_employees")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "overloaded")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "join")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "incorpotate")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else if (tokens.command == "modernize")
    {
        ShowCommand cmd(tokens.arguments);
        this->processShowCmd(cmd);
    }
    else
    {
        throw std::invalid_argument("Unknown command: " + tokens.command);
    }
    
}
void CommandProcessor::help( std::ostream & stream)
{
    stream << "List of commands: " << std::endl;
    stream << "help - prints out the list of commands" << std::endl;
    stream << "save - takes 2 arguments - object name and file name - saves the hierarchy with given name in the given file" << std::endl;
    stream << "load - ltakes 2 arguments - object name and file name- loads the hierarchy with given name from the given file" << std::endl;
    stream << "find - takes 2 arguments - object name and employee name - returns if the employee is in the object" << std::endl;
    stream << "num_subordinates - takes 2 arguments - object name and employee name - returns the subordinates of this employee in the given object" << std::endl;
    stream << "manager - takes 2 arguments -  object name and employee name - returns the manager of this employee in the given object" << std::endl;
    stream << "num_employees - takes 1 argument - object name - returns the number of all employees in the object" << std::endl;
    stream << "overloaded - takes 1 argument - object name - return the number of managers who have more than 20 subordinates" << std::endl;
    stream << "join - takes 3 arguments - first object name, second object name, result object name - joins the two objects and returns the result" << std::endl;
    stream << "fire - takes 2 arguments - object name and name of employee - removes the employee from the object" << std::endl;
    stream << "hire - takes 3 arguments - object name, employee name and manager name - add the employee as an subordinate of the manager in the given object" << std::endl;
    stream << "salary - takes 2 arguments - object name and employee name - returns the salary of the employee in the given object" << std::endl;
    stream << "incorporate - takes 1 argument - object name - incorporates the object" << std::endl;
    stream << "modernize - takes 1 argument - object name - modernizes the object" << std::endl;
    stream << "exit - exits the program" << std::endl;

}
void CommandProcessor::exit()
{
    
}

LoadCommand::LoadCommand( const std::vector <std::string>& args) :FileCommands()
{
    if ( args.size() == 1)
    {
        //трябва да се чете от стандартния вход
    }
    if( args.size() > 2)
    {
        throw std::invalid_argument("Load command takes 2 arguments");
    }
    if ( args.size() == 2)
    {
        this->object_name = args[0];
        this->file_name = args[1];
    }
}
SaveCommand::SaveCommand(const std::vector<std::string>& args) :FileCommands()
{
    if ( args.size() == 1)
    {
        //трябва да се печата на стандартния вход
    }
    if( args.size() > 2)
    {
        throw std::invalid_argument("Load command takes 2 arguments");
    }
    if ( args.size() == 2)
    {
        this->object_name = args[0];
        this->file_name = args[1];
    }
}


const std::string& FileCommands::get_file_name() const
{
    return this->file_name;
}

const std::string& FileCommands::get_object_name() const
{
    return this->object_name;
}
const std::string& EmployeeCommands::get_emp_name() const
{
    return this->emp_name;
}

const std::string& EmployeeCommands::get_object_name() const
{
    return this->object_name;
}

const std::string& ObjectCommands::get_object_name() const
{
    return this->object_name;
}

const std::string& JoinCommand::get_object1_name() const
{
    return this->object1;
}
const std::string& JoinCommand::get_object2_name() const
{
    return this->object2;
}
const std::string& JoinCommand::get_result_name() const
{
    return this->result;
}

const std::string& HireCommand::get_object_name() const
{
    return this->object;
}
const std::string& HireCommand::get_to_hire_name() const
{
    return this->to_hire;
}
    
const std::string& HireCommand::get_manager_name() const
{
    return this->manager;
}
