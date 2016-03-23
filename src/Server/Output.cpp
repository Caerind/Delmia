#include "Output.hpp"

namespace on
{

Output::Output(bool console, bool file, std::string const& filename)
: mDebug(false)
, mConsoleLogging(console)
, mFileLogging(file)
, mFile(filename)
{
}

void Output::write(std::string const& message)
{
    std::string m = getTime() + message;
    if (mFileLogging)
    {
        if (mFile.is_open())
        {
            mFile << m << std::endl;
        }
    }
    if (mConsoleLogging)
    {
        std::cout << m << std::endl;
    }
}

void Output::writeDebug(std::string const& message)
{
    if (mDebug)
    {
        write(message);
    }
}

void Output::enableDebug(bool enable)
{
    mDebug = enable;
}

void Output::enableConsoleLogging(bool enable)
{
    mConsoleLogging = enable;
}

void Output::enableFileLogging(bool enable, std::string const& filename)
{
    mFileLogging = enable;
    mFile.open(filename);
}

std::string Output::getTime() const
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[40];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,40,"[%x][%X] ",timeinfo);
    return std::string(buffer);
}

} // namespace on
