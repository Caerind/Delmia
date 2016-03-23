#include "Connection.hpp"

namespace on
{

sf::Uint32 Connection::mNumberOfCreations = 0;

Connection::Connection()
: mConnected(false)
, mReceiveThread(&Connection::receive,this)
, mSendThread(&Connection::send,this)
, mId(++mNumberOfCreations)
, mTimeout(sf::seconds(10.f))
{
}

Connection::~Connection()
{
    disconnect();
}

bool Connection::isConnected() const
{
    return mConnected;
}

bool Connection::connect()
{
    mConnected = true;

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

bool Connection::poll(sf::Packet& packet)
{
    if (mIncoming.size() > 0)
    {
        sf::Lock lock(mReceiveMutex);
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

    sf::Clock rClock;
    while (mConnected)
    {
        if (rClock.getElapsedTime() >= mTimeout)
        {
            sf::Lock lock(mReceiveMutex);
            mConnected = false;
        }

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
            rClock.restart();
        }
    }
}

void Connection::send()
{
    sf::Clock sClock;
    while (mConnected)
    {
        if (sClock.getElapsedTime() >= mTimeout)
        {
            sf::Lock lock(mSendMutex);
            mConnected = false;
        }

        if (mOutgoing.size() > 0)
        {
            mSendMutex.lock();
            sf::Packet packet = mOutgoing.front();
            mOutgoing.pop();
            mSendMutex.unlock();
            if (mSocketOut.send(packet) == sf::Socket::Done)
            {
                sClock.restart();
            }
        }

        if (sClock.getElapsedTime() > 0.5f * mTimeout)
        {
            sf::Packet packet;
            if (mSocketOut.send(packet) == sf::Socket::Done)
            {
                sClock.restart();
            }
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
