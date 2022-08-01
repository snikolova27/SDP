#pragma once
#include "commandTokenizer.h"
#include "n-aryTree.h"
#include <iostream>
#include <string>
#include <vector>

class FileCommands
{
    public:
    std::string object_name;
    std::string file_name;
  //  FileCommands();
   // FileCommands( const std::vector <std::string>& args);
   // FileCommands(std::string& obj, std::string& file) : object_name(obj), file_name(file){};
   
};

class LoadCommand : public FileCommands
{
    public:
    LoadCommand (const std::vector <std::string>& args) ;
};

class SaveCommand : public FileCommands
{
    public:
    SaveCommand(const std::vector <std::string>& args);
};

class TreesCommands
{
    private:
    std::string first_tree_name;
    std::string second_tree_name;
    public:
    TreesCommands(std::string& tree1, std::string& tree2) : first_tree_name(tree1), second_tree_name(tree2) {};
    TreesCommands(const std::vector <std::string>& args);
    const std::string& get_first_tree_name() const;
    const std::string& get_second_tree_name() const;
};

class ContainsCommand : public TreesCommands 
{
    public:
    ContainsCommand(const std::vector <std::string>& args) : TreesCommands(args){};
    
};

class RemoveCommand : public TreesCommands 
{
    public:
    RemoveCommand(const std::vector <std::string>& args) : TreesCommands(args){};
};


class CommandProcessor
{
    private:
    std::vector<N_aryTree> trees;
    std::vector<std::string> names;

    void process_save_command(const SaveCommand& cmd);
    void process_load_command(const LoadCommand& cmd);
    void process_contains_command(const ContainsCommand& cmd);
    void process_remove_command(const RemoveCommand& cmd);
   
    bool is_in(const std::string& treeName) const;
    void add_tree( const std::string& name, const N_aryTree& to_add);
    
    public:
    CommandProcessor() = default;
    CommandProcessor(std::vector<N_aryTree> trees, std::vector<std::string> names) : trees(trees), names(names){};
    void process_command(const std::string& cmd_str);
    void help( std::ostream & stream);
    int exit() const;
};

std::string get_str_from_stream(std::istream& stream, const bool break_on_empty_line);