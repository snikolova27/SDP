#include "interface.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

void Hierarchy::read_one_line() const
{

}

Hierarchy::Hierarchy(const string& data)
{
  std::istringstream ss(data);
  std::string line;
  while (std::getline(ss, line))
  {
    const auto dashIdx = line.find('-');
    if (dashIdx == line.npos) continue;
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
      // this->employees.emplace_back(sub);
      this->subs.emplace_back();
      // this->subs.emplace_back();
      // this->subs[0].emplace_back(1);
      std::cout << this->employees.size()  << std::endl;
      //this->subs.emplace_back(0);
      std::cout << this->subs.size()  << std::endl;
     // this->subs[0].emplace_back();
      std::cout <<this->subs[0].size() << std::endl;
      //hire(sub,man);
    }
  
  
   hire(sub, man);
   
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

string Hierarchy::print_subs_of ( const int idx) const
{
  if( idx >= this->employees.size() || idx < 0)
  {
     throw std::invalid_argument("Invalid index");
  }
  string res = "";
  const int current_size = this->subs[idx].size();
  std::vector<string> current_names;
  for( int k = 0; k < current_size; k++)
   {
      current_names.emplace_back(this->employees[this->subs[idx][k]]);
   }
  std::sort(current_names.begin(), current_names.end());

  for( int k = 0; k < current_size; k++)
   {
     res = res + this->get_name_by_idx(idx) + " - " + current_names[k] + "\n";
   }

return res;
}

string Hierarchy::print() const 
{
    string res = "";
    const int size = this->employees.size();
    if(size <= 0)
    {
      return res;
    }

    const std::vector<string> & names = this->employees;

    std::vector<int> sorted_ids(names.size() - 1); // Without the boss
    for ( int j = 1; j < size ; j++ ) 
    {
      sorted_ids.emplace_back(j);
    }
   // std::iota(sorted_ids.begin(), sorted_ids.end(), 1); 
    std::sort(sorted_ids.begin(), sorted_ids.end(), [&](int a, int b) { return names[a] < names[b]; });

    //print the subordinmtes of the Boss
    res += this->print_subs_of(0);

    // print the subordinates of the rest of the employees
    for ( int i = 0; i < size - 1; i++)
    {
     res += this->print_subs_of(sorted_ids[i]);
    }

    return res;
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
  if (boss_id < 0)  // invalid boss name
  {
    return false;
  } 
  if (find(who))  //would mean that the person is already hired
  {
    return false;
  } 

  const int emp_id = this->employees.size();    //new employee id
  this->employees.emplace_back(who);
  std::cout << "In hire " << this->employees[emp_id] << std::endl;
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

  this->subs.erase(this->subs.begin() + id);
  this->employees.erase(this->employees.begin() + id);

  const int size = this->subs.size();
  for ( int i = 0; i < size; i++)
  {
      const int inner_size = this->subs[i].size();
      for( int k = 0; k < inner_size; k++)
      {
         if(id > this->subs[i][k])
         {
           this->subs[i][k]-=1;
         }
      }
  }
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
   if (manager_id < 0)
   {
     throw std::invalid_argument("No manager");
   }
   if ( manager_id == 0)
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