#include "interface.h"
//#include "../include/interface.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>


bool Hierarchy:: operator == (const Hierarchy& other) const
{
  return this->employees == other.employees && this->subs == other.subs;
}

Hierarchy::Hierarchy(const string& data)
{
  std::istringstream ss(data);
  std::string line;
  while (std::getline(ss, line))
  {
    const auto dashIdx = line.find('-');
    if (dashIdx == line.npos) 
    {
      throw std::invalid_argument("No dash present");
    }
    line[dashIdx] = ' ';

    std::istringstream ss2(line);
    std::string man, sub;
    if (!(ss2 >> man >> sub))
    {
      // stuff
      continue;
    }

    if (this->employees.empty())
    {
      this->employees.emplace_back(man);
      this->subs.emplace_back();
    }
    if(!hire(sub,man))
    {
      throw std::invalid_argument("Boss hasn't been hired");
    }   
  }
}

string Hierarchy::get_name_by_idx(const int idx) const
{
  if ( idx >= this->employees.size() || idx < 0)
  {
    throw std::invalid_argument("Invalid index");
  }
  return this->employees[idx];
}

string Hierarchy::print() const 
{
    const int size = this->employees.size();
    if(size <= 0)
    {
      return "";
    }

    const std::vector<string> & names = this->employees;

    std::stringstream ss;

    struct Line
    {
      int man, sub;
    };

    std::queue<Line> printQ;

    auto push_subs = [&](const int id)
    {
      auto subs = this->subs[id];
      std::sort(subs.begin(), subs.end(), [&](int a, int b) { return names[a] < names[b]; });
      for (int sid : subs) printQ.emplace<Line>({id, sid});
    };

    push_subs(0);
    while (!printQ.empty())
    {
      const auto curr = printQ.front();
      printQ.pop();
      ss << names[curr.man] << "-" << names[curr.sub] << "\n";
      push_subs(curr.sub);
    }

    return ss.str();    
  }

int Hierarchy::num_employees() const 
{ 
  return this->employees.size(); 
}

bool Hierarchy::find(const string &name) const 
{ 
  return find_id(name) >= 0;
}

int Hierarchy::find_id(const std::string &name) const
{
  const auto it = std::find(this->employees.begin(), this->employees.end(), name);
  if (it == this->employees.end())
  {
    return -1;
  } 
  return it - this->employees.begin();
}

bool Hierarchy::hire(const string &who, const string &boss) 
{
  const int boss_id = find_id(boss);
  const int orig_emp_id = find_id(who);
  if (boss_id < 0)  // invalid boss name
  {
    return false;
  } 
  if (orig_emp_id != -1)  //would mean that the person is already hired
  {
    this->reasign_manager(orig_emp_id, boss_id);
    return true;
  } 

  const int emp_id = this->employees.size();    //new employee id
  this->employees.emplace_back(who);
  this->subs.emplace_back();    // create an empty vector for the subordinates of the new employee
  this->subs[boss_id].emplace_back(emp_id);
  return true;
}

bool Hierarchy::fire(const string& who)
{
  const int id = find_id(who);
  if (id <= 0)
  {
    return false;
  } 

  const int manager_id = find_manager(who);
  if(manager_id < 0)
  {
     return false;
  }

  const int cnt_subs_emp = this->subs[id].size(); 
  for( int i = 0; i < cnt_subs_emp; i++)
  {
    int to_insert = this->subs[id][i];
    this->subs[manager_id].emplace_back(to_insert);
  }

  this->subs.erase(this->subs.begin() +id);

  for (auto & s : this->subs)
  {
    int dst = 0;
    int src = 0;
    for (; src < s.size(); src++)
    {
      const int curr = s[src];
      if (curr == id) continue;
      s[dst++] = curr > id ? curr - 1 : curr;
    }
    if (dst < src) s.erase(s.end() - 1);
  }
  this->employees.erase(this->employees.begin() + id);
  
  return true;
}


int Hierarchy ::find_manager(const std::string& name) const
{
  const int emp_id = find_id(name);
  if (emp_id < 0)
  {
    return -1; // invalid employee id
  }
 
  if(emp_id == 0)
  {
    return 0; // the Boss does not have a manager
  }

  const int size = this->subs.size();
  for (int i = 0; i < size; i++)
  {
    const auto & s = this->subs[i];
    const auto it = std::find(s.begin(), s.end(), emp_id);
    if (it != s.end()) 
    {
      return i;
    }
  }
  return -1; // Nobody is the boss of him
}

 string Hierarchy::manager(const string& name) const
 {
   const int manager_id = find_manager(name);
   const int emp_id = find_id(name);
   if (manager_id < 0)
   {
     throw std::invalid_argument("No manager");
   }
   if ( manager_id == 0 && emp_id != 0)
   {
     return TheBoss;
   }
   if ( emp_id == 0)
   {
     return "";
   }
   return this->employees[manager_id];
 }

int Hierarchy::num_subordinates(const string& name) const
{
  const int emp_id = find_id(name);
  if(emp_id < 0) //invalid employee
  {
    return -1;
  }
  return this->subs[emp_id].size();
}

int Hierarchy::cnt_all_subs(const int id) const
{
  const auto & s = this->subs[id];
  int sum = s.size();
  for (int sub : s) 
  {
    sum += cnt_all_subs(sub);
  }
  return sum;
}

int Hierarchy::cnt_non_direct_subordinates(const int id) const
{
  return cnt_all_subs(id) - this->subs[id].size();
}

unsigned long Hierarchy::getSalary(const string& who) const
{
   const int emp_id = find_id(who);
   if( emp_id < 0)
   {
     //cast -1 to unsinged long
     return (unsigned long) -1;
    // throw std::invalid_argument("Invalid employee");
   }
  return SALARY_PER_DIRECT_SUB * num_subordinates(who) + SALARY_PER_NON_DIRECT_SUB * cnt_non_direct_subordinates(emp_id);
}

int Hierarchy::num_overloaded(int level) const
{
  int cnt = 0;
  const int size = this->subs.size();
  for( int i = 0; i < size; i++)
  {
     if (cnt_all_subs(i) > level)
    {
      cnt++;
    }
  }
  return cnt;
}

int Hierarchy::dfsl(const int id) const
{
    int m = 0;
    for (int sub : this->subs[id])
    {
      m = std::max(m, dfsl(sub));
    } 
    return m + 1;
 }

int Hierarchy::longest_chain() const
 {
   if (employees.empty()) return 0;
    return dfsl(0);
 }

unsigned long Hierarchy::max_salary(const std::vector<int>& employees) const
{
  unsigned long max = 0;
  unsigned long current_max = 0;
  const int size = employees. size();
  for(int i = 0; i < size; i++)
  {
    current_max = this->getSalary(this->employees[employees[i]]);
    if (current_max  > max  && current_max != (unsigned long) -1)
    {
      max = current_max;
    }
  }
  return max;
}

int Hierarchy::find_employee_with_highest_salary(const std::vector <int>& employees) const
{
  unsigned long max = max_salary(employees);
  unsigned long current_max = 0;
  int max_id = -1;
  int cnt_with_max_salary = 0;

  const int size = employees. size();
  for(int i = 0; i < size; i++)
  {
    current_max = this->getSalary(this->employees [employees[i]]);
    if (current_max == max && current_max != (unsigned long) -1)
    {
      cnt_with_max_salary++;
      max_id = employees[i];
    }
  }
  if (cnt_with_max_salary > 1)  //more than one person with the highest salary
  {
    return -1;
  }
  return max_id;
}

std::vector <int> Hierarchy::get_vector_employees_id_highest_salary (const std::vector <int>& employees) const
{
    unsigned long current = 0;
    const unsigned long max= max_salary(employees);
    const int size = employees.size();
    std::vector <int> res;

    for (int i = 0; i < size; i++)
    {
      current = this->getSalary(this->employees [employees[i]]);
      if ( current == max && current != (unsigned long) -1 )
      {
        res.emplace_back(employees[i]); //put the index of the employee in the result vector
      }
    }

    return res;
}

int Hierarchy::smallest_employee(const std::vector <int>& employees) const
{
  const int size = employees.size();
  if ( size == 0)
  {
    return -1; //empty vector
  }

  string current_smallest = this->employees[employees[0]];
  int smallest_id = 0;
  for (int i = 1; i < size ; i++)
  {
    if (this->employees[employees[i]] < current_smallest)
    {
      current_smallest = this->employees[employees[i]];
      smallest_id = employees[i];
    }
  }
  return smallest_id;
}

int Hierarchy::smallest_employee_from(const int from, const std::vector <int>& employees) const
{
  const int size = employees.size();
  if ( size == 0)
  {
    return -1; //empty vector
  }

  string current_smallest = this->employees[employees[from]];
  int smallest_id = employees[from];
  for (int i = from + 1; i < size ; i++)
  {
    if (this->employees[employees[i]] < current_smallest)
    {
      current_smallest = this->employees[employees[i]];
      smallest_id = employees[i];
    }
  }
  return smallest_id;
}

void Hierarchy:: remove_connection(const int id_to_remove)
{
  const int size = this->subs.size();
  int index = -1;
  for(int i = 0; i < size; i++)
  { 
   const int inner_size = this->subs[i].size();
   for ( int j = 0; j< inner_size; j++)
   {
     if(id_to_remove == this->subs[i][j])
     {
       index = j;
     }
   }
   if( index != -1)
   {
      this->subs[i].erase(this->subs[i].begin() + index); 
      index = -1;
   }
  }
}

void Hierarchy::incorporate_helper(const int index, const int size, const std::vector<int>& v)
{
    for (int j = 0; j < index;j++)
          {
            remove_connection(v[j]);  //remove all manager connections of the other employees in this subs row
            this->subs[index].push_back(v[j]); //make the smallest_emp_id the manager of all other employees in this subs row
          }
          for( int j = index + 1; j < size; j++)
          {
            remove_connection(v[j]);
            this->subs[index].push_back(v[j]);
          }
}

void Hierarchy::reasign_manager(const int idx_emp, const int idx_new_manager)
{
  const int current_manager = this->find_manager(this->get_name_by_idx(idx_emp)); //get the current manager
  if( current_manager == -1 )
  {
    throw std::invalid_argument ("There is not a valid current manager");
  }

  // find position in the vector of employees of the current manager
  int emp_at_idx = -1;
  for(int i = 0; i < this->subs[current_manager].size(); i++)
  {
  if( this->subs[current_manager][i] == idx_emp)
    {
      emp_at_idx = i;
      break;
    }
  }
  if ( emp_at_idx == -1)
  { 
    throw std::invalid_argument("Employee not found");
  }
 // remove employee from current manager
  this->subs[current_manager].erase(this->subs[current_manager].begin() + emp_at_idx);

  // add employee to new manager
  this->subs[idx_new_manager].emplace_back(idx_emp);
  
}

int Hierarchy::find_level_employee(const int idx) const
{
  if(idx == 0)    //the boss
  {
    return  0;
  }
  if(idx!= -1)
  {
      const string name = get_name_by_idx(idx);
      return 1 + this->find_level_employee(find_manager(name));
  }
  return -1;
 
}
std::vector<int> Hierarchy::get_emp_with_manager(const int& idx) const
{
  std::vector <int> result;
  const int size = this->employees.size();
  
  for (int i = 0; i < size; i++)
  {
    const int man_idx = find_manager(this->employees[i]);
    if( man_idx == idx && !(i ==0 && man_idx == 0))
    {
      result.emplace_back(i);
    }
  }

  return result;
}

std::vector<Team> Hierarchy::teams_at(const int& level) const
{
  const int size = this->employees.size();
  std::vector<bool> managers(size,true);
  std::vector<Team> result;
  if ( level > this->longest_chain())
  {
    throw std::invalid_argument("Invalid level index");
  }
  if ( level == 0)
  {
     std::vector<int> t;
    Team current =Team(t);
    current.set_manager(0);
    managers[0] = false;
     result.emplace_back(current);
    return result;
  }

  for( int i = 0; i < size; i++)
  {
      if( this->find_level_employee(i) == level)
      {
       const int man_idx = find_manager(this->employees[i]);
       
       if (managers[man_idx]) //would mean that we haven't created the teams of this manager
       {
         std::vector<int> team_mates = get_emp_with_manager(man_idx);
         Team current= Team(team_mates);
         current.set_manager(man_idx);
         result.emplace_back(current);

         managers[man_idx] = false;
       }      
      }
  }

  return result;
}


std::vector<std::vector<Team>> Hierarchy::get_teams() const
{
  std::vector<std::vector<Team>> result;
  const int deepest_level = this->longest_chain();
  for( int i = 0; i < deepest_level; i++)
  {
    std::vector<Team> current = this->teams_at(i);
    result.emplace_back(current);
  }

  return result;
}

void Hierarchy::incorporate()
{
 const std::vector<std::vector<Team>> teams = this->get_teams();
 std::vector <int> managers_to_incorporate;
    
 const int size = teams.size();
    for(int i = size - 1; i >= 0; i--)
    {
      const int current = teams[i].size();
      for(int j=0; j< current;j++)
      { 
        if(teams[i][j].team_mates.size() >= 2 )
        {
          const int current_man = teams[i][j].get_manager();
          managers_to_incorporate.emplace_back(current_man);
        }
      }
    }

const int size_imcorp = managers_to_incorporate.size();
    for( int i = 0; i < size_imcorp; i++)
    {
      auto & current = this->subs[managers_to_incorporate[i]];
      const int size_current = current.size();
      const int max_salaries_id = find_employee_with_highest_salary(current); 

      if(max_salaries_id == -1) //more than one person with the highest salary
      {
        std::vector <int> max_salaries_ids (get_vector_employees_id_highest_salary(current));
        if(!max_salaries_ids.empty())
        {
          const int smallest_emp_id = smallest_employee_from(0,max_salaries_ids); //find lexicographically smallest employee with the highest salary
          if( smallest_emp_id != -1)
          {
            const auto smallest_in_row = std::find(current.begin(), current.end(), smallest_emp_id);
            if( smallest_in_row == current.end())
            {
              throw std::invalid_argument("Invalid smallest employee");
            }
            const int idx_smallest_in_row = smallest_in_row - current.begin();
            std::vector <int> to_reasign_managers;
            for(int k = 0; k < size_current;k++)
            {
              if ( k== idx_smallest_in_row)
              {
                continue;
              }
              to_reasign_managers.emplace_back(current[k]);
            }
            for( int j = 0; j < to_reasign_managers.size(); j++)
            {
              this->reasign_manager(to_reasign_managers[j], smallest_emp_id);
            }
          }
         }
      }

       //only one employee with the highest salary
      else 
      {
        const auto max_salary_row = std::find(current.begin(), current.end(), max_salaries_id);
        if( max_salary_row == current.end())
        {
          throw std::invalid_argument("Invalid employee with greatest salary");
        }
        const int idx_max_in_row = max_salary_row - current.begin();
        std::vector<int> to_reasign_managers;
        for(int k = 0; k <size_current; k++)
        {
          if(k == idx_max_in_row)
          {
            continue;
          }
         to_reasign_managers.emplace_back(current[k]);
        }
        for( int j = 0; j < to_reasign_managers.size(); j++)
            {
              this->reasign_manager(to_reasign_managers[j], max_salaries_id);
            }

      }
    }
}
 
 std::vector<std::vector<Team>> Hierarchy::uneven_levels_teams() const
{ 
  std::vector<std::vector <Team>> res;
  std::vector<std::vector<Team>> teams = this->get_teams();
  const int size = teams.size();
  for( int i = 0; i < size; i++)
  {
    if( i% 2 != 0)  //emplace all the teams on uneven levels
    {
      res.emplace_back(teams[i]); 
    }
  }
  return res;
}

const bool Hierarchy::is_manager(const int who) const
{
  return !this->subs[who].empty();  // if subs at that index is empty => not a manager
}

void  Hierarchy::modernize()
{
  //save original uneven levels
  std::vector<int> uneven_levels;
  const std::vector<std::vector <Team>> teams = this->get_teams();
  const int size = teams.size();
  for( int i = size - 1; i >= 0; i--)
  {
    if( i % 2 != 0)   //if we are on an uneven level
    {
      uneven_levels.emplace_back(i);
    }
  }

  const int uneven = uneven_levels.size();
  const int subs_size = this->subs.size();

  std::vector <string> to_be_modernized;

  for(int g = 0; g < uneven ; g++ ) // start from the lowest uneven level
  {
    for( int i =subs_size - 1; i >= 0 ; i--)
    {
      const int current_row_size = this->subs[i].size();
      for(int j = 0; j < current_row_size; j++)
      {
        const int current_emp = this->subs[i][j];
        const int level_emp = this->find_level_employee(current_emp);
        if(level_emp == uneven_levels[g] && this->is_manager(current_emp)) // if the employee is on an uneven level and is a manager
        {       
          to_be_modernized.emplace_back(this->get_name_by_idx( current_emp));
        }
      }
    }
  }
  const int modernize_size = to_be_modernized.size();
  for(int i = 0; i < modernize_size; i++)
  {
    const int idx = this->find_id(to_be_modernized[i]);
    const int new_manager = this->find_id( manager(to_be_modernized[i]));
    const int cnt_subs = this->subs[idx].size();
    for( int k = 0; k < cnt_subs; k++)
        {
          this->reasign_manager(this->subs[idx][k] ,new_manager);  // switch the manager
        }
    this->fire(this->get_name_by_idx(idx));
  }
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
  Hierarchy result;
  if( *this == right)
  {
    return *this;
  }

  const int idx_boss = this->find_id(TheBoss);
  const int idx_boss_r = this->find_id(TheBoss);

  if ( idx_boss == -1 || idx_boss_r == -1)
  {
    throw std::invalid_argument("The boss was not found");
  }
  if(!this->check_the_boss(right))
  {
    throw std::invalid_argument("The boss is not same in the hierarchies");
  }

  const int size_h1 = this->subs.size();
  const int size_h2 = right.subs.size();

  for( auto& s:this->subs)    //starting by looking the employees at one of the hierarchies
  {
    for( auto& emp:s)
    {
      const string current_emp = this->get_name_by_idx(emp);
      if(!right.find(current_emp))    //if the employee is not in the second hierarchy
      {
        result.hire(current_emp, this->manager(current_emp));

      
        const int index_employee = emp;
        const auto & current_subs  = this->subs[index_employee];
        const int subs_current = current_subs.size();
        for( int i = 0; i < subs_current; i++)  //hire all their direct subordinates from the first hierarchy
        {
          const string to_hire = this->get_name_by_idx(current_subs[i]);
          result.hire(to_hire, current_emp);
        }
      }
      else      // is in both hierarchies
      {
        const string manager_1 = this->manager(current_emp);
        const string manager_2 = right.manager(current_emp);

        if( manager_1 == manager_2) // if managers are the same, hire all the different subordinates of current employee from both hierarchies
        {
          result.hire_from_2_hierarchies_one_manager(current_emp, *this, right, manager_1);
        }

        // find the manager at a higher level
        const int man_1_level = this->find_level_employee(this->find_id(manager_1));
        const int man_2_level = right.find_level_employee(right.find_id(manager_2));

        if( man_1_level == man_2_level && manager_1!= manager_2)   // have to choose the lexicographically smaller one
        {
          const string final_manager = manager_1 < manager_2 ? manager_1 : manager_2;
          result.hire_from_2_hierarchies_one_manager(current_emp, *this, right, final_manager);

        }
        if ( man_1_level < man_2_level)   //manager in the first hierarchy is higher
        {
          result.hire_from_2_hierarchies_one_manager(current_emp, *this, right, manager_1);
        }
        if( man_2_level < man_1_level) 
        {
          result.hire_from_2_hierarchies_one_manager(current_emp, *this, right, manager_2);
        }
      }

    }

  }

  for( auto& s2 : right.subs)
  {
    for(auto & emp2 : s2)
    {
      const string current_emp = right.get_name_by_idx(emp2);
      if(!result.find(current_emp))   // not in the result hierarchy yet
      {
        result.hire(current_emp, right.manager(current_emp));

        const int emp_idx = right.find_id(current_emp);
        const auto& current_subs = right.subs[emp_idx];
         const int subs_current = current_subs.size();
        for( int i = 0; i < subs_current; i++)  //hire all their direct subordinates from the first hierarchy
        {
          const string to_hire = right.get_name_by_idx(current_subs[i]);
          result.hire(to_hire, current_emp);
        }
      }
    }
  }

  if(!this->check_for_possible_join(right))
  {
    if(this->has_loop(result))
    {
    Hierarchy empty;
    return empty;
    }
  
  }
  return result;
}

const bool Hierarchy::is_indirect_manager(const int manager, const int to_whom) const
{
  const int size = this->subs[manager].size();

  for( int i = 0; i < size; i++)
  {
    if( this-> subs[manager] [i] == to_whom)
    {
      return true;
    }
     return this->is_indirect_manager(this->subs[manager][i], to_whom);
  }

  return false;
}

 const bool Hierarchy::indirect_manager(const string& manager, const string& to_whom) const
 {
   const int man_id = this->find_id(manager);
   if( man_id == -1)
   {
     throw std::invalid_argument("Invalid manager index/name");
   }
   const int emp_id = this->find_id(to_whom);
   if ( emp_id == -1)
   {
        throw std::invalid_argument("Invalid employee index/name");
   }

   return this->is_indirect_manager(man_id, emp_id);
 }

