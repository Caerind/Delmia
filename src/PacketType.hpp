#ifndef PACKETTYPE_HPP_INCLUDED
#define PACKETTYPE_HPP_INCLUDED

namespace Packet
{

enum Type
{
    None = 0, // Used for update

    // MainServer To Client
    MS_ClientJoined,
    MS_ClientLeft,
    MS_ServerStopped,
    MS_ServerMessage,
    MS_Banned,
    MS_Kicked,

    // Client To MainServer
    MS_Login,
    MS_Disconnect,
    MS_ClientMessage,

    // GameServer To Client
    GS_ClientJoined,
    GS_ClientLeft,
    GS_ServerStopped,
    GS_ServerMessage,
    GS_Banned,
    GS_Kicked,

    // Client To GameServer
    GS_Login,
    GS_Disconnect,
    GS_ClientMessage,

    // GameServer To MainServer
    GS_Register_MS,


    // MainServer To GameServer
    MS_TransferUser_GS,
};

} // namespace Packet

#endif // PACKETTYPE_HPP_INCLUDED
