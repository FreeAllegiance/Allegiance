#ifndef __EditWindow_h__
#define __EditWindow_h__

/////////////////////////////////////////////////////////////////////////////
// EditWindow.h | Declaration of the TCEditWindow class.

#include <malloc.h>


/////////////////////////////////////////////////////////////////////////////
// TCEditWindow is a derivation of the ATL CWindowImpl template that
// provides an encapsulation of a Windows edit control. It is closely modeled
// after the MFC CEdit class. You should refer to that documentation for any
// information missing from this class overview, such as notifications sent
// from the control.
//
// The TCEditWindow class provides the functionality of a Windows edit
// control. An edit control is a rectangular child window in which the user
// can enter text.
//
// TCEditWindow inherits significant functionality from CWindowImpl, which
// inherits from CWindow. To set and retrieve text from an TCEditWindow
// object, use the CWindow member functions *SetWindowText* and
// *GetWindowText*, which set or get the entire contents of an edit control,
// even if it is a multiline control. Also, if an edit control is multiline,
// you can get and set only part of the control's text by calling the
// TCEditWindow member functions GetLine, SetSel, GetSel, and ReplaceSel.
//
// See Also: ATL::CWindowImpl, ATL::CWindow, CEdit (MFC)
class TCEditWindow : public CWindowImpl<TCEditWindow>
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef CWindowImpl<TCEditWindow> TCEditWindow_Base;

// Declarations
public:
  DECLARE_WND_SUPERCLASS(NULL, TEXT("EDIT"))

// Message Map
public:
  BEGIN_MSG_MAP(TCEditWindow)
  END_MSG_MAP()

// Attributes / Operations
public:
  BOOL CanUndo() const;
  int GetLineCount() const;
  BOOL GetModify() const;
  void SetModify(BOOL bModified = TRUE);
  void GetRect(LPRECT lpRect) const;
  void GetSel(int& nStartChar, int& nEndChar) const;
  DWORD GetSel() const;
  HLOCAL GetHandle() const;
  void SetHandle(HLOCAL hBuffer);
  int GetLine(int nIndex, BSTR* pbstr) const;
  int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;
  void EmptyUndoBuffer();
  BOOL FmtLines(BOOL bAddEOL);
  void LimitText(int nChars = 0);
  int LineFromChar(int nIndex = -1) const;
  int LineIndex(int nLine = -1) const;
  int LineLength(int nLine = -1) const;
  void LineScroll(int nLines, int nChars = 0);
  void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
  void SetPasswordChar(TCHAR ch);
  void SetRect(LPCRECT lpRect);
  void SetRectNP(LPCRECT lpRect);
  void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
  void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE);
  BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
  void SetTabStops();
  BOOL SetTabStops(const int& cxEachStop);    // takes an 'int'
  BOOL Undo();
  void Clear();
  void Copy();
  void Cut();
  void Paste();
  BOOL SetReadOnly(BOOL bReadOnly = TRUE);
  int GetFirstVisibleLine() const;
  TCHAR GetPasswordChar() const;
  void SetMargins(UINT nLeft, UINT nRight);
  DWORD GetMargins() const;
  void SetLimitText(UINT nMax);
  UINT GetLimitText() const;
  POINT PosFromChar(UINT nChar) const;
  int CharFromPos(POINT pt) const;

