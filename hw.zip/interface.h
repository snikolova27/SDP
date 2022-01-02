#pragma once
#include <string>
#include <vector>
#include "team.h"

using std::string;
const int SALARY_PER_DIRECT_SUB = 500;
const int SALARY_PER_NON_DIRECT_SUB = 50;
const string TheBoss = "Uspeshnia";

class Hierarchy
{
public:
    Hierarchy(Hierarchy&& r) noexcept = default;
    Hierarchy(const Hierarchy& r) = default;
    Hierarchy(const string& data);
    Hierarchy() {this->employees.emplace_back(TheBoss);}
    ~Hierarchy() noexcept = default;
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

 
    void incorporate();
    void modernize();

    //TODO
    Hierarchy join(const Hierarchy& right) const;

    //If you need it - add more public methods here
private:
    std::vector<std::string> employees;
    std::vector<std::vector <int> > subs;

    /// @brief  Finds id of employee by given name, returns -1 if not found, valid index otherwise
    int find_id(const std::string & name) const;

    /// @brief Finds id of manager of employee given by name, returns -1 if not found, valid index otherwise
    int find_manager(const std::string& name) const;

    /// @brief Returns the number of all subordinates (direct and non-direct) of an employee given by id
    int cnt_all_subs(const int id) const;

    /// @brief Returns the number of non-direct subordinates of an employee given by id
    int cnt_non_direct_subordinates(const int id) const;

    /// @brief DFS for length, starting point is the employee, given by id
    int dfsl(const int id) const;

    /// @brief Finds the employee with the highest salary out of a vector with employees - vector with their ids
    /// @return returns -1 if all employees have the same salary, index of employee otherwise
    int find_employee_with_highest_salary(const std::vector <int>& employees) const;

    /// @brief Return max salary of given employees
    unsigned long max_salary(const std::vector<int>& employees) const;

    /// @brief Return a vector with the indices of employees with highest salaries
    std::vector <int> get_vector_employees_id_highest_salary (const std::vector <int>& employees) const;

    /// @brief Return the index of the lexicographically smallest employee in given vector of employees from a given position
    /// @return -1 if not valid, otherwise valid index
    int smallest_employee_from(const int from, const std::vector <int>& employees) const;

    /// @brief Remove the employee from all their managers
    void remove_connection(const int id_to_remove);

    /// @brief Used to evade code repetition
    void incorporate_helper(const int index, const int size, const std::vector<int>& v);

    /// @brief Change the manager of an employee
    void reasign_manager(const int idx_emp, const int idx_new_manager);

  
    int smallest_employee(const std::vector <int>& employees) const;
    
    ///@brief Get the ids of managers who are on uneven levels
    std::vector<std::vector<Team>> uneven_levels_teams() const;

    /// @brief Finds the level of a given employee by their index
    int find_level_employee(const int idx) const;

public:

     ///@brief Return name of employee by given index, if index is not valid - throws an exception
    string get_name_by_idx(const int idx) const;

    /// @brief Returns the teams on a given level
    std::vector<Team> teams_at(const int& level) const;

    ///@brief Return a vector of teams on different levels
    std::vector<std::vector<Team>> get_teams() const;

    /// @brief Return the employees with given manager
    std::vector<int> get_emp_with_manager(const int& idx) const;

    /// @brief Returns if the employee is a manager
    const bool is_manager(const int who) const;

    /// @brief Returns if the given manager is an indirect manager to the given employee
   const bool is_indirect_manager(const int manager, const int to_whom) const;

    /// @brief Wrapper for is_indirect_manager that works with employee names
   const bool indirect_manager(const string& manager, const string& to_whom) const;

   const bool check_the_boss(const Hierarchy& h2) const;

   const std::vector<int> get_subs_of(const int id) const;

   void hire_from_2_hierarchies_one_manager(const string& subs_of, const Hierarchy& first, const Hierarchy& second, const string& manager);

};

