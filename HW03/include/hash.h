#include <stdexcept>
#include <string>
#include <vector>

template<typename T>
class HashMap
{
private:
    const int N;
    const int mult;

    std::vector<std::string> strs;
    std::vector<T> contents;

public:
    HashMap(const int capacity, const int mult) : N(capacity), mult(mult)
    {
        strs.resize(N);
        contents.resize(N);
    }

    void put(const std::string & key, const T value)
    {
        const int idx = hash(key);
        strs[idx] = key;
        contents[idx] = value;
    }

    bool check(const int idx) const
    {
        return strs[idx] != "";
    }

    bool check(const std::string & key) const
    {
        const int h = hash(key);
        return strs[h] == key;
    }

    const T & operator[](const int idx) const
    {
        return contents[idx];
    }

    T & operator[](const int idx)
    {
        return contents[idx];
    }

    const T & operator[](const std::string & key) const
    {
        const int idx = hash(key);
        return contents[idx];
    }

    T & operator[](const std::string & key)
    {
        const int idx = hash(key);
        return contents[idx];
    }

    int hash(const std::string & str) const
    {
        int h = 0;
        for (int i = 0; i < str.size(); i++)
        {
            h = (h * mult + str[i]) % N;
        }

        const int h0 = h;
        while (h < N && (strs[h] != "" && strs[h] != str)) h++;
        if (h >= N) h = 0;
        while (h < h0 && (strs[h] != "" && strs[h] != str)) h++;
        if (h == h0) throw std::logic_error("Out of capacity");
        return h;
    }
};