// Operators
public:
  const TCEditWindow& operator=(HWND hwnd);
};


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes / Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Determines whether an edit-control operation can be undone.
//
// Call this function to determine if the last edit operation can be undone.
//
// Return Value: Nonzero if the last edit operation can be undone by a call
// to the Undo member function; 0 if it cannot be undone.
//
// See Also: TCEditWindow::Undo, TCEditWindow::EmptyUndoBuffer, EM_CANUNDO
inline BOOL TCEditWindow::CanUndo() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_CANUNDO, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the number of lines in a multiple-line edit
// control.
//
// Call this function to retrieve the number of lines in a multiple-line edit
// control. 
//
// GetLineCount is only processed by multiple-line edit controls.
//
// Return Value: An integer containing the number of lines in the
// multiple-line edit control. If no text has been entered into the edit
// control, the return value is 1.
//
// See Also: EM_GETLINECOUNT
inline int TCEditWindow::GetLineCount() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_GETLINECOUNT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines whether the contents of an edit control have been
// modified.
//
// Call this function to determine whether the contents of an edit control
// have been modified. 
//
// Windows maintains an internal flag indicating whether the contents of the
// edit control have been changed. This flag is cleared when the edit control
// is first created and may also be cleared by calling the SetModify member
// function.
//
// Return Value: Nonzero if the edit-control contents have been modified; 0
// if they have remained unchanged.
//
// See Also: TCEditWindow::SetModify, EM_GETMODIFY
inline BOOL TCEditWindow::GetModify() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_GETMODIFY, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or clears the modification flag for an edit control.
//
// Call this function to set or clear the modified flag for an edit control.
// The modified flag indicates whether or not the text within the edit
// control has been modified. It is automatically set whenever the user
// changes the text. Its value may be retrieved with the GetModify member
// function.
//
// Parameters:
//   bModified - A value of *TRUE* indicates that the text has been modified,
// and a value of *FALSE* indicates it is unmodified. By default, the
// modified flag is set.
//
//
// See Also: TCEditWindow::GetModify, EM_SETMODIFY
inline void TCEditWindow::SetModify(BOOL bModified)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETMODIFY, bModified, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the formatting rectangle of an edit control.
//
// Call this function to get the formatting rectangle of an edit control.
// The formatting rectangle is the limiting rectangle of the text, which is
// independent of the size of the edit-control window. 
//
// The formatting rectangle of a multiple-line edit control can be modified
// by the SetRect and SetRectNP member functions.
//
// Parameters:
//   lpRect - Points to the *RECT* structure that receives the formatting
// rectangle.
//
// See Also: TCEditWindow::SetRect, TCEditWindow::SetRectNP, EM_GETRECT
inline void TCEditWindow::GetRect(LPRECT lpRect) const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_GETRECT, 0, (LPARAM)lpRect);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the starting and ending character positions of the
// current selection in an edit control.
//
// Call this function to get the starting and ending character positions of
// the current selection (if any) in an edit control, using either the return
// value or the parameters.
//
// Parameters:
//   nStartChar - Reference to an integer that will receive the position of
// the first character in the current selection.
//   nEndChar - Reference to an integer that will receive the position of the
// first nonselected character past the end of the current selection.
//
// See Also: TCEditWindow::SetSel, EM_GETSEL
inline void TCEditWindow::GetSel(int& nStartChar, int& nEndChar) const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar);
}

