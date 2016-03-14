#include "Message.hpp"

namespace on
{

Message::Message()
{
    setActualTime();
}

Message::Message(std::string const& emitter, std::string const& content)
: mEmitter(emitter)
, mContent(content)
{
    setActualTime();
}

std::string Message::getEmitter() const
{
    return mEmitter;
}

void Message::setEmitter(std::string const& emitter)
{
    mEmitter = emitter;
}

std::string Message::getContent() const
{
    return mContent;
}

void Message::setContent(std::string const& content)
{
    mContent = content;
}

bool Message::isCommand() const
{
    return mContent.front() == '/';
}

sf::Int64 Message::getTime() const
{
    return mTime;
}

void Message::setTime(sf::Int64 time)
{
    mTime = time;
}

void Message::setActualTime()
{
    mTime = static_cast<sf::Int64>(time(nullptr));
}

std::ostream& operator <<(std::ostream& stream, const Message& message)
{
    if (message.getEmitter() != "")
    {
        stream << message.getEmitter() << " : " << message.getContent();
    }
    else
    {
        stream << message.getContent();
    }
    return stream;
}

sf::Packet& operator <<(sf::Packet& packet, const Message& msg)
{
    return packet << msg.getEmitter() << msg.getContent() << msg.getTime();
}

sf::Packet& operator >>(sf::Packet& packet, Message& msg)
{
    std::string e,c;
    sf::Int64 t;
    packet >> e >> c >> t;
    msg.setEmitter(e);
    msg.setContent(c);
    msg.setTime(t);
    return packet;
}

} // namespace on
