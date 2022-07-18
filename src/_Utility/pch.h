//////////////////////////////////////////////////////////////////////////////
//
// PCH.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PCH_H_
#define _PCH_H_

//
// C headers
//

#include "stdio.h"
#include "string.h"
#include <stdarg.h>
#include <time.h>
#include <malloc.h>
// mdvalley: Another include to shut the ATL libraries up.
#include <stddef.h>

//
// Windows headers
//

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include <urlmon.h>

#include <algorithm>

// BT - STEAM
#include "steam_gameserver.h"

//
// Wintrek headers
//
#include "guids.h"
#include "zlib.h"
#include "Utility.h"
//#include "zauth.h" // KG removed


#endif