/////////////////////////////////////////////////////////////////////////////
// Return Value: The version that returns a DWORD returns a value that
// contains the starting position in the low-order word and the position of
// the first nonselected character after the end of the selection in the
// high-order word.
inline DWORD TCEditWindow::GetSel() const
{
  ZAssert(::IsWindow(m_hWnd));
  return ::SendMessage(m_hWnd, EM_GETSEL, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a handle to the memory currently allocated for a
// multiple-line edit control.
//
// Call this function to retrieve a handle to the memory currently allocated
// for a multiple-line edit control. The handle is a local memory handle and
// may be used by any of the Local Windows memory functions that take a local
// memory handle as a parameter. 
//
// GetHandle is processed only by multiple-line edit controls. 
//
// Call GetHandle for a multiple-line edit control in a dialog box only if
// the dialog box was created with the *DS_LOCALEDIT* style flag set. If the
// *DS_LOCALEDIT* style is not set, you will still get a nonzero return
// value, but you will not be able to use the returned value.
//
// Return Value: A local memory handle that identifies the buffer holding the
// contents of the edit control. If an error occurs, such as sending the
// message to a single-line edit control, the return value is 0.
//
// See Also: TCEditWindow::SetHandle, EM_GETHANDLE
inline HLOCAL TCEditWindow::GetHandle() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (HLOCAL)::SendMessage(m_hWnd, EM_GETHANDLE, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the handle to the local memory that will be used by a
// multiple-line edit control.
//
// Call this function to set the handle to the local memory that will be used
// by a multiple-line edit control. The edit control will then use this
// buffer to store the currently displayed text instead of allocating its own
// buffer. 
//
// This member function is processed only by multiple-line edit controls. 
//
// Before an application sets a new memory handle, it should use the
// GetHandle member function to get the handle to the current memory buffer
// and free that memory using the *LocalFree* Windows function. 
//
// SetHandle clears the undo buffer (the CanUndo member function then returns
// 0) and the internal modification flag (the GetModify member function then
// returns 0). The edit-control window is redrawn. 
//
// You can use this member function in a multiple-line edit control in a
// dialog box only if you have created the dialog box with the
// *DS_LOCALEDIT* style flag set.
//
// Parameters:
//   hBuffer - Contains a handle to the local memory. This handle must have
// been created by a previous call to the *LocalAlloc* Windows function using
// the *LMEM_MOVEABLE* flag. The memory is assumed to contain a
// null-terminated string. If this is not the case, the first byte of the
// allocated memory should be set to 0.
//
// See Also: TCEditWindow::CanUndo, TCEditWindow::GetHandle,
// TCEditWindow::GetModify, EM_SETHANDLE, LocalAlloc, LocalFree
inline void TCEditWindow::SetHandle(HLOCAL hBuffer)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETHANDLE, (WPARAM)hBuffer, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a line of text from an edit control.
//
// Call this function to retrieve a line of text from an edit control. This
// call is not processed for a single-line edit control. 
//
// The copied line does not contain a null-termination character.
//
// Parameters:
//   nIndex - Specifies the line number to retrieve from a multiple-line edit
// control. Line numbers are zero-based; a value of 0 specifies the first
// line. This parameter is ignored by a single-line edit control.
//   pbstr - The address of a BSTR variable to receive the specified line of
// text. The caller must free this BSTR using ::SysFreeString when it no
// longer needs it.
//
// TODO: This could be made slightly more efficient by using
// ::SysAllocStringLength since we already know the length in advance.
//
// TODO: The BSTR version of this does not appear to be initializing the
// first word of the buffer. This should probably be fixed.
//
// Return Value: The number of bytes actually copied. The return value is 0
// if the line number specified by nIndex is greater then the number of lines
// in the edit control.
//
// See Also: TCEditWindow::LineLength, CWindow::GetWindowText, EM_GETLINE
inline int TCEditWindow::GetLine(int nIndex, BSTR* pbstr) const
{
  *pbstr = NULL;
  ZAssert(::IsWindow(m_hWnd));

  int cchLine = LineLength(nIndex);
  if (!cchLine)
    return 0;

  LPTSTR psz = (LPTSTR)_alloca(cchLine + sizeof(TCHAR));
  int nResult = (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, LPARAM(psz));
  psz[nResult] = TEXT('\0');

  USES_CONVERSION;
  *pbstr = SysAllocString(T2OLE(psz));

  return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   pszBuffer - Points to the buffer that receives a copy of the line. The
// first word of the buffer must specify the maximum number of bytes that can
// be copied to the buffer.
//   nMaxLength - Specifies the maximum number of bytes that can be copied to
// the buffer. GetLine places this value in the first word of
// /pszBuffer/ before making the call to Windows.
inline int TCEditWindow::GetLine(int nIndex, LPTSTR pszBuffer,
  int nMaxLength) const
{
  ZAssert(::IsWindow(m_hWnd));
  *(LPWORD)pszBuffer = (WORD)nMaxLength;
  return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)pszBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Resets (clears) the undo flag of an edit control.
//
// Call this function to reset (clear) the undo flag of an edit control. The
// edit control will now be unable to undo the last operation. The undo flag
// is set whenever an operation within the edit control can be undone. 
//
// The undo flag is automatically cleared whenever the SetHandle or the
// CWindow::SetWindowText member functions are called.
//
// See Also: TCEditWindow::CanUndo, TCEditWindow::SetHandle,
// TCEditWindow::Undo, CWindow::SetWindowText, EM_EMPTYUNDOBUFFER
inline void TCEditWindow::EmptyUndoBuffer()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_EMPTYUNDOBUFFER, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the inclusion of soft line-break characters on or off
// within a multiple-line edit control.
//
// Call this function to set the inclusion of soft line-break characters on
// or off within a multiple-line edit control. A soft line break consists of
// two carriage returns and a linefeed inserted at the end of a line that is
// broken because of word wrapping. A hard line break consists of one
// carriage return and a linefeed. Lines that end with a hard line break are
// not affected by FmtLines. 
//
// Windows will only respond if the TCEditWindow object is a multiple-line
// edit control. 
//
// FmtLines only affects the buffer returned by GetHandle and the text
// returned by *WM_GETTEXT*. It has no impact on the display of the text
// within the edit control.
//
// Parameters:
//   bAddEOL - Specifies whether soft line-break characters are to be
// inserted. A value of *TRUE* inserts the characters; a value of
// *FALSE* removes them.
//
// Return Value: Nonzero if any formatting occurs; otherwise 0.
//
// See Also: TCEditWindow::GetHandle, CWindow::GetWindowText, EM_FMTLINES
inline BOOL TCEditWindow::FmtLines(BOOL bAddEOL)
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_FMTLINES, bAddEOL, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Limits the length of the text that the user may enter into an
// edit control.
//
// Call this function to limit the length of the text that the user may enter
// into an edit control. 
//
// Changing the text limit restricts only the text the user can enter. It has
// no effect on any text already in the edit control, nor does it affect the
// length of the text copied to the edit control by the
// CWindow::SetWindowText member function. If an application uses the
// SetWindowText function to place more text into an edit control than is
// specified in the call to LimitText, the user can delete any of the text
// within the edit control. However, the text limit will prevent the user
// from replacing the existing text with new text, unless deleting the
// current selection causes the text to fall below the text limit.
//
// Note: In Win32 (Windows NT and Windows 95), SetLimitText replaces this
// function.
//
// Parameters:
//   nChars - Specifies the maximum number of characters the user can enter.
// If this parameter is zero, the text length is set to 0x7FFFFFFE characters
// for single-line edit controls or 0xFFFFFFFF for multiline edit controls.
//
// See Also: TCEditWindow::GetLimitText, TCEditWindow::SetLimitText,
// CWindow::SetWindowText, EM_LIMITTEXT
inline void TCEditWindow::LimitText(int nChars)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_LIMITTEXT, nChars, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the line number of the line that contains the
// specified character index.
//
// Call this function to retrieve the line number of the line that contains
// the specified character index. A character index is the number of
// characters from the beginning of the edit control. 
//
// This member function is only used by multiple-line edit controls.
//
// Parameters:
//   nIndex - Contains the zero-based index value for the desired character
// in the text of the edit control, or contains –1. If nIndex is –1, it
// specifies the current line, that is, the line that contains the caret.
//
// Return Value: The zero-based line number of the line containing the
// character index specified by nIndex. If nIndex is –1, the number of the
// line that contains the first character of the selection is returned.
// If there is no selection, the current line number is returned.
//
// See Also: TCEditWindow::LineIndex, EM_LINEFROTCHAR
inline int TCEditWindow::LineFromChar(int nIndex) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_LINEFROTCHAR, nIndex, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the character index of a line within a
// multiple-line edit control.
//
// Call this function to retrieve the character index of a line within a
// multiple-line edit control. The character index is the number of
// characters from the beginning of the edit control to the specified line. 
//
// This member function is only processed by multiple-line edit controls.
//
// Parameters:
//   nLine - Contains the index value for the desired line in the text of the
// edit control, or contains –1. If nLine is –1, it specifies the current
// line, that is, the line that contains the caret.
//
// Return Value: The character index of the line specified in nLine or –1 if
// the specified line number is greater then the number of lines in the edit
// control.
//
// See Also: TCEditWindow::LineFromChar, EM_LINEINDEX
inline int TCEditWindow::LineIndex(int nLine) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_LINEINDEX, nLine, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the length of a line in an edit control.
//
// Call this function to retrieve the length of a line in an edit control. 
//
// Use the LineIndex member function to retrieve a character index for a
// given line number within a multiple-line edit control.
//
// Parameters:
//   nLine - Specifies the character index of a character in the line whose
// length is to be retrieved. If this parameter is –1, the length of the
// current line (the line that contains the caret) is returned, not including
// the length of any selected text within the line. When LineLength is called
// for a single-line edit control, this parameter is ignored.
//
// Return Value: When LineLength is called for a multiple-line edit control,
// the return value is the length (in characters) of the line specified by
// /nLine/. When LineLength is called for a single-line edit control, the
// return value is the length (in characters) of the text in the edit
// control.
//
// See Also: TCEditWindow::LineIndex, EM_LINELENGTH
inline int TCEditWindow::LineLength(int nLine) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_LINELENGTH, nLine, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Scrolls the text of a multiple-line edit control.
//
// Call this function to scroll the text of a multiple-line edit control. 
//
// This member function is processed only by multiple-line edit controls. 
//
// The edit control does not scroll vertically past the last line of text in
// the edit control. If the current line plus the number of lines specified
// by /nLines/ exceeds the total number of lines in the edit control, the
// value is adjusted so that the last line of the edit control is scrolled to
// the top of the edit-control window. 
//
// LineScroll can be used to scroll horizontally past the last character of
// any line.
//
// Parameters:
//   nLines - Specifies the number of lines to scroll vertically.
//   nChars - Specifies the number of character positions to scroll
// horizontally. This value is ignored if the edit control has either the
// *ES_RIGHT* or *ES_CENTER* style.
//
// See Also: TCEditWindow::LineIndex, EM_LINESCROLL
inline void TCEditWindow::LineScroll(int nLines, int nChars)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_LINESCROLL, nChars, nLines);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Replaces the current selection in an edit control with the
// specified text.
//
// Call this function to replace the current selection in an edit control
// with the text specified by /pszNewText/.
//
// Replaces only a portion of the text in an edit control. If you want to
// replace all of the text, use the CWindow::SetWindowText member function. 
//
// If there is no current selection, the replacement text is inserted at the
// current cursor location.
//
// Parameters:
//   pszNewText - Points to a null-terminated string containing the
// replacement text.
//   bCanUndo - To specify that this function can be undone, set the value of
// this parameter to *TRUE* . The default value is *FALSE*.
//
// See Also: CWindow::SetWindowText, EM_REPLACESEL
inline void TCEditWindow::ReplaceSel(LPCTSTR pszNewText, BOOL bCanUndo)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_REPLACESEL, bCanUndo, (LPARAM)pszNewText);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or removes a password character displayed in an edit
// control when the user enters text.
//
// Call this function to set or remove a password character displayed in an
// edit control when the user types text. When a password character is set,
// that character is displayed for each character the user types. 
//
// This member function has no effect on a multiple-line edit control. 
//
// When the SetPasswordChar member function is called, TCEditWindow will
// redraw all visible characters using the character specified by /ch/. 
//
// If the edit control is created with the *ES_PASSWORD* style, the default
// password character is set to an asterisk (*). This style is removed if
// SetPasswordChar is called with /ch/ set to 0.
//
// Parameters:
//   ch - Specifies the character to be displayed in place of the character
// typed by the user. If /ch/ is 0, the actual characters typed by the user
// are displayed.
//
// See Also: TCEditWindow::GetPasswordChar, EM_SETPASSWORDCHAR
inline void TCEditWindow::SetPasswordChar(TCHAR ch)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETPASSWORDCHAR, ch, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the formatting rectangle of a multiple-line edit control
// and updates the control.
//
// Call this function to set the dimensions of a rectangle using the
// specified coordinates. This member is processed only by multiple-line edit
// controls. 
//
// Use SetRect to set the formatting rectangle of a multiple-line edit
// control. The formatting rectangle is the limiting rectangle of the text,
// which is independent of the size of the edit-control window. When the edit
// control is first created, the formatting rectangle is the same as the
// client area of the edit-control window. By using the SetRect member
// function, an application can make the formatting rectangle larger or
// smaller than the edit-control window. 
//
// If the edit control has no scroll bar, text will be clipped, not wrapped,
// if the formatting rectangle is made larger than the window. If the edit
// control contains a border, the formatting rectangle is reduced by the size
// of the border. If you adjust the rectangle returned by the GetRect member
// function, you must remove the size of the border before you pass the
// rectangle to SetRect. 
//
// When SetRect is called, the edit control's text is also reformatted and
// redisplayed.
//
// Parameters:
//   lpRect - Points to the *RECT* structure that specifies the new
// dimensions of the formatting rectangle.
//
// See Also: TCEditWindow::GetRect, TCEditWindow::SetRectNP, EM_SETRECT
inline void TCEditWindow::SetRect(LPCRECT lpRect)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETRECT, 0, (LPARAM)lpRect);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the formatting rectangle of a multiple-line edit control
// without redrawing the control window.
//
// Call this function to set the formatting rectangle of a multiple-line
// edit control. The formatting rectangle is the limiting rectangle of the
// text, which is independent of the size of the edit-control window. 
//
// SetRectNP is identical to the SetRect member function except that the
// edit-control window is not redrawn. 
//
// When the edit control is first created, the formatting rectangle is the
// same as the client area of the edit-control window. By calling the
// SetRectNP member function, an application can make the formatting
// rectangle larger or smaller than the edit-control window. 
//
// If the edit control has no scroll bar, text will be clipped, not wrapped,
// if the formatting rectangle is made larger than the window. 
//
// This member is processed only by multiple-line edit controls.
//
// Parameters:
//   lpRect - Points to the *RECT* structure that specifies the new
// dimensions of the formatting rectangle.
//
// See Also: TCEditWindow::GetRect, TCEditWindow::SetRect, EM_SETRECTNP
inline void TCEditWindow::SetRectNP(LPCRECT lpRect)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETRECTNP, 0, (LPARAM)lpRect);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Selects a range of characters in an edit control.
//
// Call this function to select a range of characters in an edit control.
//
// Parameters:
//   dwSelection - Specifies the starting position in the low-order word and
// the ending position in the high-order word. If the low-order word is 0
// and the high-order word is –1, all the text in the edit control is
// selected. If the low-order word is –1, any current selection is removed.
//   bNoScroll - Indicates whether the caret should be scrolled into view. If
// *FALSE*, the caret is scrolled into view. If *TRUE*, the caret is not
// scrolled into view.
//
// See Also: TCEditWindow::GetSel, TCEditWindow::ReplaceSel, EM_SETSEL
inline void TCEditWindow::SetSel(DWORD dwSelection, BOOL bNoScroll)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
  if (!bNoScroll)
    ::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   nStartChar - Specifies the starting position. If /nStartChar/ is 0 and
