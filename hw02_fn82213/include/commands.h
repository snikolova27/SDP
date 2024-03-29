#pragma once
#include "interface.h"
#include "commandTokenizer.h"
#include <iostream>
#include <string>
#include <vector>

class FileCommands
{
    public:
    std::string object_name;
    std::string file_name ="";
  //  FileCommands();
   // FileCommands( const std::vector <std::string>& args);
   // FileCommands(std::string& obj, std::string& file) : object_name(obj), file_name(file){};
   
};

class LoadCommand : public FileCommands
{
    public:
    LoadCommand (const std::vector <std::string>& args) ;
};

class SaveCommand : public FileCommands
{
    public:
    SaveCommand(const std::vector <std::string>& args);
};

class EmployeeCommands
{
    private:
    std::string object_name;
    std::string emp_name;
    public:
    EmployeeCommands(std::string& obj, std::string& name) : object_name(obj), emp_name(name) {};
    EmployeeCommands(const std::vector <std::string>& args);
    const std::string& get_object_name() const;
    const std::string& get_emp_name() const;
};

class FindCommand : public EmployeeCommands 
{
    public:
    FindCommand(const std::vector <std::string>& args) : EmployeeCommands(args){};
};

class SubordinatesCommand : public EmployeeCommands 
{
    public:
    SubordinatesCommand(const std::vector <std::string>& args) : EmployeeCommands(args){};
};

class ManagerCommand : public EmployeeCommands 
{
    public:
    ManagerCommand(const std::vector <std::string>& args) : EmployeeCommands(args){};
};

class FireCommand : public EmployeeCommands
{   
    public:
    FireCommand(const std::vector <std::string>& args) : EmployeeCommands(args){};
};

class SalaryCommand : public EmployeeCommands
{
    public:
    SalaryCommand(const std::vector <std::string>& args) : EmployeeCommands(args){};
};


class ObjectCommands
{
    private:
    std::string object_name;
    public:
    ObjectCommands( std::string& obj) : object_name(obj) {};
    ObjectCommands(const std::vector <std::string>& args);
    const std::string& get_object_name() const;
};

class NumEmployeesCommand : public ObjectCommands
{
    public:
    NumEmployeesCommand(const std::vector <std::string>& args) : ObjectCommands(args) {};
};

class OverloadedCommand : public ObjectCommands 
{
    public:
    OverloadedCommand(const std::vector <std::string>& args) : ObjectCommands(args) {};
};

class IncorporateCommand : public ObjectCommands
{
    public:
    IncorporateCommand(const std::vector <std::string>& args) : ObjectCommands(args) {};
};

class ModernizeCommand : public ObjectCommands 
{
    public:
    ModernizeCommand(const std::vector <std::string>& args) : ObjectCommands(args) {};
};

class JoinCommand
{
    private:
    std::string object1;
    std::string object2;
    std::string result;
    public:
    JoinCommand(std::string& obj1, std::string& obj2, std::string& res) : object1(obj1), object2(obj2),result(res) {}; 
    JoinCommand(const std::vector <std::string>& args);
    const std::string& get_object1_name() const;
    const std::string& get_object2_name() const;
    const std::string& get_result_name() const;

};

class HireCommand
{
    private:
    std::string object;
    std::string to_hire;
    std::string manager;
    public:
    HireCommand(std::string& obj, std::string& hire, std::string& manager) : object(obj), to_hire(hire), manager(manager) {};
    HireCommand(const std::vector <std::string>& args);
    const std::string& get_object_name() const;
    const std::string& get_to_hire_name() const;
    const std::string& get_manager_name() const;
};

class CommandProcessor
{
    private:
    std::vector<Hierarchy> h;
    std::vector<std::string> names;

    void process_save_command(const SaveCommand& cmd);
    void process_load_command(const LoadCommand& cmd);
    void process_find_command(const FindCommand& cmd);
    void process_subordinates_command(const SubordinatesCommand& cmd);
    void process_manager_command(const ManagerCommand& cmd);
    void process_fire_command(const FireCommand& cmd);
    void process_salary_command(const SalaryCommand& cmd);
    void process_employees_command(const NumEmployeesCommand& cmd);
    void process_overloaded_command(const OverloadedCommand& cmd);
    void process_incorporate_command(const IncorporateCommand& cmd);
    void process_modernize_command(const ModernizeCommand& cmd);
    void process_join_command(const JoinCommand& cmd);
    void process_hire_command(const HireCommand& cmd);

    bool is_in(const std::string& hierarchy) const;
    void add_hierarchy( const std::string& name, const Hierarchy& to_add);
    
    public:
    CommandProcessor() =default;
    CommandProcessor(std::vector<Hierarchy> h, std::vector<std::string> object_names) : h(h), names(object_names) {};
    void process_command(const std::string& cmd_str);
    void help( std::ostream & stream);
    int exit() const;
};

std::string get_str_from_stream(std::istream& stream, const bool break_on_empty_line);