#pragma once

/////////////////////////////////////////////////////////////////////////////
// CoInit.h : Declaration and implementation of the TCCoInit class.
//

#include <TCLib.h>


/////////////////////////////////////////////////////////////////////////////
// Description: Simplifies the matching of calls to
// CoInitialize/CoInitializeEx and CoUninitialize.
//
class TCCoInit
{
// Construction / Destruction
public:
  #if _WIN32_WINNT >= 0x0400 || defined(_WIN32_DCOM)
    TCCoInit(COINIT coInit = COINIT_APARTMENTTHREADED) :
      m_hr(CoInitializeEx(NULL, coInit))  {}
  #else
    TCCoInit() : m_hr(CoInitialize(NULL)) {}
  #endif
  ~TCCoInit()
  {
    Uninit();
  }

// Attributes
public:
  bool Succeeded() const
  {
    return SUCCEEDED(m_hr);
  }
  bool Failed() const
  {
    return FAILED(m_hr);
  }

// Operations
public:
  void Uninit()
  {
    if (SUCCEEDED(m_hr))
    {
      m_hr = E_FAIL;
      CoUninitialize();
    }
  }

// Operators
public:
  operator HRESULT() const
  {
    return m_hr;
  }

// Data Members
protected:
  HRESULT m_hr;
};


/////////////////////////////////////////////////////////////////////////////