/*-------------------------------------------------------------------------
  LobbyEvents.h
  
  Lobby NT Events
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once

#define LobbyEventBaseID 5000


/*

    DO NOT EVER INSERT ITEMS IN THE LIST OR DELETE ITEMS

    ONLY ADD ITEMS TO THE END OF THE LIST

*/

enum LobbyEvent
{
  LE_RegStrMissingDef,
  LE_RegStrMissingNoDef,
  LE_RegIntMissingDef,
  LE_NoServiceHandler,
  LE_ServiceStopped,
  LE_BadServiceReq,
  LE_Started,
  LE_StartFailed,
  LE_NoAppIDKey,
  LE_NotRegistered,
  LE_Creating,
  LE_BadGameInfoSrv,
  LE_ShuttingDown,
  LE_Initializing,
  LE_HostSessionFailure,
  LE_ServerMissedRollCall,
  LE_Running,
  LE_ODBC_Error,
  LE_Assert,
  LE_BadCDKey,
  LE_CantRemovePlayer,
  LE_CorruptPlayerJoinMsg,
  LE_PlayerJoinInvalidMission,
  LE_CorruptPlayerQuitMsg,
  LE_ServerConnected,
  LE_ServerDisconnected,
  LE_ServersSessionLost,
  LE_ServerPause,
  LE_ServerContinue,
  LE_MoreTokens,
  LE_BadZTicket,
  LE_DecryptTicketFailure,
  LE_NoCredentials,
  LE_CorruptFindPlayerMsg,
  LE_UnknownMsgFromPlayer,
  LE_ClientsSessionLost,
  LE_MissionCreated,
  LE_MissionGone,
  LE_BadAutoUpdateConfig,
  LE_AutoUpdateDeactivated,
  LE_SQLInitFailed,
  LE_DatabaseError,
};

extern const char * g_rgszLobbyEvents[];