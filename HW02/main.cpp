#include "interface.h"
#include "team.h"
#include "iostream"

int main()
{
    const string lozenec =
    "Uspeshnia - Gosho \n"
    "Uspeshnia - Misho \n"
    "Gosho     - Pesho \n"
    "Gosho     - Dancho\n"
    "Pesho     - Alex  \n"
    "Dancho    - Boris \n"
    "Dancho    - Kamen \n"
    "Uspeshnia - Slavi \n"
    "Slavi     - Slav1 \n"
    "Slavi     - Slav2 \n"
    "Slav1     - Mecho \n"
    "Mecho     - Q12Adl\n";

    const string large =
    "Uspeshnia - 1\n"
    "1-2\n1-3\n1-4\n1-5\n1-6\n1-7\n"
    "2-21\n2-22\n2-23\n2-24\n2-25\n2-26\n2-27\n2-28\n"
    "3-31\n3-32\n3-33\n3-34\n3-35\n3-36\n3-37\n3-38\n3-39\n";

    Hierarchy h(large);
    std::cout << h.longest_chain() << std::endl;
    std::vector<std::vector<Team>> teams = h.get_teams();

    std::cout <<h.print();

    for(int i = 0; i< teams.size();i++)
    {
        for(int j = 0; j < teams[i].size();j++)
        {
            std::cout << i << " " << j << std::endl;
         teams[i][j].print();
        }
   
    }
    h.incorporate();
std::cout << h.print() << std::endl;

   // teams[0][0].print(); 
   // std::cout << h.get_emp_with_greatest_salary(teams[3][1]) << std::endl;
    //std::cout << h.get_name_by_idx(h.get_emp_with_greatest_salary(teams[3][1])) << std::endl;
   // std::cout << h.get_smallest_emp(teams[3][1]) << std::endl;
   // std::cout <<h.get_name_by_idx(  h.get_smallest_emp(teams[3][1])) << std::endl;
    // std::cout << h.find_level_employee(12) << std::endl;
    // std::cout << h.get_name_by_idx(12) << std::endl;
   // std::cout << h.print();
    return 0;
}