// /nEndChar/ is –1, all the text in the edit control is selected. If
// /nStartChar/ is –1, any current selection is removed.
//   nEndChar - Specifies the ending position.
inline void TCEditWindow::SetSel(int nStartChar, int nEndChar,
  BOOL bNoScroll)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
  if (!bNoScroll)
    ::SendMessage(m_hWnd, EM_SCROLLCARET, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the tab stops in a multiple-line edit control.
//
// Call this function to set the tab stops in a multiple-line edit control.
// When text is copied to a multiple-line edit control, any tab character in
// the text will cause space to be generated up to the next tab stop.
//
// To set tab stops to the default size of 32 dialog units, call the
// parameterless version of this member function. To set tab stops to a size
// other than 32, call the version with the /cxEachStop/ parameter. To set
// tab stops to an array of sizes, use the version with two parameters. 
//
// This member function is only processed by multiple-line edit controls. 
//
// SetTabStops does not automatically redraw the edit window. If you change
// the tab stops for text already in the edit control, call
// CWindow::InvalidateRect to redraw the edit window.
//
// Parameters:
//   nTabStops - Specifies the number of tab stops contained in rgTabStops.
// This number must be greater than 1.
//   rgTabStops - Points to an array of unsigned integers specifying the tab
// stops in dialog units. A dialog unit is a horizontal or vertical distance.
// One horizontal dialog unit is equal to one-fourth of the current dialog
// base width unit, and 1 vertical dialog unit is equal to one-eighth of the
// current dialog base height unit. The dialog base units are computed based
// on the height and width of the current window font. The Win32
// *MapDialogRect* function converts (maps) the specified dialog box units to
// screen units (pixels).
//
// Return Value: Nonzero if the tabs were set; otherwise 0.
//
// See Also: CWindow::InvalidateRect, EM_SETTABSTOPS, ::MapDialogRect
inline BOOL TCEditWindow::SetTabStops(int nTabStops, LPINT rgTabStops)
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops,
    (LPARAM)rgTabStops);
}

