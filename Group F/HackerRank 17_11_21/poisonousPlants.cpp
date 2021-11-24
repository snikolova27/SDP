#include <vector>
#include <iostream>

bool isDescending(const std::vector<int> &v)
{
    size_t len = v.size();

    for (size_t i = 0; i < len - 1; ++i)
    {
        if (v[i] < v[i + 1])
        {
            return false;
        }
    }
    return true;
}

void day(std::vector<int> &v)
{
    size_t len = v.size();

    for (size_t i = len-1; i >=1 ; --i)
    {
        if (v[i] > v[i - 1])
        {
            v.erase(v.begin() + i);

        }
    }
}

void print(std::vector<int>& v)
{
    int len = v.size();

    for(int i=0; i<len;i++)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int poisonousPlants(std::vector<int> p)
{
    size_t days = 0;

    while (!isDescending(p))
    {
        day(p);
        days++;
    }
    return days;
}


int main()
{
    std::vector<int> v1;
    v1.push_back(3);
    v1.push_back(6);
    v1.push_back(2);
    v1.push_back(7);
    v1.push_back(5);

    //6 5 8 4 7 10 9

    std::vector<int> v2;
    v2.push_back(6);
    v2.push_back(5);
    v2.push_back(8);
    v2.push_back(4);
    v2.push_back(7);
    v2.push_back(10);
    v2.push_back(9);

    std::cout << poisonousPlants(v1) << std::endl;

    std::cout << poisonousPlants(v2) << std::endl;
    return 0;
}