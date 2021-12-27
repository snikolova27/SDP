#include "interface.h"

#include <algorithm>
#include <stdexcept>

string Hierarchy::print() const { return "water sheep"; }

int Hierarchy::num_employees() const { return this->employees.size(); }

bool Hierarchy::find(const string &name) const { return find_id(name) >= 0; }

int Hierarchy::find_id(const std::string &name) const
{
  const auto it = std::find(this->employees.begin(), this->employees.end(), name);
  if (it == this->employees.end()) return -1;
  return it - this->employees.begin();
}

bool Hierarchy::hire(const string &who, const string &boss) 
{
  const int boss_id = find_id(boss);
  if (boss_id < 0)
  {
    return false;
  } 
  if (find(who)) return false;

  const int emp_id = this->employees.size();
  this->employees.emplace_back(who);
  this->subs.emplace_back();
  this->subs[boss_id].emplace_back(emp_id);
  return true;
}

bool Hierarchy::fire(const string& who)
{
  const int id = find_id(who);
  if (id <= 0) return false;

  const int manager_id = find_manager(who);
  if(manager_id < 0) return false;

  const int cnt_subs_emp = this->subs[id].size(); 
  for( int i = 0; i < cnt_subs_emp; i++)
  {
    int toInsert = this->subs[id][i];
    this->subs[manager_id].emplace_back(toInsert);
  }

  this->subs.erase(this->subs.begin() + id);
  this->employees.erase(this->employees.begin() + id);

  const int size = this->subs.size();
  for ( int i = 0; i < size; i++)
  {
      const int innerSize = this->subs[i].size();
      for( int k = 0; k < innerSize; k++)
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

  const int size = this->subs.size();
  for (int i = 0; i < size; i++)
  {
    const auto & s = this->subs[i];
    const auto it = std::find(s.begin(), s.end(), emp_id);
    if (it != s.end()) return i;
  }
  return -1; // Nobody is the boss of him
  
  // for (int i = 0; i < size; i++)
  // {
  //   int innerSize = this->subs[i].size();
  //   for( int j = 0; j < innerSize; j++)
  //   {
  //      if(emp_id ==  this->subs[i][j])
  //      {
  //        return i;
  //      }`
  //   }
  // }
  // return -1; // manager was not found
}

 string Hierarchy::manager(const string& name) const
 {
   const int manager_id = find_manager(name);
   if (manager_id < 0)
   {
     throw std::invalid_argument("No manager");
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
  for (int sub : s) sum += cnt_all_subs(sub);
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
     throw std::invalid_argument("Invalid employee");
   }
  return 500 * num_subordinates(who) + 50 * cnt_non_direct_subordinates(emp_id);
}

int Hierarchy::num_overloaded(int level) const
{
  int cnt = 0;
  const int size = this->subs.size();

  for(int i=0; i < size; i++)
  {
    if ( cnt_all_subs(i) > level)
    {
      cnt++;
    }
  }
  return cnt;
}

  int Hierarchy::dfsl(const int id) const
  {
    int m = 0;
    for (int sub : this->subs[id]) m = std::max(m, dfsl(sub));
    return m + 1;
  }

 int Hierarchy::longest_chain() const
 {
    return dfsl(0);
 }
