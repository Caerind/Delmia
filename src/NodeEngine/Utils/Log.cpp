#include "Log.hpp"

NLog* NLog::mInstance = nullptr;

NLog& NLog::instance()
{
	if (mInstance == nullptr)
	{
		mInstance = new NLog();
	}
	return *mInstance;
}

void NLog::log(std::string const& message)
{
    std::stringstream ss;
	ss << NTime::getTime("[%x][%X]");
	switch (instance().mType)
	{
		case NLog::LogType::Error:
			ss << "[ERROR] : ";
			break;

		case NLog::LogType::Warning:
			ss << "[WARNING] : ";
			break;

		default:
			ss << "[INFO] : ";
	}
	ss << message;

	if (instance().mConsole)
	{
		std::cout << ss.str() << std::endl;
	}

	if (instance().mFile.is_open())
	{
		instance().mFile << ss.str() << std::endl;
	}

	if (instance().mOnline)
	{
		NLog::onlineLog(message);
	}

	instance().mType = NLog::LogType::Info;
}

void NLog::onlineLog(std::string const& message)
{
	std::stringstream stream;
	stream << "action=insert";

	if (instance().mType == LogType::Info)
		stream << "&type=info";
	else if (instance().mType == LogType::Warning)
		stream << "&type=warning";
	else if (instance().mType == LogType::Error)
		stream << "&type=error";
	else
		stream << "&type=none";

	if (instance().mAppName != "")
		stream << "&appname=" << instance().mAppName;

	if (instance().mAppVersion != "")
		stream << "&appversion=" << instance().mAppVersion;

	stream << "&address=" << sf::IpAddress::getPublicAddress().toString();

	if (instance().mUsername != "")
		stream << "&username=" << instance().mUsername;

	stream << "&content=" << message;

	instance().mThread.setMessage(stream.str());
}

void NLog::setLogType(LogType type)
{
    instance().mType = type;
}

bool NLog::openLog(std::string const& filename)
{
    instance().mFile.open(filename,std::ios::app);
    return instance().mFile.is_open();
}

bool NLog::isLogOpen()
{
    return instance().mFile.is_open();
}

void NLog::useConsole(bool use)
{
    instance().mConsole = use;
}

bool NLog::usingConsole()
{
	return instance().mConsole;
}

void NLog::setOnline(std::string const& url, std::string const& uri, std::string const& appname, std::string const& appversion, std::string const& username)
{
	instance().mAppName = (appname != "") ? appname : "App";
	instance().mAppVersion = (appversion != "") ? appversion : "v0.1";
	instance().mUsername = (username != "") ? username : "Unknown";

	instance().mThread.setAddress(url,uri);

	instance().mOnline = true;
}

bool NLog::isOnline()
{
	return instance().mOnline;
}

NLog::NLog() : mType(NLog::LogType::Info), mFile(), mConsole(true), mOnline(false)
{
}

NLog::~NLog()
{
}
