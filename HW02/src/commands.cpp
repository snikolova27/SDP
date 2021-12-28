#include "commands.h"

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
