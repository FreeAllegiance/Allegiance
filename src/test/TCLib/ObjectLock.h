#pragma once

/////////////////////////////////////////////////////////////////////////////
// ObjectLock.h | Declaration of the TCObjectLock template class.
//

#include "AutoHandle.h"
#include "..\Inc\TCLib.h"
#include <atlconv.h>


/////////////////////////////////////////////////////////////////////////////
// Global Diagnostic Function

#ifdef _DEBUG

  #ifdef _ENABLE_TRACE_LOCKS
    #pragma warning(disable: 4786)
    #include <map>

    inline bool TCObjectLock_EnableTraceLocks(const char* pszClassName)
    {
      // This is not currenlty being used, due to the _ENABLE_TRACE_LOCKS
      // directive not being defined, but it really appears to be quite
      // thread *UNSAFE*, since this map object does not have any explicit
      // thread synchronization.
      typedef std::map<const char*, bool> CClassNameMap;
      static CClassNameMap mapClassNames;

      // First lookup the class name in the map
      CClassNameMap::iterator it = mapClassNames.find(pszClassName);
      if (mapClassNames.end() != it)
        return (*it).second;

      // Open the debugging subkey
      bool bEnable = false;
      const LPCTSTR pszKeyName =
        TEXT("Software\\Microsoft\\Microsoft Games\\Debug\\EnableTraceLocks");
      TCRegKeyHandle hkey;
      LONG lResult =
        RegOpenKeyEx(HKEY_LOCAL_MACHINE, pszKeyName, 0, KEY_READ, &hkey);
      if (ERROR_SUCCESS == lResult)
      {
        // Attempt to read the value with the specified class name
        DWORD dwTraceLocks = 0;
        DWORD dwType = REG_DWORD;
        DWORD dwDataSize = sizeof(dwTraceLocks);
        USES_CONVERSION;
        lResult = RegQueryValueEx(hkey, A2CT(pszClassName), NULL, &dwType,
          LPBYTE(&dwTraceLocks), &dwDataSize);
        if (ERROR_SUCCESS == lResult && REG_DWORD == dwType)
          bEnable = dwTraceLocks != 0;
      }

      // Add the class name and the enabled flag to the map
      mapClassNames.insert(CClassNameMap::value_type(pszClassName, bEnable));
      return bEnable;               
    }
  #else // _ENABLE_TRACE_LOCKS
    inline bool TCObjectLock_EnableTraceLocks(const char* pszClassName)
    {
      return false;
    }
  #endif // _ENABLE_TRACE_LOCKS

#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// TCObjectLock provides a convenient mechanism for the locking and unlocking
// of protected resources.
//
// The TCObjectLock template class can be used for any object, /T/, that has
// the public members, Lock and Unlock, each without a parameter list.
//
// This object is constructed with a pointer to the attached object, /T/, and
// the Lock method of that object is called. When this object goes out of
// scope, the Unlock method of the attached object is automatically called.
// Alternately, the Unlock method of this object will unlock the attached
// object prior to this object going out of scope.
//
// The following is an example usage of this class:
//
//       DWORD CMyObject::GetValue() const
//       {
//         TCObjectLock<CMyObject> lock(this);
//         return m_dwValue;
//       }  // Unlock is called automatically when lock goes out of scope
//
// Note that this example assumes that the CMyObject class provides the
// public member functions, Lock and Unlock.
//
// It is common for a lockable object to declare a typedef for TCObjectLock.
// For example, if the CMyObject declaration provides the following typedef:
//
//       typedef TCObjectLock<CMyObject> XLock;
//
// then, the previous examples could be changed to the following, using the
// more abbreviated notation:
//
//       DWORD CMyObject::GetValue() const
//       {
//         XLock lock(this);
//         return m_dwValue;
//       }  // Unlock is called automatically when lock goes out of scope
//
// Parameters:
//   T - Any type that publicly exposes both a Lock and an Unlock method,
//   both of which do not take any parameters.
template <class T>
class TCObjectLock
{
// Construction / Destruction
public:
  TCObjectLock(T* pObject);
  ~TCObjectLock();

// Operations
public:
  void Unlock();

// Data Members:
protected:
  T* m_pObject;  // A pointer to the lockable class, /T/
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Locks the object, /T/, by calling the Lock method of class /T/.
template <class T>
inline TCObjectLock<T>::TCObjectLock(T* pObject)
  : m_pObject(pObject)
{
  if (m_pObject)
    m_pObject->Lock();

  #ifdef _ENABLE_TRACE_LOCKS
    #ifdef _DEBUG
      const char* pszClass = TCTypeName(T);
      if (TCObjectLock_EnableTraceLocks(pszClass))
      {
        DWORD dwID = GetCurrentThreadId();
        _TRACE3("TCObjectLock<%hs> on thread 0x%0X (%d): Locking\n",
          pszClass, dwID, dwID);
      }
    #endif // _DEBUG
  #endif // _ENABLE_TRACE_LOCKS
}

/////////////////////////////////////////////////////////////////////////////
// Unlocks the object, /T/, if Unlock has not already been called.
template <class T>
inline TCObjectLock<T>::~TCObjectLock()
{
  if (m_pObject)
    Unlock();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Unlocks the object, /T/, by calling the Unlock method of class /T/.
//
// After unlocking the object, the m_pObject data member is set to NULL so
// that the destructor does not attempt to unlock it.
//
// See Also: TCThread::destructor
template <class T>
inline void TCObjectLock<T>::Unlock()
{
  #ifdef _ENABLE_TRACE_LOCKS
    #ifdef _DEBUG
      const char* pszClass = TCTypeName(T);
      if (TCObjectLock_EnableTraceLocks(pszClass))
      {
        DWORD dwID = GetCurrentThreadId();
        _TRACE3("TCObjectLock<%hs> on thread 0x%0X (%d): Unlocking\n",
          pszClass, dwID, dwID);
      }
    #endif // _DEBUG
  #endif // _ENABLE_TRACE_LOCKS

  if (m_pObject)
  {
    m_pObject->Unlock();
    m_pObject = NULL;
  }
}


/////////////////////////////////////////////////////////////////////////////