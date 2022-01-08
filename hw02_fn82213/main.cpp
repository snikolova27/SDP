#include "commands.h"
#include "interface.h"
#include "team.h"
#include "iostream"

int main(int argc, char **argv)
{
  CommandProcessor cmd;
  cmd.help(std::cout);
  std::string line;
    while (std::getline(std::cin, line))
    {
        try
        {
            if ( line == "exit" || line == "EXIT")
            {
               return cmd.exit();
            }
            cmd.process_command(line);
            std::cout << " ========  Command OK  ========" << std::endl;
        }
        catch (const std::exception &e)
        {

            std::cerr << "Command FAILED: " << e.what() << std::endl;
        }
    }
    return 0;
}