#include "../include/commands.h"
#include "../include/commandTokenizer.h"

#include <cmath>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <string>

void CommandProcessor::process_save_command(const SaveCommand& cmd)
{
    const std::string name = cmd.object_name;
    if(!this->is_in(name))
    {
        throw std::logic_error("Tree does not exist");
    }

    const auto it = std::find(this->names.begin(), this->names.end(), name);
    const int idx = it - this->names.begin();

    if(cmd.file_name == "") //print on console
    {
       this->trees[idx].print(std::cout);
       std::cout << "\n";
    }
    else 
    {
        const std::string file_name = cmd.file_name;
        std::ofstream write;
        write.open(file_name);
        this->trees[idx].print(write);
        write.close();
    }

}
void CommandProcessor::process_load_command(const LoadCommand& cmd)
{
    const std::string name = cmd.object_name;
    if( this->is_in(name) && !this->names.empty())
    {
        //TODO
        // презаписва старата информация
    }

    std::string src;
    if( cmd.file_name == "") //read from console
    {
        std::cout << "Write the tree" << std::endl;
        src = get_str_from_stream(std::cin,true);
    }
    else 
    {
       std::ifstream file(cmd.file_name);
       src = get_str_from_stream(file,false);
    }
    this->trees.emplace_back(N_aryTree(name, src));
    this->names.emplace_back(name);   
}

void CommandProcessor::process_contains_command(const ContainsCommand& cmd)
{
    const std::string firstTree = cmd.get_first_tree_name();
    const std::string secondTree = cmd.get_second_tree_name();
    if( this->is_in(firstTree) && this->is_in(secondTree))
    {
        const auto it = std::find(this->names.begin(), this->names.end(), firstTree);
        const int idxFirstTree = it - this->names.begin();
        const auto it2 = std::find(this->names.begin(), this->names.end(), secondTree);
        const int idxSecondTree = it2 - this->names.begin();
        std::cout << std::boolalpha << this->trees[idxFirstTree].contains(this->trees[idxSecondTree]) << std::endl;
    }
    else if(!this->is_in(firstTree))
    {
        throw std::logic_error("The first tree does not exist");
    }else{
        throw std::logic_error("The second tree does not exist");
    }
}


void CommandProcessor::process_command(const std::string& cmd_str)
{

    const CommandTokens tokens = tokenize(cmd_str);
    // Tokenizer always returns lower-case commands
    if(tokens.command == "HELP")
    {
        this->help(std::cout);
    }

    else if(tokens.command == "EXIT")
    {
        this->exit();
    }

    else if (tokens.command == "SAVE")
    {
        SaveCommand cmd(tokens.arguments);
        this->process_save_command(cmd);
    }
    else if (tokens.command == "LOAD")
    {
        LoadCommand cmd(tokens.arguments);
        this->process_load_command(cmd);
    }
    else if (tokens.command == "CONTAINS")
    {
        ContainsCommand cmd(tokens.arguments);
        this->process_contains_command(cmd);
    }
    else if (tokens.command == "REMOVE")
    {
        RemoveCommand cmd(tokens.arguments);
        this->process_remove_command(cmd);
    }
    else
    {
        throw std::invalid_argument("Unknown command: " + tokens.command);
    }
    
}
void CommandProcessor::help( std::ostream & stream)
{
    stream << " =========  List of commands  =========" << std::endl;
    stream << "> HELP - prints out the list of commands" << std::endl;
    stream << "> SAVE - <tree name> <file name> - saves the tree with given name in the given file" << std::endl;
    stream << "> LOAD - <tree name> <file name>- loads the tree with given name from the given file" << std::endl;
    stream << "> CONTAINS - <tree1 name> <tree2 name> - returns if tree2 is a subtree of tree1" << std::endl;
    stream << "> EXIT - exits the program" << std::endl;

}
int CommandProcessor::exit() const
{
    return 0;
}


bool CommandProcessor::is_in(const std::string& treeName) const 
{   
    const auto it = std::find(this->names.begin(), this->names.end(), treeName);
    return it != this->names.end();

}
void CommandProcessor::add_tree(const std::string& name,  const N_aryTree& to_add)
{
    if(this->is_in(name)){
        //TODO
        // презаписва старата информация
    }
    this->names.emplace_back(name);
    this->trees.emplace_back(to_add);
}

LoadCommand::LoadCommand( const std::vector <std::string>& args) :FileCommands()
{
    if ( args.size() == 1)
    {
        this->object_name = args[0];

    }    if( args.size() > 2)
    {
        throw std::logic_error("Load command takes up to 2 arguments");
    }
    if ( args.size() == 2)
    {
        this->object_name = args[0];
        this->file_name = args[1];
    }
}
SaveCommand::SaveCommand(const std::vector<std::string>& args) :FileCommands()
{
    if ( args.size() == 1)
    {
        this->object_name = args[0];
    }
    if( args.size() > 2)
    {
        throw std::logic_error("Save command takes up to 2 arguments");
    }
    if ( args.size() == 2)
    {
        this->object_name = args[0];
        this->file_name = args[1];
    }
}

TreesCommands::TreesCommands(const std::vector <std::string>& args)
{
    if( args.size() != 2 )
    {
        throw std::logic_error("Tree commands take exactly 2 arguments");
    }

    this->first_tree_name = args[0];
    this->second_tree_name = args[1];   
}

std::string get_str_from_stream(std::istream& stream, const bool break_on_empty_line)
{
    std::stringstream ss;
    std::string line;
    while (std::getline(stream, line))
    {
        if (line == "")
        {
            if (break_on_empty_line) break;
            continue;
        }
        ss << line << "\n";
    }
    return ss.str();
}