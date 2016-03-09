#ifndef NLOG_HPP
#define NLOG_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Network/Http.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Time.hpp"
#include "HttpThread.hpp"

class NLog
{
    public:
		static NLog& instance();

        enum LogType
        {
            Error,
            Warning,
            Info,
        };

        static void log(std::string const& message);
		static void onlineLog(std::string const& message);

        friend NLog& operator<<(NLog& log, std::string const& message)
        {
            NLog::log(message);
            return log;
        }

        friend NLog& operator<<(NLog& log, LogType type)
        {
			NLog::setLogType(type);
            return log;
        }

        static void setLogType(LogType type);
		static LogType getLogType();

        static bool openLog(std::string const& filename);
        static bool isLogOpen();

        static void useConsole(bool use);
		static bool usingConsole();

		static void setOnline(std::string const& url, std::string const& uri, std::string const& appname = "", std::string const& appversion = "", std::string const& username = "");
		static bool isOnline();

	protected:
        NLog();
		~NLog();

		static NLog* mInstance;

    protected:
        LogType mType;
        std::ofstream mFile;
        bool mConsole;
		bool mOnline;

		lp::HttpThread mThread;

        std::string mAppName;
        std::string mAppVersion;
        std::string mUsername;
};

#endif // NLOG_HPP
