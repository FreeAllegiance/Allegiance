/*-------------------------------------------------------------------------
 * pch.h
 * 
 * Headers for fedsrv
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _FEDSRV_PCH_
#define _FEDSRV_PCH_

#define WIN32_LEAN_AND_MEAN

//KG- fix VS2015 issues
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#define _ATL_STATIC_REGISTRY

// browse info overflow warnings for STL objects
#pragma warning(disable : 4786)

// Standard C Includes
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
// Standard C++ Includes
#include <algorithm>
#include <map>
#include <set>
#include <vector>

// Windows Includes
#include <windows.h>
#include <ShellAPI.h>
#include <commctrl.h>
#include <sqlext.h>
//#include <dplobby.h>
#include <mmsystem.h> 

// ZLib Includes
#if !defined(ALLSRV_STANDALONE)
  #define __MODULE__ "AllSrv"
#else // !defined(ALLSRV_STANDALONE)
  #define __MODULE__ "AllSrv"
#endif // !defined(ALLSRV_STANDALONE)
#include <guids.h>
#include <zlib.h>

#include "cpp-httplib/httplib.h"

#include "json.hpp"

#include "resource.h"
#include <utility.h>
#include <igc.h>

// BT - STEAM
#include "steam_gameserver.h"
#include "AllegianceSecurity.h"
#include "CSteamAchievements.h"

#include "..\AGC\GameParamData.h"

//#include <atlbase.h>
//extern CServiceModule _Module;
//#include <atlcom.h>

// KG- removed
//#include <ZoneAuthDecrypt.h>

// This also includes <atlbase.h> and <atlcom.h>
#include "AllSrvModule.h"

#include <TCLib.h>
#include <..\TCLib\AutoHandle.h>

#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>


#include "Zone.h"

#include <messagesAll.h>
#include <Messages.h>
#include <MessagesLS.h>

//#include "SWMRG.h" Imago removed (again)
#include "BitArray.h"
#include "point.h"
//#include "srvdbg.h"

#include "sharemem.h"
//#include "config.h"
#include "counters.h"
#include "srvqueries.h" // and allegdb.h

#include "AllSrvModuleIDL.h" 
#include "AdminUtil.h" 
#include "fsside.h"
#include "fscluster.h"
#include "fsmission.h"
#include "fedsrvsql.h" //KG removed - Imago unremoved 6/10

// KG guard with USEAUTH for consistency 
#ifdef USEAUTH
#include "zauth.h" 
#endif

#include "fedsrv.h"
extern Global g;
#include "fsship.h"

#include "AdminGame.h"
#include "AdminGames.h"
#include "AdminSession.h"


#include "AdminUser.h"
#include "AdminUsers.h"
#include "AdminServer.h"
#include "AdminShip.h"

#include "dbgutil.h"


#include "regkey.h"

// BT - STEAM
#include "CSteamValidation.h"
#include "CSteamUserGroupStatus.h"


/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif // _FEDSRV_PCH_