/////////////////////////////////////////////////////////////////////////////
inline void TCEditWindow::SetTabStops()
{
  ZAssert(::IsWindow(m_hWnd));
  LRESULT lResult = ::SendMessage(m_hWnd, EM_SETTABSTOPS, 0, 0);
  ZAssert(lResult);
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   cxEachStop - Specifies that tab stops are to be set at every
// /cxEachStop/ dialog units.
inline BOOL TCEditWindow::SetTabStops(const int& cxEachStop)
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, 1,
    (LPARAM)(LPINT)&cxEachStop);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Reverses the last edit-control operation.
//
// Call this function to undo the last edit-control operation. 
//
// An undo operation can also be undone. For example, you can restore deleted
// text with the first call to Undo. As long as there is no intervening edit
// operation, you can remove the text again with a second call to Undo.
//
// Return Value: For a single-line edit control, the return value is always
// nonzero. For a multiple-line edit control, the return value is nonzero if
// the undo operation is successful, or 0 if the undo operation fails.
//
// See Also: TCEditWindow::CanUndo, EM_UNDO
inline BOOL TCEditWindow::Undo()
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Deletes (clears) the current selection (if any) in the edit
// control.
//
// Call this function to delete (clear) the current selection (if any) in the
// edit control. 
//
// The deletion performed by Clear can be undone by calling the Undo member
// function. 
//
// To delete the current selection and place the deleted contents into the
// Clipboard, call the Cut member function.
//
// See Also: TCEditWindow::Undo, TCEditWindow::Copy, TCEditWindow::Cut,
// TCEditWindow::Paste, WM_CLEAR
inline void TCEditWindow::Clear()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_CLEAR, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Copies the current selection (if any) in the edit control to
// the Clipboard in *CF_TEXT* format.
//
// Call this function to coy the current selection (if any) in the edit
// control to the Clipboard in *CF_TEXT* format.
//
// See Also: TCEditWindow::Clear, TCEditWindow::Cut, TCEditWindow::Paste,
// WM_COPY
inline void TCEditWindow::Copy()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_COPY, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Deletes (cuts) the current selection (if any) in the edit
// control and copies the deleted text to the Clipboard in *CF_TEXT* format.
//
// Call this function to delete (cut) the current selection (if any) in the
// edit control and copy the deleted text to the Clipboard in
// *CF_TEXT* format. 
// 
// The deletion performed by Cut can be undone by calling the Undo member
// function. 
// 
// To delete the current selection without placing the deleted text into the
// Clipboard, call the Clear member function.
//
// See Also: TCEditWindow::Undo, TCEditWindow::Clear, TCEditWindow::Copy,
// TCEditWindow::Paste, WM_CUT
inline void TCEditWindow::Cut()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_CUT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Inserts the data from the Clipboard into the edit control at
// the current cursor position. Data is inserted only if the Clipboard
// contains data in *CF_TEXT* format.
//
// Call this function to insert the data from the Clipboard into the
// TCEditWindow at the insertion point. Data is inserted only if the
// Clipboard contains data in *CF_TEXT* format.
//
// See Also: TCEditWindow::Clear, TCEditWindow::Copy, TCEditWindow::Cut,
// WM_PASTE
inline void TCEditWindow::Paste()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_PASTE, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or clears the read-only state of an edit control.
//
// Calls this function to set the read-only state of an edit control. 
//
// The current setting can be found by testing the *ES_READONLY* flag in the
// return value of CWindow::GetStyle.
//
// Parameters:
//   bReadOnly - Specifies whether to set or remove the read-only state of
// the edit control. A value of *TRUE* sets the state to read-only; a value
// of *FALSE* sets the state to read/write.
//
// Return Value: Nonzero if the operation is successful, or 0 if an error
// occurs.
//
// See Also: CWindow::GetStyle, EM_SETREADONLY
inline BOOL TCEditWindow::SetReadOnly(BOOL bReadOnly )
{
  ZAssert(::IsWindow(m_hWnd));
  return (BOOL)::SendMessage(m_hWnd, EM_SETREADONLY, bReadOnly, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines the topmost visible line in an edit control.
//
// Call this function to determine the topmost visible line in an edit
// control.
//
// Return Value: The zero-based index of the topmost visible line. For
// single-line edit controls, the return value is 0.
//
// See Also: TCEditWindow::GetLine, EM_GETFIRSTVISIBLELINE
inline int TCEditWindow::GetFirstVisibleLine() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_GETFIRSTVISIBLELINE, 0, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the password character displayed in an edit control
// when the user enters text.
//
// Call this function to retrieve the password character displayed in an edit
// control when the user enters text. 
//
// If the edit control is created with the *ES_PASSWORD* style, the default
// password character is set to an asterisk (*).
//
// Return Value: Specifies the character to be displayed in place of the
// character typed by the user. The return value is *NULL* if no password
// character exists.
//
// See Also: TCEditWindow::SetPasswordChar, EM_GETPASSWORDCHAR
inline TCHAR TCEditWindow::GetPasswordChar() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (TCHAR)::SendMessage(m_hWnd, EM_GETPASSWORDCHAR, 0, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the left and right margins for this TCEditWindow.
//
// Call this member function to set the left and right margins of this edit
// control.
//
// Parameters:
//   nLeft - The width of the new left margin, in pixels.
//   nRight - The width of the new right margin, in pixels.
//
// See Also: TCEditWindow::GetMargins, EM_SETMARGINS
inline void TCEditWindow::SetMargins(UINT nLeft, UINT nRight)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN,
    MAKELONG(nLeft, nRight));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the left and right margins for this TCEditWindow.
//
// Call this member function to retrieve the left and right margins of this
// edit control. Margins are measured in pixels.
//
// Return Value: The width of the left margin in the low-order *WORD* and
// the width of the right margin in the high-order *WORD*.
//
// See Also: TCEditWindow::SetMargins, EM_GETMARGINS
inline DWORD TCEditWindow::GetMargins() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (DWORD)::SendMessage(m_hWnd, EM_GETMARGINS, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the maximum amount of text this TCEditWindow can
// contain.
//
// Call this member function to set the text limit for this TCEditWindow
// object. The text limit is the maximum amount of text, in characters, that
// the edit control can accept.
//
// Changing the text limit restricts only the text the user can enter. It has
// no effect on any text already in the edit control, nor does it affect the
// length of the text copied to the edit control by the
// CWindow::SetWindowText member function. If an application uses the
// CWindow::SetWindowText function to place more text into an edit control
// than is specified in the call to LimitText, the user can delete any of the
// text within the edit control. However, the text limit will prevent the
// user from replacing the existing text with new text, unless deleting the
// current selection causes the text to fall below the text limit.
//
// This function replaces LimitText in Win32.
//
// Parameters:
//   nMax - The new text limit, in characters.
//
// See Also: TCEditWindow::GetLimitText, TCEditWindow::LimitText
// EM_SETLIMITTEXT
inline void TCEditWindow::SetLimitText(UINT nMax)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, EM_SETLIMITTEXT, nMax, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the maximum amount of text this TCEditWindow can
// contain.
//
// Call this member function to get the text limit for this TCEditWindow
// object. The text limit is the maximum amount of text, in characters, that
// the edit control can accept.
//
// Return Value: The current text limit, in characters, for this TCEditWindow
// object.
//
// See Also: TCEditWindow::SetLimitText, TCEditWindow::LimitText,
// EM_GETLIMITTEXT
inline UINT TCEditWindow::GetLimitText() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (UINT)::SendMessage(m_hWnd, EM_GETLIMITTEXT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the coordinates of the upper-left corner of a
// specified character index.
//
// Call this function to get the position (top-left corner) of a given
// character within this TCEditWindow object. The character is specified by
// giving its zero-based index value. If /nChar/ is greater than the index of
// the last character in this TCEditWindow object, the return value specifies
// the coordinates of the character position just past the last character in
// this TCEditWindow object.
//
// Parameters:
//   nChar - The zero-based index of the character for which the position is
// to be returned.
//
// Return Value: The coordinates of the top-left corner of the character
// specified by /nChar/.
//
// See Also: TCEditWindow::CharFromPos, EM_POSFROTCHAR
inline POINT TCEditWindow::PosFromChar(UINT nChar) const
{
  ZAssert(::IsWindow(m_hWnd));
  POINT pt;
  ::SendMessage(m_hWnd, EM_POSFROTCHAR, WPARAM(&pt), nChar);
  return pt;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the line and character indices for the character
// closest to a specified position.
//
// Call this function to retrieve the zero-based line and character indices
// of the character nearest the specified point in this TCEditWindow control.
//
// Parameters:
//   pt - The coordinates of a point in the client area of this TCEditWindow
// object.
//
// Return Value: The character index in the low-order *WORD*, and the line
// index in the high-order *WORD*.
//
// See Also: TCEditWindow::PosFromChar, EM_CHARFROMPOS
inline int TCEditWindow::CharFromPos(POINT pt) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, EM_CHARFROMPOS, 0,
    MAKELPARAM(pt.x, pt.y));
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Subclasses the specified window.
//
// Parameters:
//   hwnd - the handle of the window to be subclassed by this object.
//
// See Also: CWindowImpl::SubclassWindow
inline const TCEditWindow& TCEditWindow::operator=(HWND hwnd)
{
  SubclassWindow(hwnd);
  return *this;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__EditWindow_h__
