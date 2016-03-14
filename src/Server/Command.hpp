#ifndef ON_COMMAND_HPP
#define ON_COMMAND_HPP

// Standards Libs
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace on
{

class Command
{
    public:
        typedef std::function<void(const std::string&,const std::string&,bool)> Function;

        Command();
        Command(std::string const& name, Function func, bool adminOnly = true);

        std::string getName() const;
        void execute(std::string const& args, std::string const& executant, bool isServer);
        bool isAdminOnly() const;

        static std::vector<std::string> getCommandName(std::string const& command);
        static std::vector<std::string> splitArguments(std::string const& arguments);

    protected:
        std::string mName;
        Function mFunction;
        bool mAdminOnly;
};

} // namespace on

#endif // ON_COMMAND_HPP
