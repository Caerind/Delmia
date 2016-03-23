#ifndef PACKETTYPE_HPP_INCLUDED
#define PACKETTYPE_HPP_INCLUDED

namespace Packet
{

enum Type
{
    None,
    Message,

    Login,
    Disconnect,

    LoginSuccess,
    LoginFailed,
    ClientJoined,
    ClientLeft,
    ServerStopped,
};

} // namespace Packet

#endif // PACKETTYPE_HPP_INCLUDED
