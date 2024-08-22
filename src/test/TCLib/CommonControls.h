#pragma once

/////////////////////////////////////////////////////////////////////////////
// CommonControls.h | Declaration of the TCCommonControls class.

#include "DynaLib.h"
#include <commctrl.h>


/////////////////////////////////////////////////////////////////////////////
// TCCommonControls provides a run-time interface to the Win32 Common Control
// Library.
//
// The Common Control Library underwent changes (around the time of IE3 and
// NT4-SP3) that greatly enhanced the functionality and scope of the provided
// controls.  However, to ensure that the newer version of the controls are
// installed on a machine, it is best to dynamically load the "COTCTL32.dll"
// library and check for the existence of the InitCommonControlsEx export. If
// this API export does *not* exist, the application should refrain from
// using features of the Common Controls Library that are not available in
// the earlier versions.
class TCCommonControls : public TCDynaLib
{
// Construction
public:
  TCCommonControls();

// Operations
public:
  BOOL InitEx(LPINITCOMMONCONTROLSEX pInitCtrls);

// Group=Library Functions
public:
  TCDynaLib_Fn0_void(InitCommonControls);
  TCDynaLib_Fn1(BOOL, InitCommonControlsEx, LPINITCOMMONCONTROLSEX);
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// Constructs the object by calling the base class constructor, specifying
// "comctl32.dll" as the library to be loaded into this instance.
//
// See Also: TCDynaLib, TCDynaLib::TCDynaLib, TCDynaLib::LoadLibrary
inline TCCommonControls::TCCommonControls()
  : TCDynaLib(TEXT("comctl32.dll"))
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Initializes the Common Controls Library.
//
// Provides a method of intitializing the Common Controls Library that is
// compatible with earlier and later versions of the library.
//
// Since the *InitCommonControlsEx* API is only available with later versions
// of the library, this method should be called instead. It takes the same
// parameter, /pInitCtlrs/, as the *InitCommonControlsEx* API, but will
// ignore the parameter if an earlier version of the library is installed.
//
// Parameters:
//   pInitCtrls - A pointer to an INITCOMMONCONTROLSEX structure which
//   specifies which controls are to be initialized. If an earlier version of
//   the library is installed, this paramter is ignored and *all* controls
//   are initialized by calling the *InitCommonControls* API.
inline BOOL TCCommonControls::InitEx(LPINITCOMMONCONTROLSEX pInitCtrls)
{
  if (Exists_InitCommonControlsEx())
    return InitCommonControlsEx(pInitCtrls);
  InitCommonControls();
  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Library Functions
#ifdef _DOCJET_ONLY

  ///////////////////////////////////////////////////////////////////////////
  // Description: Initializes the Common Controls Library.
  //
  // Registers and initializes the common control window classes. This
  // function is obsolete. New applications should use InitCommonControlsEx.
  //
  // See Also: TCDynaLib_Fn Macros
  void TCCommonControls::InitCommonControls()
  {
  }

  ///////////////////////////////////////////////////////////////////////////
  // Description: Initializes the Common Controls Library as specified.
  //
  // Initializes the Common Controls Library as specified in the
  // /pInitCtrls/ parameter.
  //
  // Since this API is only available in the later versions of the Common
  // Controls Library, an application should not call it directly but,
  // instead, should use the InitEx method.
  //
  // Return Value: TRUE if the function succeeded, otherwise FALSE.
  //
  // Parameters:
  //   pInitCtrls - A pointer to an INITCOMMONCONTROLSEX structure which
  //   specifies which controls are to be initialized.
  //
  // See Also: TCDynaLib_Fn Macros
  BOOL TCCommonControls::InitCommonControlsEx(
    LPINITCOMMONCONTROLSEX pInitCtrls)
  {
  }
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////