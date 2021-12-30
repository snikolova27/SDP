#include "input.h"
#include "interface.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <numeric>
#include <queue>

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

  for(int i=0; i < size; i++)
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

unsigned long Hierarchy::max_salary(const std::vector <int>& employees) const
{
  unsigned long max = 0;
  unsigned long current_max = 0;
  const int size = employees. size();
  for(int i = 0; i < size; i++)
  {
    current_max = this->getSalary(this->employees [employees[i]]);
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
    if (current_max  > max  && current_max != (unsigned long) -1)
    {
      max_id = i;
    }
    else if (current_max == max && current_max != (unsigned long) -1)
    {
      cnt_with_max_salary++;
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
        res.emplace_back(i); //put the index of the employee in the result vector
      }
    }

    return res;
}

// int Hierarchy::smallest_employee(const std::vector <int>& employees) const
// {
//   const int size = employees.size();
//   if ( size == 0)
//   {
//     return -1; //empty vector
//   }

//   string current_smallest = this->employees[employees[0]];
//   int smallest_id = -1;
//   for (int i = 1; i < size ; i++)
//   {
//     if (this->employees[employees[i]] < current_smallest)
//     {
//       current_smallest = this->employees[employees[i]];
//       smallest_id = employees[i];
//     }
//   }
//   return smallest_id;
// }

int Hierarchy::smallest_employee_from(const int from, const std::vector <int>& employees) const
{
  const int size = employees.size();
  if ( size == 0)
  {
    return -1; //empty vector
  }

  string current_smallest = this->employees[employees[from]];
  int smallest_id = -1;
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

void Hierarchy::remove_connection(const int id_to_remove)
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
            remove_connection(v[j]); //remove all manager connections of the other employees in this subs row
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
  if( current_manager == -1 ||  current_manager == 0)
  {
    throw std::invalid_argument ("There is not a valid current manager");
  }

  // remove employee from current manager

  // find position in the vector of employees of the current manager
  const auto pos = std::find(this->subs[current_manager].begin(), this->subs[current_manager].end(), idx_emp);
  if ( pos == this->subs[current_manager].end())
  {
    throw std::invalid_argument("Employee not found");
  }
  this->subs[current_manager].erase(pos);

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
    if( man_idx == idx)
    {
      result.emplace_back(i);
    }
  }

  return result;
}

std::vector<Team> Hierarchy::teams_at(const int& level) const
{
  std::vector<Team> result;
  if ( level > this->longest_chain())
  {
    throw std::invalid_argument("Invalid level index");
  }

  const int size = this->employees.size();
  std::vector<bool> managers(size,true);

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
  const int deepest_level = this->longest_chain() -1;
  for( int i = deepest_level; i >= 0; i--)
  {
    std::vector<Team> current = this->teams_at(i);
    result.emplace_back(current);
  }

  return result;
}


void Hierarchy::incorporate()
{
  const int starting_teams = this->subs.size();
  for (int i = starting_teams -1 ; i >= 0; i--)  //starting at the bottom of the tree
  {
    const auto & s = this->subs[i];
    const int current_subs = s.size();
    if( current_subs >= 2 )
    {
      if(find_employee_with_highest_salary(s) == -1)  //if more than one person with  the highest salary
      {
        std::vector <int> max_salaries_ids = get_vector_employees_id_highest_salary(s);
        if(!max_salaries_ids.empty())
        {
          const int smallest_emp_id = smallest_employee_from(0,max_salaries_ids); //find lexicographically smallest employee with the highest salary
          if( smallest_emp_id != -1)
          {
            this->incorporate_helper(smallest_emp_id , current_subs, s);
          }
          // for (int j = 0; j < smallest_emp_id;j++)
          // {
          //   remove_connection(s[j]); //remove all manager connections of the other employees in this subs row
          //   this->subs[smallest_emp_id].push_back(s[j]); //make the smallest_emp_id the manager of all other employees in this subs row
          // }
          // for( int j = smallest_emp_id + 1; j < current_subs; j++)
          // {
          //   remove_connection(s[j]);
          //   this->subs[smallest_emp_id].push_back(s[j]);
          // }
        }
      }

      const int max_salaries_id = find_employee_with_highest_salary(s);  //only one employee with the highest salary
      if (max_salaries_id != -1)
      {
          this->incorporate_helper(max_salaries_id, current_subs, s);
      }
    
      // for (int j = 0; j < max_salaries_id;j++)
      //     {
      //       remove_connection(s[j]); //remove all manager connections of the other employees in this subs row
      //       this->subs[max_salaries_id].push_back(s[j]); //make the smallest_emp_id the manager of all other employees in this subs row
      //     }
      //     for( int j = max_salaries_id + 1; j < current_subs; j++)
      //     {
      //       remove_connection(s[j]);
      //       this->subs[max_salaries_id].push_back(s[j]);
      //     }
    }
  } 
}


std::vector<int> Hierarchy::uneven_levels_teams() const
{
  const int size = this->employees.size();
  std::vector <int> uneven;
  for (int i = 0; i < size; i++)
  {
    const auto & s = this->subs[i];
    if( i % 2 != 0 && s.size() > 0)
    {
      uneven.emplace_back(i);
    }
  }
  return uneven;
  
}

void  Hierarchy::modernize()
{

}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
  return *this;
}