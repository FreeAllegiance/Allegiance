//////////////////////////////////////////////////////////////////////////////
//
// PCH.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PCH_H_
#define _PCH_H_

// BT - STEAM
#include "steam_api.h"

// C4786: BSC name truncation
#pragma warning(disable:4786)

//
// C Headers
//

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

//
// C++ Headers
//

#include <list>
#include <algorithm>

//
// Windows headers
//

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <commdlg.h>
#include <commctrl.h>

// Need to include zlib before any DirectX headers

#define ID_APP_ICON "winTrekIcon"
#include "zlib.h"

//#include <dplobby.h>

//
// WinTrek headers
//

#include "engine.h"
#include "effect.h"
#include "utility.h"
#include "Zone.h"
//#include "zauth.h" KG removed
#include "igc.h"
#include "UiEngine.h"

#include "trekinput.h"
#include "resource.h"

#include "messagesAll.h"
#include "messages.h"
#include "messagesLC.h"
#include "clintlib.h"

#include "soundengine.h"
#include "mission.h"

#include "screen.h"
#include "wintrek.h"
#include "indicator.h"
#include "radarimage.h"
#include "trekctrls.h"
#include "trekmdl.h"
#include "loadout.h"

#include "wintrekp.h"
#include "artwork.h"
#include "logon.h"
#include "help.h"

#define and     &&
#define or      ||
#define bit_and &
#define bit_or  |
#define not     !

#endif
