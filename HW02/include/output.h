#pragma once
#include <string>

using std::string;

class Output
{
    private:
    string output = "";
    string fileToWriteIn;

    public:
    Output(const string& output, const string& file);
    //TODO:write write func
    void writeInFile();

};