const bool Hierarchy::check_the_boss( const Hierarchy& h2) const
{
    return this->manager(TheBoss) == "" && h2.manager(TheBoss) == "";
}

const std::vector<int> Hierarchy::get_subs_of(const int id) const
{
  if( id < 0)
  {
    throw std::invalid_argument("Invalid index");
  }
  return this->subs[id];

}

void Hierarchy::hire_from_2_hierarchies_one_manager( const string& subs_of, const Hierarchy& first, const Hierarchy& second, const string& manager)
{
  this->hire(subs_of, manager);
  const int idx_1 = first.find_id(subs_of);
  const int idx_2 = second.find_id(subs_of);

  // get the subordinates of the employees of both hierarchies
  const std::vector<int> &subs_1 = first.get_subs_of(idx_1);
  const std::vector<int> &subs_2 = second.get_subs_of(idx_2);
  const int size_1 = subs_1.size();
  const int size_2 = subs_2.size();

  // from the first hierarchy
  for (int i = 0; i < size_1; i++) 
  {
    this->hire(first.get_name_by_idx(subs_1[i]), subs_of);
  }
  // from the second hierarchy
  for (int i = 0; i < size_2; i++) 
  {
    if (!this->find(second.get_name_by_idx(subs_2[i]))) // if it hasn't already been hired
    {
      this->hire(second.get_name_by_idx(subs_2[i]), subs_of);
    }
  }
}

 template<typename T>
 void sort_and_unique(std::vector<T> & v)
 {
   std::sort(v.begin(), v.end());
   v.erase(std::unique(v.begin(), v.end()), v.end());
 }

 bool Hierarchy::check_for_possible_join(const Hierarchy& h) const
 {
   // get the names of all employees
   std::vector<std::string> merged_names = this->employees;
   for (const std::string & name : h.employees) merged_names.emplace_back(name);
   sort_and_unique(merged_names);

   const auto & names1 = this->employees;
   const auto & names2 = h.employees;
   using IdMap = std::vector<int>;
   IdMap map1(names1.size());
   IdMap map2(names2.size());

   auto get_new_id = [&](const std::string & s) -> int
   {
     const auto it = std::lower_bound(merged_names.begin(), merged_names.end(), s);
     return it - merged_names.begin();
   };

   auto fillMap = [&](const std::vector<std::string> & n, IdMap & m)
   {
     for (int i = 0; i < n.size(); i++)
     {
       m[i] = get_new_id(n[i]);
     }
   };

   // map the names of the employees of both hierarchies
   fillMap(names1, map1);
   fillMap(names2, map2);

   const int N = merged_names.size();
   std::vector<std::vector<int>> nsubs(N);

   auto copySubs = [&](const std::vector<std::string> & n, const std::vector<std::vector<int>> & s,
                       const IdMap & m)
   {
     for (int i = 0; i < n.size(); i++)
     {
       const int v = get_new_id(n[i]);
       auto & nvs = nsubs[v];
       for (int os : s[i]) nvs.emplace_back(m[os]);
     }
   };

    // get subs of all employees from both hierarchies
   copySubs(names1, this->subs, map1);
   copySubs(names2, h.subs, map2);

   // do DFS searching for a loop
   std::vector<bool> visited(N, false);
   std::stack<int> s;
   bool has_loop = false;
   for (int i = 0; i < N; i++)
   {
     if (visited[i]) continue;
     visited[i] = true;
     s.push(i);

     while (!s.empty())
     {
       const int curr = s.top();
       s.pop();

       const auto & sc = nsubs[curr];
       for (int next : sc)
       {
         if (visited[next])
         {
           has_loop = true;
           break;
         }

         visited[next] = true;
         s.push(next);
       }
     }

     if (has_loop) break;
   }

   return !has_loop;
 }


 bool Hierarchy:: has_loop(const Hierarchy& h) const
 {
   const int size = h.employees.size();
   std::vector <bool> visited(size, false);
   std::stack<int> s;
   bool has_loop = false;
   for (int i = 0; i < size; i++)
   {
     if (visited[i]) continue;
     visited[i] = true;
     s.push(i);

     while (!s.empty())
     {
       const int curr = s.top();
       s.pop();

       const auto & sc = h.subs[curr];
       for (int next : sc)
       {
         if (visited[next])
         {
           has_loop = true;
           break;
         }

         visited[next] = true;
         s.push(next);
       }
     }

     if (has_loop) break;
   }
   return has_loop;
 }