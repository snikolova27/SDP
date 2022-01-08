#include "commands.h"
#include "commandTokenizer.h"
#include "interface.h"
#include <cmath>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <string>

void CommandProcessor::process_save_command(const SaveCommand& cmd)
{
    const std::string name = cmd.object_name;
    if(!this->is_in(name))
    {
        throw std::logic_error("Hierarchy does not exist");
    }

    const auto it = std::find(this->names.begin(), this->names.end(), name);
    const int idx = it - this->names.begin();

    if(cmd.file_name == "") //print on console
    {
       std::cout << this->h[idx].print() << std::endl;
    }
    else 
    {
        const std::string file_name = cmd.file_name;
        std::ofstream write;
        write.open(file_name);
        write << this->h[idx].print();
        write.close();
    }

}
void CommandProcessor::process_load_command(const LoadCommand& cmd)
{
    const std::string name = cmd.object_name;
    if( this->is_in(name) && !this->names.empty())
    {
        throw std::logic_error("Hierarchy is already loaded");
    }

    std::string src;
    if( cmd.file_name == "") //read from console
    {
        std::cout << "Write the hierarchy" << std::endl;
        src = get_str_from_stream(std::cin,true);
    }
    else 
    {
       std::ifstream file(cmd.file_name);
       src = get_str_from_stream(file,false);
    }
    this->h.emplace_back(Hierarchy(src));
    this->names.emplace_back(name);   
}

