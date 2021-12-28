#pragma once
#include <string>
#include <vector>

class FileCommands
{
    private:
    std::string object_name;
    std::string file_name;
    public:
    FileCommands(std::string& obj, std::string& file) : object_name(obj), file_name(file){};
    const std::string& get_object_name () const;
    const std::string& get_file_name() const;
};

class LoadCommand : public FileCommands {};

class SaveCommand : public FileCommands {};

class EmployeeCommands
{
    private:
    std::string object_name;
    std::string emp_name;
    public:
    EmployeeCommands(std::string& obj, std::string& name) : object_name(obj), emp_name(name) {};
    const std::string& get_object_name() const;
    const std::string& get_emp_name() const;
};

class FindCommand : public EmployeeCommands {};

class SubordinatesCommand : public EmployeeCommands {};

class ManagerCommand : public EmployeeCommands {};

class FireCommand : public EmployeeCommands {};

class SalaryCommand : public EmployeeCommands {};


class ObjectCommands
{
    private:
    std::string object_name;
    public:
    ObjectCommands( std::string& obj) : object_name(obj) {};
    const std::string& get_object_name() const;
};

class NumEmployeesCommand : public ObjectCommands {};

class OverloadedCommand : public ObjectCommands {};

class IncorporateCommand : public ObjectCommands {};

class ModernizeCommand : public ObjectCommands {};

class JoinCommand
{
    private:
    std::string object1;
    std::string object2;
    std::string result;
    public:
    JoinCommand(std::string& obj1, std::string& obj2, std::string& res) : object1(obj1), object2(obj2),result(res) {}; 
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
    const std::string& get_object_name() const;
    const std::string& get_to_hire_name() const;
    const std::string& get_manager_name() const;
};

