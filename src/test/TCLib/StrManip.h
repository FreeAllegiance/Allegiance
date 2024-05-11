#pragma once

/////////////////////////////////////////////////////////////////////////////
// StrManip.h : Declaration of some handy string manipulation routines.
//


/////////////////////////////////////////////////////////////////////////////
// Description: Copy characters of one string to another.
//
// This function is similar to the *_tcsncpy* function, except that, like
// *lstrcpyn*, the destination string is guaranteed to have a terminating
// null character. Rather than using *lstrcpyn*, this function is implemented
// by calling *_tcscpyn* so as to not mix C++ runtime with Windows string
// functions.
//
// Note that the buffer pointed to by /pszDest/ must be large enough to
// include a terminating null character, and the string length value
// specified by /cchMaxDest/ includes room for a terminating null character.
// Thus, the following code
//
//      TCHAR szBuffer[512];
//      …
//      TC_tcscpyn(szBuffer, TEXT("abcdefghijklmnop"), 4) ;
//
// ... copies the string "abc", followed by a terminating null character, to
// szBuffer.
//
// Parameters:
//   pszDest - Points to a buffer into which the function copies characters.
// The buffer must be large enough to contain the number of characters
// specified by /cchMaxDest/, including room for a terminating null character.
//   pszSource - Points to a null-terminated string from which the function
// copies characters.
//   cchMaxDest - Specifies the number of characters to be copied from the
// string pointed to by /pszSource/ into the buffer pointed to by /pszDest/,
// including a terminating null character.
//
// Return Value: This function always returns /pszDest/. No return value is
// reserved to indicate an error.
//
// See Also: *_tcsncpy*, *lstrcpyn*
inline LPTSTR TC_tcscpyn(LPTSTR pszDest, LPCTSTR pszSource, int cchMaxDest)
{
  if (cchMaxDest > 0)
  {
    if (!pszSource)
      pszSource = TEXT("");
    _tcsncpy(pszDest, pszSource, cchMaxDest - 1);
    pszDest[cchMaxDest - 1] = TEXT('\0');
  }
  return pszDest;
}


/////////////////////////////////////////////////////////////////////////////
// Search and Replace

int TCReplaceText(LPCTSTR pszSource, LPCTSTR pszFind, LPCTSTR pszReplace,
  LPTSTR pszDest, int cchMaxDest, int* pnOccurrences = NULL);

int TCReplaceTextNoCase(LPCTSTR pszSource, LPCTSTR pszFind,
  LPCTSTR pszReplace, LPTSTR pszDest, int cchMaxDest,
  int* pnOccurrences = NULL);


/////////////////////////////////////////////////////////////////////////////
// MFC Search and Replace Wrappers

#ifdef _AFX

  CString TCReplaceText(CString strSource, CString strFind,
    CString strReplace, int* pnOccurrences = NULL);

  int TCReplaceText(CString strSource, CString strFind, CString strReplace,
    CString& strDest, int* pnOccurrences = NULL);

  CString TCReplaceTextNoCase(CString strSource, CString strFind,
    CString strReplace, int* pnOccurrences = NULL);

  int TCReplaceTextNoCase(CString strSource, CString strFind,
    CString strReplace, CString& strDest, int* pnOccurrences = NULL);

#endif // _AFX


/////////////////////////////////////////////////////////////////////////////