void CommandProcessor::process_find_command(const FindCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_emp_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << std::boolalpha << this->h[idx].find(emp) << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_subordinates_command(const SubordinatesCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_emp_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << this->h[idx].num_subordinates(emp) << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_manager_command(const ManagerCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_emp_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << this->h[idx].manager(emp) << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_fire_command(const FireCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_emp_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << std::boolalpha << this->h[idx].fire(emp) << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_salary_command(const SalaryCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_emp_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << this->h[idx].getSalary(emp) << std::endl; 
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_employees_command(const NumEmployeesCommand& cmd)
{
    const string object = cmd.get_object_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << this->h[idx].num_employees() << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_overloaded_command(const OverloadedCommand& cmd)
{
    const string object = cmd.get_object_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        std::cout << this->h[idx].num_overloaded() << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_incorporate_command(const IncorporateCommand& cmd)
{
    const string object = cmd.get_object_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        this->h[idx].incorporate();
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_modernize_command(const ModernizeCommand& cmd)
{
    const string object = cmd.get_object_name();
    if( this->is_in(object))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), object );
        const int idx = it - this->names.begin();
        this->h[idx].modernize();
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
}
void CommandProcessor::process_join_command(const JoinCommand& cmd)
{
    const string object1 = cmd.get_object1_name();
    const string object2 = cmd.get_object2_name();
    const string result = cmd.get_result_name();
    if( this->is_in(object1) && this->is_in(object2))
    {
        const auto it1 = std::find(this->names.begin(), this->names.end(), object1 );
        const int idx1 = it1 - this->names.begin();
        const auto it2 = std::find(this->names.begin(), this->names.end(), object2);
        const int idx2 = it2 - this->names.begin();
        this->h[idx1].join(this->h[idx2]);
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }

}
void CommandProcessor::process_hire_command(const HireCommand& cmd)
{
    const string object = cmd.get_object_name();
    const string emp = cmd.get_to_hire_name();
    const string manager = cmd.get_manager_name();
    if( this->is_in(object))
    {
        const auto it1 = std::find(this->names.begin(), this->names.end(), object);
        const int idx1 = it1 - this->names.begin();
        std::cout << std::boolalpha << this->h[idx1].hire(emp, manager) << std::endl;
    }
    else 
    {
        throw std::logic_error("Hierarchy with given name does not exist");
    }
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
        ManagerCommand cmd(tokens.arguments);
        this->process_manager_command(cmd);
    }
    else if (tokens.command == "fire")
    {
        FireCommand cmd(tokens.arguments);
        this->process_fire_command(cmd);
    }
    else if (tokens.command == "hire")
    {
        HireCommand cmd(tokens.arguments);
        this->process_hire_command(cmd);
    }
    else if (tokens.command == "salary")
    {
        SalaryCommand cmd(tokens.arguments);
        this->process_salary_command(cmd);
    }
    else if (tokens.command == "num_employees")
    {
        NumEmployeesCommand cmd(tokens.arguments);
        this->process_employees_command(cmd);
    }
    else if (tokens.command == "overloaded")
    {
        OverloadedCommand cmd(tokens.arguments);
        this->process_overloaded_command(cmd);
    }
    else if (tokens.command == "join")
    {
        JoinCommand cmd(tokens.arguments);
        this->process_join_command(cmd);
    }
    else if (tokens.command == "incorporate")
    {
        IncorporateCommand cmd(tokens.arguments);
        this->process_incorporate_command(cmd);
    }
    else if (tokens.command == "modernize")
    {
        ModernizeCommand cmd(tokens.arguments);
        this->process_modernize_command(cmd);
    }
    else
    {
        throw std::invalid_argument("Unknown command: " + tokens.command);
    }
    
}
void CommandProcessor::help( std::ostream & stream)
{
    stream << " =========  List of commands  =========" << std::endl;
    stream << "> help - prints out the list of commands" << std::endl;
    stream << "> save - <object name> <file name> - saves the hierarchy with given name in the given file" << std::endl;
    stream << "> load - <object name> <file name>- loads the hierarchy with given name from the given file" << std::endl;
    stream << "> find - <object name> <employee name> - returns if the employee is in the object" << std::endl;
    stream << "> num_subordinates - <object name> <employee name> - returns the subordinates of this employee in the given object" << std::endl;
    stream << "> manager - <object name> <employee name> - returns the manager of this employee in the given object" << std::endl;
    stream << "> num_employees - <object name> - returns the number of all employees in the object" << std::endl;
    stream << "> overloaded - <object name> - return the number of managers who have more than 20 subordinates" << std::endl;
    stream << "> join - <first object name> <second object name> <result object name> - joins the two objects and returns the result" << std::endl;
    stream << "> fire - <object name> <name of employee> - removes the employee from the object" << std::endl;
    stream << "> hire - <object name> <employee name> <manager name> - add the employee as an subordinate of the manager in the given object" << std::endl;
    stream << "> salary - <object name> <employee name> - returns the salary of the employee in the given object" << std::endl;
    stream << "> incorporate - <object name> - incorporates the object" << std::endl;
    stream << "> modernize - <object name> - modernizes the object" << std::endl;
    stream << "> exit - exits the program" << std::endl;

}
int CommandProcessor::exit() const
{
    return 0;
}




bool CommandProcessor::is_in(const std::string& hierarchy) const 
{   
    const auto it = std::find(this->names.begin(), this->names.end(), hierarchy);
    return it != this->names.end();

}
void CommandProcessor::add_hierarchy(const std::string& name,  const Hierarchy& to_add)
{
    this->names.emplace_back(name);
    this->h.emplace_back(to_add);
}

LoadCommand::LoadCommand( const std::vector <std::string>& args) :FileCommands()
{
    if ( args.size() == 1)
    {
        this->object_name = args[0];

    }    if( args.size() > 2)
    {
        throw std::logic_error("Load command takes up to 2 arguments");
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
        this->object_name = args[0];
    }
    if( args.size() > 2)
    {
        throw std::logic_error("Load command takes up to 2 arguments");
    }
    if ( args.size() == 2)
    {
        this->object_name = args[0];
        this->file_name = args[1];
    }
}

EmployeeCommands::EmployeeCommands(const std::vector <std::string>& args)
{
    if( args.size() != 2 )
    {
        throw std::logic_error("Employee commands take exactly 2 arguments");
    }

    this->object_name = args[0];
    this->emp_name = args[1];   
}

 ObjectCommands::ObjectCommands(const std::vector <std::string>& args)
 {
     if( args.size() != 1)
     {
        throw std::logic_error("Object commands take exactly 1 argument");
     }
     this->object_name = args[0];
 }  

 JoinCommand::JoinCommand(const std::vector <std::string>& args)
 {
     if( args.size() != 3)
     {
        throw std::logic_error("Join command takes exactly 3 arguments");
     }
     this->object1 =  args[0];
     this->object2 = args[1];
     this->result = args[2];
 }
HireCommand::HireCommand(const std::vector <std::string>& args)
{
    if( args.size() != 3)
     {
        throw std::logic_error("Hire command takes exactly 3 arguments");
     }
    this->object =  args[0];
     this->to_hire= args[1];
     this->manager = args[2];   
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

std::string get_str_from_stream(std::istream& stream, const bool break_on_empty_line)
{
    std::stringstream ss;
    std::string line;
    while (std::getline(stream, line))
    {
        if (line == "")
        {
            if (break_on_empty_line) break;
            continue;
        }
        ss << line << "\n";
    }
    return ss.str();
}