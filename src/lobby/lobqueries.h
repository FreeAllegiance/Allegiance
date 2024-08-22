/*-------------------------------------------------------------------------
  LobQueries.h
  
  Database queries for the lobby
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once

#include "allegdb.h"

// Stuff we need to logon
BEGIN_QUERY(CQLobbyLogon, true, 
  TEXT("{call GetLobbyLogonInfo(?, ?, ?, ?, ?)}"))

  CQLobbyLogonData() :
    szReason(NULL)
  {}

  ~CQLobbyLogonData()
  {
    delete [] szReason;
  }

  // Stuff to remember when precessing results
  DWORD dwConnectionID;                 // Remember who this is using a SAFE mechanism
  bool  fValid : 1;                         // If false we just go straight to the handler
  bool  fRetry : 1;                         // Remember whether the client should retry the logon
  char * szReason;
  DWORD dTime;

  // query parameters
  char  szCDKey[1024]; // Imago changed
  char  szCharacterName[c_cbName];
  int   characterID;
  char  fValidCode; // out
  char  fCanCheat;  // out

  // BT - STEAM
#ifndef NO_STEAM
  int8	steamAuthTicket[1024];
  int32 steamAuthTicketLength;
  uint64 steamID;
#endif

  //squad stuff
  char szSquadName[31]; // constant??? Please???
  int  status;
  int  squadID;
  int  detailedStatus;
  
  BEGIN_PARAM_MAP(CQLobbyLogonData)
    COLUMN_ENTRY_TYPE(1,  DBTYPE_STR, szCharacterName)
    COLUMN_ENTRY_TYPE(2,  DBTYPE_I4,  characterID)
    COLUMN_ENTRY_TYPE(3,  DBTYPE_STR, szCDKey)
    SET_PARAM_TYPE(DBPARAMIO_OUTPUT)
    COLUMN_ENTRY_TYPE(4,  DBTYPE_I1,  fValidCode)
    COLUMN_ENTRY_TYPE(5,  DBTYPE_I1,  fCanCheat)
  END_PARAM_MAP()

  BEGIN_COLUMN_MAP(CQLobbyLogonData)
    COLUMN_ENTRY_TYPE(1, DBTYPE_STR,  szSquadName)
    COLUMN_ENTRY_TYPE(2, DBTYPE_I4,   status)
    COLUMN_ENTRY_TYPE(3, DBTYPE_I4,   squadID)
    COLUMN_ENTRY_TYPE(4, DBTYPE_I4,   detailedStatus)
  END_COLUMN_MAP()

END_QUERY(CQLobbyLogon, true)