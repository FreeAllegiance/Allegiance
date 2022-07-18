/////////////////////////////////////////////////////////////////////////////
// StrManip.cpp : Implementation of some handy string manipulation routines.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "StrManip.h"


/////////////////////////////////////////////////////////////////////////////
// Group=


/////////////////////////////////////////////////////////////////////////////
// {secret}
int TCReplaceTextImpl(LPCTSTR pszSource, LPCTSTR pszFind, LPCTSTR pszReplace,
  LPTSTR pszDest, int cchMaxDest, bool bIgnoreCase, int* pnOccurrences)
{
  // Initialize the number of occurrences found
  if (pnOccurrences)
    *pnOccurrences = 0;

  // Count the length of the source string
  int cchSource = pszSource ? _tcslen(pszSource) : 0;

  // If ignoring case, copy the source string and make it lowercase
  LPCTSTR pszSource2;
  if (cchSource)
  {
    if (bIgnoreCase)
    {
      LPTSTR pszTemp = LPTSTR(_alloca((cchSource + 1) * sizeof TCHAR));
      _tcscpy(pszTemp, pszSource);
      _tcslwr(pszTemp);
      pszSource2 = pszTemp;
    }
    else
      pszSource2 = pszSource;
  }

  // Count the length of the search phrase
  int cchFind = pszFind ? _tcslen(pszFind) : 0;

  // If ignoring case, copy the search phrase and make it lowercase
  LPCTSTR pszFind2;
  if (cchFind)
  {
    if (bIgnoreCase)
    {
      LPTSTR pszTemp = LPTSTR(_alloca((cchFind + 1) * sizeof TCHAR));
      _tcscpy(pszTemp, pszFind);
      _tcslwr(pszTemp);
      pszFind2 = pszTemp;
    }
    else
      pszFind2 = pszFind;
  }

  // Count the length of the replacement string
  int cchReplace = pszReplace ? _tcslen(pszReplace) : 0;

  // Ensure that cchMaxDest is zero if pszDest is NULL
  if (!pszDest)
    cchMaxDest = 0;
  // Determine if pszDest is equal to pszSource (in-place operation)
  else if (pszDest == pszSource)
  {
    // Ensure that the replacement phrase is no longer than the search phrase
    if (cchReplace > cchFind)
      return -1;
    
    // If cchMaxDest is negative, use the length of the source string
    if (cchMaxDest < 0)
      cchMaxDest = cchSource + 1;
    // Ensure that cchMaxDest characters will fit into the source string
    else if (cchMaxDest > (cchSource + 1))
      return -1;
  }
  // pszDest cannot be within pszSource
  else if (pszDest > pszSource && pszDest <= pszSource + cchSource)
    return -1;
  // Otherwise cchMaxDest must be zero or positive
  else if (cchMaxDest < 0)
    return -1;

  // Loop until search phrase is not found
  int nOccurrences = 0, cchNeeded;
  if (cchSource && cchFind)
  {
    LPCTSTR pszBegin = pszSource;
    LPCTSTR pszBegin2 = pszSource2;
    LPCTSTR pszFound;
    while (NULL != (pszFound = _tcsstr(pszBegin2, pszFind2)))
    {
      // Increment the count of occurrences
      ++nOccurrences;

      // Copy the string up to the first character of the search phrase
      int cch = pszFound - pszBegin2;
      if (cch < cchMaxDest)
        _tcsncpy(pszDest, pszBegin, cch);
      else
        TC_tcscpyn(pszDest, pszBegin, cchMaxDest);
      cchMaxDest = std::max(0, cchMaxDest - cch);
      pszDest += cch;

      // Copy the replacement string
      if (cchReplace < cchMaxDest)
        _tcsncpy(pszDest, pszReplace, cchReplace);
      else
        TC_tcscpyn(pszDest, pszReplace, cchMaxDest);
      cchMaxDest = std::max(0, cchMaxDest - cchReplace);
      pszDest += cchReplace;

      // Increment the begining pointers past the search phrase
      pszBegin += cch + cchFind;
      pszBegin2 += cch + cchFind;
    }

    // Copy the remaining part of the string and ensure null-termination
    TC_tcscpyn(pszDest, pszBegin, cchMaxDest);

    // Compute the number of characters needed for the destination string
    cchNeeded = nOccurrences * (cchReplace - cchFind) + cchSource;
  }
  else if (!cchFind)
  {
    TC_tcscpyn(pszDest, pszReplace, cchMaxDest);
    nOccurrences = 1;
    cchNeeded = cchReplace;
  }
  else
  {
    nOccurrences = 0;
    cchNeeded = 0;
    TC_tcscpyn(pszDest, NULL, cchMaxDest);
  }

  // Copy the number of occurrences of the search phrase that were found
  if (pnOccurrences)
    *pnOccurrences = nOccurrences;

  // Return the number of characters needed for the destination string
  return cchNeeded;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Replaces occurences of a substring within another string.
//
// This function copies /pszSource/ to /pszDest/, replacing all occurrences
// of /pszFind/ with /pszReplace/.
//
// By specifying *NULL* for /pszDest/ or zero for /cchMaxDest/, the function
// simply counts the number of occurrences of /pszFind/ that are in
// /pszSource/ and computes the number of characters needed for the search
// and replace operation.
//
// Note: The search for /pszFind/ within /pszSource/ is performed sensitive
// to case. For a case-insensitive search and replace, use the
// TCReplaceTextNoCase function.
//
// Parameters:
//   pszSource - [in] Points to a null-terminated string from which the
// function copies characters and searches for /pszFind/.
//   pszFind - [in] Points to a null-terminated string which is to be
// replaced by /pszReplace/ when copying /pszSource/ to /pszDest/. If
// *NULL* or an empty string, the entire source string is replaced.
//   pszReplace - [in] Points to a null-terminated string with which every
// occurrence of /pszFind/ is replaced when copying /pszSource/ to /pszDest/.
// If *NULL* or an empty string, all occurrences of /pszFind/ are removed.
//   pszDest - [out] Points to a buffer into which the function copies
// characters. The buffer must be large enough to contain the number of
// characters specified by /cchMaxDest/, including room for a terminating
// null character. This may be *NULL*, in which case the function only counts
// the number of occurrences of /pszFind/ in /pszSource/ and computes the
// number of characters needed for the search and replace operation.
//   cchMaxDest - [in] Specifies the maximum number of characters to be
// copied into the buffer pointed to by /pszDest/, including a terminating
// null character. This may be 0, in which case /pszDest/ is ignored and the
// function only counts the number of occurrences of /pszFind/ in
// /pszSource/ and computes the number of characters needed for the search
// and replace operation.
//   pnOccurrences - [out] Points to a variable that receives the number of
// occurrences of /pszFind/ that were found in /pszSource/. This defaults to
// *NULL*, which specifies that the caller does not need this information.
//
// Return Value: The number of characters needed to hold the entire output
// string, not including the terminating null character. One of the MFC
// wrapper function overrides returns a *CString* object as the destination
// string.
//
// See Also: TCReplaceTextNoCase
int TCReplaceText(LPCTSTR pszSource, LPCTSTR pszFind, LPCTSTR pszReplace,
  LPTSTR pszDest, int cchMaxDest, int* pnOccurrences)
{
  // Delegate to TCReplaceTextImpl
  return TCReplaceTextImpl(pszSource, pszFind, pszReplace, pszDest,
    cchMaxDest, false, pnOccurrences);
}


/////////////////////////////////////////////////////////////////////////////
// Description: Replaces occurences of a substring within another string.
//
// This function copies /pszSource/ to /pszDest/, replacing all occurrences
// of /pszFind/ with /pszReplace/.
//
// By specifying *NULL* for /pszDest/ or zero for /cchMaxDest/, the function
// simply counts the number of occurrences of /pszFind/ that are in
// /pszSource/ and computes the number of characters needed for the search
// and replace operation.
//
// Note: The search for /pszFind/ within /pszSource/ is performed without
// regard to case. For a case-sensitive search and replace, use the
// TCReplaceText function.
//
// Parameters:
//   pszSource - [in] Points to a null-terminated string from which the
// function copies characters and searches for /pszFind/.
//   pszFind - [in] Points to a null-terminated string which is to be
// replaced by /pszReplace/ when copying /pszSource/ to /pszDest/. If
// *NULL* or an empty string, the entire source string is replaced.
//   pszReplace - [in] Points to a null-terminated string with which every
// occurrence of /pszFind/ is replaced when copying /pszSource/ to /pszDest/.
// If *NULL* or an empty string, all occurrences of /pszFind/ are removed.
//   pszDest - [out] Points to a buffer into which the function copies
// characters. The buffer must be large enough to contain the number of
// characters specified by /cchMaxDest/, including room for a terminating
// null character. This may be *NULL*, in which case the function only counts
// the number of occurrences of /pszFind/ in /pszSource/ and computes the
// number of characters needed for the search and replace operation.
//   cchMaxDest - [in] Specifies the maximum number of characters to be
// copied into the buffer pointed to by /pszDest/, including a terminating
// null character. This may be 0, in which case /pszDest/ is ignored and the
// function only counts the number of occurrences of /pszFind/ in
// /pszSource/ and computes the number of characters needed for the search
// and replace operation.
//   pnOccurrences - [out] Points to a variable that receives the number of
// occurrences of /pszFind/ that were found in /pszSource/. This defaults to
// *NULL*, which specifies that the caller does not need this information.
//
// Return Value: The number of characters needed to hold the entire output
// string, not including the terminating null character. One of the MFC
// wrapper function overrides returns a *CString* object as the destination
// string.
//
// See Also: TCReplaceText
int TCReplaceTextNoCase(LPCTSTR pszSource, LPCTSTR pszFind,
  LPCTSTR pszReplace, LPTSTR pszDest, int cchMaxDest, int* pnOccurrences)
{
  // Delegate to TCReplaceTextImpl
  return TCReplaceTextImpl(pszSource, pszFind, pszReplace, pszDest,
    cchMaxDest, true, pnOccurrences);
}


/////////////////////////////////////////////////////////////////////////////
// MFC Search and Replace Wrappers

#ifdef _AFX

  ///////////////////////////////////////////////////////////////////////////
  CString TCReplaceText(CString strSource, CString strFind,
    CString strReplace, int* pnOccurrences)
  {
    CString strDest;
    TCReplaceText(strSource, strFind, strReplace, strDest, NULL);
    return strDest;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Parameters:
  //   strDest - [out] A reference to a *CString* object that receives the
  // result of the search and replace operation.
  int TCReplaceText(CString strSource, CString strFind, CString strReplace,
    CString& strDest, int* pnOccurrences)
  {
    // Compute the number of characters needed for the destination string
    int cchDest = TCReplaceText(strSource, strFind, strReplace, NULL, 0);

    // Allocate the buffer in the specified CString
    LPTSTR pszDest = strDest.GetBuffer(cchDest + 1);

    // Call the non-MFC function overload
    int cchDest2 = TCReplaceText(strSource, strFind, strReplace, pszDest,
      cchDest + 1, pnOccurrences);
    ZAssert(cchDest2 == cchDest);

    // Release the CString buffer
    strDest.ReleaseBuffer(cchDest);

    // Return the number of characters needed for the destination string
    return cchDest;
  }

  ///////////////////////////////////////////////////////////////////////////
  CString TCReplaceTextNoCase(CString strSource, CString strFind,
    CString strReplace, int* pnOccurrences)
  {
    CString strDest;
    TCReplaceTextNoCase(strSource, strFind, strReplace, strDest, NULL);
    return strDest;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Parameters:
  //   strDest - [out] A reference to a *CString* object that receives the
  // result of the search and replace operation.
  int TCReplaceTextNoCase(CString strSource, CString strFind,
    CString strReplace, CString& strDest, int* pnOccurrences)
  {
    // Compute the number of characters needed for the destination string
    int cchDest = TCReplaceTextNoCase(strSource, strFind, strReplace, NULL,
      0);

    // Allocate the buffer in the specified CString
    LPTSTR pszDest = strDest.GetBuffer(cchDest + 1);

    // Call the non-MFC function overload
    int cchDest2 = TCReplaceTextNoCase(strSource, strFind, strReplace,
      pszDest, cchDest + 1, pnOccurrences);
    ZAssert(cchDest2 == cchDest);

    // Release the CString buffer
    strDest.ReleaseBuffer(cchDest);

    // Return the number of characters needed for the destination string
    return cchDest;
  }

#endif // _AFX

