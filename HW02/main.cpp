#include "interface.h"
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
    Hierarchy h(lozenec);
    // std::cout << h.find_level_employee(12) << std::endl;
    // std::cout << h.get_name_by_idx(12) << std::endl;
   // std::cout << h.print();
    return 0;
}