/////////////////////////////////////////////////////////////////////////////
// DynaLib.cpp | Implementation of the TCDynaLib class used to wrap the
// loading of DLL's and resolving a set of API's that may or may not exist
// at runtime.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "DynaLib.h"


/////////////////////////////////////////////////////////////////////////////
// TCDynaLib


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// When using the default constructor, the LoadLibrary member function must
// be used to load a DLL.
TCDynaLib::TCDynaLib()
  : m_hinst(NULL)
{
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   pszLibName - The name of the DLL to be loaded into this instance.
//
// Remarks:
// When using the second form of the constructor, use the IsLibraryLoaded
// method to determine if the library was successfully loaded.
TCDynaLib::TCDynaLib(LPCTSTR pszLibName)
  : m_hinst(NULL)
{
  // Attempt to load the specified library
  LoadLibrary(pszLibName);
}

/////////////////////////////////////////////////////////////////////////////
// If a DLL is loaded into this instance, it is freed.
TCDynaLib::~TCDynaLib()
{
  CLock lock(this);
  if (NULL != m_hinst)
    FreeLibrary(m_hinst);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Determines if a library is loaded.
//
// Return Value: true if a DLL is loaded into this instance, otherwise false.
bool TCDynaLib::IsLibraryLoaded()
{
  CLock lock(this);
  return NULL != m_hinst;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines if a library is loaded and a set of API exports
// have been resolved.
//
// Return Value: true if a DLL is loaded into this instance and all required
// API exports can be resolved, otherwise false.
//
// Remarks: A derived class can override this method using the following set
// of macros:
//  + TCDynaLib_BEGIN_REQUIRED_MAP
//  + TCDynaLib_REQUIRED_FUNCTION
//  + TCDynaLib_END_REQUIRED_MAP
bool TCDynaLib::IsLibraryLoadedAndResolved()
{
  return IsLibraryLoaded();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Loads the specified dynamic link library (DLL).
//
// Parameters:
//   pszLibName - The name of the DLL to be loaded into this instance.
//
// Return Value: true if the DLL was successfully loaded into this instance.
// false if an error occurred.
// 
// Remarks: Each instance can be associated with only one DLL.  Once
// LoadLibrary has been called, either directly or indirectly (using the
// second form of the constuctor), subsequent attempts to call this method
// will fail.
bool TCDynaLib::LoadLibrary(LPCTSTR pszLibName)
{
  bool bResult = false;
  __try
  {
    // Lock the object
    Lock();

    // LoadLibrary can only be successfully called once
    ZAssert(!m_hinst);

    // Load the specified library and indicate whether it was loaded or not
    bResult = NULL != (m_hinst = ::LoadLibrary(pszLibName));
  }
  __except(1)
  {
    bResult = false;
  }

  // Unlock the object
  Unlock();

  // Return the last result
  return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Attempts to resolve the specified API export name.
//
// Parameters:
//   pszFn - The API export name to be resolved. This parameter is
//     intentionally an ANSI string because the Win32 ::GetProcAddress API
//     does not accept Unicode strings.
//   chDecoration - The calling convention used to decorate the exported
// name. See the remarks for possible values.
//   cbArgs - The number of bytes of arguments passed to the export. This is
// only used when the __stdcall or __fastcall calling convention decoration
// is specified 's' or 'f'.
//
// Return Value: The function pointer of the resolved export if successful,
// otherwise NULL.
//
// Remarks: This method will always fail if no DLL has been loaded.
//
// The possible values for the /chDecoration/ parameter are as follows:
//   'c' - Decorated according to the __cdecl calling convention.
//   's' - Decorated according to the __stdcall calling convention.
//   'f' - Decorated according to the __fastcall calling convention.
//
// Typically, this function is not called directly, but rather, used by the
// TCDynaLib_Fn macros.
//
// See Also: TCDynaLib::LoadLibrary, TCDynaLib_Fn macros
FARPROC TCDynaLib::GetProcAddress(LPCSTR pszFn, char chDecoration, int cbArgs)
{
  // Apply the specified name decoration
  char* szDecorated = (char*)_alloca(strlen(pszFn) + 8);
  switch (tolower(chDecoration))
  {
    case 'c':
    {
      sprintf(szDecorated, "_%s", pszFn);
      break;
    }
    case 's':
    {
      sprintf(szDecorated, "_%s@%d", pszFn, cbArgs);
      break;
    }
    case 'f':
    {
      sprintf(szDecorated, "@%s@%d", pszFn, cbArgs);
      break;
    }
    default:
      strcpy(szDecorated, pszFn);
  }

  // Resolve the address of the specified function name
  return GetProcAddress(szDecorated);
}

FARPROC TCDynaLib::GetProcAddress(LPCSTR pszFn)
{
  // Get the library instance handle
  HINSTANCE hinst;
  {
    CLock lock(this);
    hinst = m_hinst;
  }

  // Can't resolve if library couldn't be loaded
  if (NULL == hinst)
    return NULL;

  // Resolve the address of the specified function name
  return ::GetProcAddress(m_hinst, pszFn);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// {Secret}
// Description: Called internally when an unresolved API is invoked and
// cannot be resolved in the loaded DLL.
void TCDynaLib::FunctionNotFound(LPCSTR pszFn)
{
  LPCSTR pszFmt = "TCDynaLib: Function \"%s\" could not be resolved\n";
  LPSTR pszMsg = LPSTR(_alloca(strlen(pszFmt) + strlen(pszFn)));
  sprintf(pszMsg, pszFmt, pszFn);
  OutputDebugStringA(pszMsg);
  ZAssert(pszMsg);
}

