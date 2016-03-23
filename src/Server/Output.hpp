#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <ctime>
#include <fstream>
#include <iostream>

namespace on
{

class Output
{
    public:
        Output(bool console = false, bool file = false, std::string const& filename = "");

        void write(std::string const& message);
        void writeDebug(std::string const& message);

        void enableDebug(bool enable);
        void enableConsoleLogging(bool enable);
        void enableFileLogging(bool enable, std::string const& filename = "");

        std::string getTime() const;

    private:
        bool mDebug;
        bool mConsoleLogging;
        bool mFileLogging;
        std::ofstream mFile;
};

} // namespace on

#endif // OUTPUT_HPP
