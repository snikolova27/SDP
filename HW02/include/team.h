#pragma once
#include <vector>
#include <iostream>
 
 struct Team
 {
     private:
     
     int manager;

     public:
     std::vector<int> team_mates;
     Team();
     Team(std::vector <int>& mates);
     void add_mate(const int& mate_idx);
     void set_manager(const int& man_idx);
     void print();
     int size() const;
     int get_mate_at(const int index) const;
 };