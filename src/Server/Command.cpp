#include "Command.hpp"

namespace on
{

Command::Command()
{
}

Command::Command(std::string const& name, Command::Function func, bool adminOnly)
: mName(name)
, mFunction(func)
, mAdminOnly(adminOnly)
{
}

std::string Command::getName() const
{
    return mName;
}

void Command::execute(std::string const& args, std::string const& executant, bool isServer)
{
    if (mFunction)
    {
        mFunction(args,executant,isServer);
    }
}

bool Command::isAdminOnly() const
{
    return mAdminOnly;
}

std::vector<std::string> Command::getCommandName(std::string const& command)
{
    std::vector<std::string> args;
    std::size_t i = command.find(" ");
    if (i != std::string::npos)
    {
        args.push_back(command.substr(0,i));
        args.push_back(command.substr(i+1));
    }
    else
    {
        args.push_back(command);
        args.push_back("");
    }
    return args;
}

std::vector<std::string> Command::splitArguments(std::string const& arguments)
{
    std::vector<std::string> args;
    std::stringstream ss(arguments);
    std::string arg;
    while (std::getline(ss,arg,' '))
    {
        args.push_back(arg);
    }
    return args;
}

} // namespace on
