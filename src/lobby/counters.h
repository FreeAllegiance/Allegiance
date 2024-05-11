#pragma once

typedef struct _LOBBY_COUNTERS
{
    DWORD cLogins;
    DWORD cLogoffs;
    DWORD cPlayersMissions;
    DWORD cPlayersLobby;
    DWORD cPlayersDropped;
    DWORD cInboundQueueLength;
    DWORD cInboundQueueSize;
    DWORD cOutboundQueueLength;
    DWORD cOutboundQueueSize;
    DWORD timeInnerLoop;
    DWORD cMissions;
    DWORD cServers;
} LOBBY_COUNTERS, *PLOBBY_COUNTERS;

typedef struct _PER_SERVER_COUNTERS
{
    DWORD cPlayers;
    DWORD cMissions;
    DWORD percentLoad; // based on how many players they said they can handle
    DWORD dummy; // 8 byte alignment
} PER_SERVER_COUNTERS, *PPER_SERVER_COUNTERS;