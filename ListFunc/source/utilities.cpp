#include "utilities.h"
#include "analyzer.h"
#include "commandReader.h"
#include "expressionElements.h"
#include "interpreter.h"

bool isLetter(const char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isDigit(const char ch)
{
    return ch >= '0' && ch <= '9';
}


void run(std::istream& in, std::ostream& out)
{
    std::string input;
    Interpreter interpreter; // we need it to store the user defined functions

    while(true)
    {
        out << "ListFunc > ";
        getline(in, input);

        if(input == "exit") {break;}

        commandReader reader(input);

        std::vector<Token*> tokens; // used so that the commandReader and the analyzer don't interfere

        if(!reader.tokenize(tokens,out))
        {
            continue; // continue if the input is wrong
        } 

        Analyzer analyzer(tokens);

        for(auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            *it = nullptr;
        }

        Element* element = analyzer.analyze(out);

        if(!element)
        {
            continue; // no need to interpret it
        }

        interpreter.interpret(element,out); // any errors will be printed in the output stream

        out << std::endl;
    }
}
