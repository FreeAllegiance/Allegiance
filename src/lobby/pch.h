/*-------------------------------------------------------------------------
  pch.h
  
  Precompiled header for Lobby
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _LOBBY_PCH_
#define _LOBBY_PCH_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <mmsystem.h> // wlp 2006 added = somehow appweb.h excludes timegetTime in this module 


//#include <dplobby.h>



// This also includes <atlbase.h> and <atlcom.h>
#include "LobbyModule.h"
#include <stdio.h>

// identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)

// 'identifier' : decorated name length exceeded, name was truncated
#pragma warning(disable : 4503)

#include <map>

#define __MODULE__ "Lobby"
#include <zlib.h>
#include <utility.h>

#include "cpp-httplib/httplib.h"

#include <TCLib.h>
#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>

#include "steam_gameserver.h"

//Imago removed 7/5/08
//#ifdef USEAUTH
#include "LobQueries.h" // and allegdb.h
//#endif
#include <guids.h>
#include "resource.h"
#include "messagecore.h"
#include "messagesAll.h"
#include "messagesLS.h"
#include "messagesLC.h"
#include "sharemem.h"
#include "counters.h"
// KG - guard with USEAUTH for consistency
#ifdef USEAUTH
#include "zauth.h"
#endif
#include "broadcast.h"
#include "zgameinfo.h"
#include "LobbyApp.h"
#include "mission.h"
#include "client.h"
#include "server.h"
#include "AutoUpdate.h"
#include "LobbyEvents.h"

#include "CSteamValidation.h" // BT - STEAM


/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif
