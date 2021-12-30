#pragma once
#include <vector>
#include <iostream>
 
 struct Team
 {
     private:
     std::vector<int> team_mates;
     int manager;
     public:
     Team();
     Team(std::vector <int>& mates);
     void add_mate(const int& mate_idx);
     void set_manager(const int& man_idx);
     void print();
 };