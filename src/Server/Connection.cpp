#include "Connection.hpp"

namespace on
{

sf::Uint32 Connection::mNumberOfCreations = 0;

Connection::Connection()
: mConnected(false)
, mReceiveThread(&Connection::receive,this)
, mSendThread(&Connection::send,this)
, mId(++mNumberOfCreations)
, mTimeout(sf::seconds(5.f))
{
}

Connection::~Connection()
{
    disconnect();
}

bool Connection::poll(sf::Packet& packet)
{
    sf::Lock lock(mReceiveMutex);
    if (mIncoming.size() > 0)
    {
        std::swap(packet,mIncoming.front());
        mIncoming.pop();
        return true;
    }
    return false;
}

void Connection::send(sf::Packet& packet)
{
    sf::Lock lock(mSendMutex);
    mOutgoing.emplace(packet);
}

bool Connection::isConnected() const
{
    return mConnected;
}

bool Connection::connect()
{
    mConnected = true;

    mReceiveClock.restart();
    mSendClock.restart();

    mReceiveThread.launch();
    mSendThread.launch();

    return mConnected;
}

void Connection::disconnect()
{
    mConnected = false;

    // Stop threads
    mReceiveThread.wait();
    mSendThread.wait();

    // Send last packets
    while (mOutgoing.size() > 0)
    {
        mSocketOut.send(mOutgoing.front());
        mOutgoing.pop();
    }

    // Stop sockets
    mSocketIn.disconnect();
    mSocketOut.disconnect();
}

sf::Uint32 Connection::getId() const
{
    return mId;
}

sf::TcpSocket& Connection::getSocketIn()
{
    return mSocketIn;
}

sf::TcpSocket& Connection::getSocketOut()
{
    return mSocketOut;
}

void Connection::receive()
{
    sf::SocketSelector selector;
    selector.add(mSocketIn);

    while (mConnected)
    {
        if (!selector.wait(sf::seconds(1.f)) || !selector.isReady(mSocketIn))
        {
            continue;
        }

        sf::Packet packet;
        if (mSocketIn.receive(packet) == sf::Socket::Done)
        {
            sf::Lock lock(mReceiveMutex);
            if (!packet.endOfPacket())
            {
                mIncoming.emplace(std::move(packet));
            }
            mReceiveClock.restart();
        }

        if (mReceiveClock.getElapsedTime() >= mTimeout)
        {
            sf::Lock lock(mReceiveMutex);
            mConnected = false;
        }
    }
}

void Connection::send()
{
    while (mConnected)
    {
        if (mOutgoing.size() > 0)
        {
            mSendMutex.lock();
            sf::Packet packet = mOutgoing.front();
            mOutgoing.pop();
            mSendMutex.unlock();
            if (mSocketOut.send(packet) == sf::Socket::Done)
            {
                sf::Lock lock(mSendMutex);
                mSendClock.restart();
            }
        }

        if (mSendClock.getElapsedTime() > sf::seconds(1.f))
        {
            sf::Packet packet;
            if (mSocketOut.send(packet) == sf::Socket::Done)
            {
                sf::Lock lock(mSendMutex);
                mSendClock.restart();
            }
        }

        if (mSendClock.getElapsedTime() >= mTimeout)
        {
            sf::Lock lock(mSendMutex);
            mConnected = false;
        }
    }
}

void Connection::setTimeout(sf::Time const& timeout)
{
    mTimeout = timeout;
}

sf::Time Connection::getTimeout() const
{
    return mTimeout;
}

} // namespace on
