#pragma once
#include <string>
#include <vector>

using std::string;

class Hierarchy
{
public:
    // Hierarchy(Hierarchy&& r) noexcept;
    // Hierarchy(const Hierarchy& r);
    // Hierarchy(const string& data);
    // ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print()const;

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

    Hierarchy join(const Hierarchy& right) const;

    //If you need it - add more public methods here
private:
    std::vector<std::string> employees;
    std::vector<std::vector<int>> subs;

    // -1 if not found, valid index otherwise
    int find_id(const std::string & name) const;

    int find_manager(const std::string& name) const;

    int cnt_all_subs(const int id) const;

    int cnt_non_direct_subordinates(const int id) const;

    int dfsl(const int id) const;

};