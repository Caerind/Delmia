#include "HttpThread.hpp"

namespace lp
{

HttpThread::HttpThread()
: mThread(std::bind(&HttpThread::run,this))
{
    mRunning = true;
    mThread.launch();
}

HttpThread::HttpThread(std::string const& url, std::string const& uri)
: mThread(std::bind(&HttpThread::run,this))
, mUrl(url)
, mUri(uri)
{
    mRunning = true;
    mThread.launch();
}

HttpThread::~HttpThread()
{
    mRunning = false;
    mThread.wait();
}

void HttpThread::run()
{
    while (mRunning)
    {
        while (!mMessages.empty())
        {
            sf::Http http(mUrl);
            sf::Http::Request request(mUri, sf::Http::Request::Post);
            request.setBody(mMessages.back());

            mMutex.lock();
            mMessages.pop_back();
            mMutex.unlock();

            http.sendRequest(request);
        }
    }
}

void HttpThread::setAddress(std::string const& url, std::string const& uri)
{
    mMutex.lock();
    mUrl = url;
    mUri = uri;
    mMutex.unlock();
}

void HttpThread::setMessage(std::string const& message)
{
    mMutex.lock();
    mMessages.push_back(message);
    mMutex.unlock();
}

} // namespace lp
