#pragma once

typedef struct _SERVER_COUNTERS
{
    DWORD cLoginAttempts;           
    DWORD cLogins;                  
    DWORD cLoginsFailed;            
    DWORD cLogoffs;                 
    DWORD cTimeouts;
    DWORD cTimeoutsPerSecond;
    DWORD cRelogins;                
    DWORD cPacketsIn;               
    DWORD cPacketsInPerSecond;
    DWORD cPacketsOut;
    DWORD cPacketsOutPerSecond;
    DWORD cBytesIn;
    DWORD cBytesInPerSecond;
    DWORD cBytesOut;
    DWORD cBytesOutPerSecond;
    DWORD cPlayersOnline;
    DWORD csecAvgUpdateLatency;
    DWORD csecMaxUpdateLatency;
    DWORD cSysMessagesIn;
    DWORD cSysMessagesInPerSecond;
    DWORD cInboundQueueLength;
    DWORD cInboundQueueSize;
    DWORD cOutboundQueueLength;
    DWORD cOutboundQueueSize;
    DWORD timeIGCWork;
    DWORD timeBetweenInnerLoops;
    DWORD timeNetworkMessages;
    DWORD timeMsgPump;
    DWORD timeCycleTarget;
} SERVER_COUNTERS, *PSERVER_COUNTERS;