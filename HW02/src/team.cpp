#include "team.h"
#include <stdexcept>

Team::Team(std::vector<int>& mates) 
{
    this->team_mates = mates;
}

void Team::add_mate(const int& mate_idx)
{
    if (mate_idx == -1)
    {
        throw std::invalid_argument("Invalid index");
    }
    this->team_mates.emplace_back(mate_idx);
}
void Team:: set_manager(const int& man_idx)
{
    this->manager = man_idx;
}

void Team::print()
{
    std::cout << "Manager: " << manager << std::endl;
    std::cout << "Team mates: " << std::endl;
    for(int i = 0; i < this->team_mates.size();i++)
    {
        std::cout << this->team_mates[i] << std::endl;
    }
}

int Team:: size() const
{
    return this->team_mates.size();
}

int Team::get_mate_at(const int index) const
{
    if( index > this->size() || index < 0)  //invalid index
    {
        return -1;
    }
    return this->team_mates[index];
}