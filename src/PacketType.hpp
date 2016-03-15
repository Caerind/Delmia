#ifndef PACKETTYPE_HPP_INCLUDED
#define PACKETTYPE_HPP_INCLUDED

namespace Packet
{

enum Type
{
    /// MainServer

    MS_None = 0,            // Nothing
    MS_Stopped,             // Tell both game servers and clients that the main server stopped

    // MainServer To Client
    MS_LoginValid,          // Tell the client the login he entered is valid
    MS_LoginFailed,         // Tell the client the login he entered isnt valid
    MS_ServerList,          // Give the client a list of game servers
    MS_ValidChoice,         // Tell the client he can go the the game server he requested

    // MainServer To GameServer
    MS_TransferClient,      // Give the game server an incoming client

    /// Client

    C_None,                 // Nothing
    C_Disconnect,           // Tell the client disconnect
    C_Login,                // Tell the client login

    // Client To MainServer
    C_RequestServerList,    // Tell the main server that the client want the new server list
    C_RequestGameServer,    // Tell the main server that the client want to join a game server

    /// GameServer
    GS_None,                // Nothing
    GS_Stopped,             // Tell others the server has stopped

    // GameServer To MainServer
    GS_Register,            // Register this game server on the main server
};

} // namespace Packet

#endif // PACKETTYPE_HPP_INCLUDED
