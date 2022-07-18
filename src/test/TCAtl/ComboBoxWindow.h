#ifndef __ComboBoxWindow_h__
#define __ComboBoxWindow_h__

/////////////////////////////////////////////////////////////////////////////
// ComboBoxWindow.h | Declaration of the TCComboBoxWindow class.

#include <malloc.h>


/////////////////////////////////////////////////////////////////////////////
// TCComboBoxWindow is a derivation of the ATL CWindowImpl template that
// provides an encapsulation of a Windows combo box control. It is closely
// modeled after the MFC CComboBox class. You should refer to that
// documentation for any information missing from this class overview, such
// as notifications sent from the control.
//
// The TCComboBoxWindow class provides the functionality of a Windows combo
// box. A combo box consists of a list box combined with either a static
// control or edit control. The list-box portion of the control may be
// displayed at all times or may only drop down when the user selects the
// drop-down arrow next to the control.
//
// The currently selected item (if any) in the list box is displayed in the
// static or edit control. In addition, if the combo box has the drop-down
// list style, the user can type the initial character of one of the items in
// the list, and the list box, if visible, will highlight the next item with
// that initial character.
//
// The following table compares the three combo-box styles:
//
// + Style          | When is list box visible? | Static or edit control?
// + Simple         | Always                    | Edit
// + Drop-down      | When dropped down         | Edit
// + Drop-down list | When dropped down         | Static
//
// To associate a data item with each list item in a combo box, Windows
// provides the *CB_SETITEMDATA* and *CB_GETITEMDATA* messages, which are
// exposed as the SetItemDataRaw and GetItemDataRaw attributes of this class.
// The suffix /Raw/ is used to indicate that the data item associated with
// each list item is a raw, 32-bit (DWORD) value. For some uses, this may be
// exactly the type of data to be associated with the list items. Usually,
// however, a pointer to a *class*, *struct*, or *interface* is the type of
// data associated with the list items. Thus, when using the raw window
// messages or the SetItemDataRaw and GetItemDataRaw attributes of this
// class, it is necessary to type cast the data items going in and out of the
// combo box list. This is an annoying distraction from the programming task
// at hand. To alleviate this, use the TCTypedComboBox template class. This
// class takes a template parameter, /I/, that allows you to specify the
// exact type of the data items associated with the combo box list items. The
// class provides type-specific member functions for every method that refers
// to a data item. Thus, no type casting is required. Obviously, since the
// combo box is still storing 32-bit values, the specified data type is
// limited to 32-bit types. Since pointers are 32-bit in Win32, this does
// not pose a problem.
//
// For situations when the visual aspects of combo box list items (and the
// static control of a drop-down list) need to be customized, Windows
// provides a mechanism called owner-draw. This allows the parent window of a
// control to perform its own graphical manipulation of the combo box list
// items. The down side of this, however, is that since the parent window
// does the drawing, owner-draw windows are difficult to keep encapsulated
// in one C++ class. To alleviate this, the TCSelfDrawComboBox template class
// subclasses the parent window so that it can handle the owner draw messages
// being sent to the parent. The result is that a class derived from
// TCSelfDrawComboBox can easily be reused /without/ any special code being
// added to the control's parent window class. Since a control with
// customized drawing will typically need to maintain some list item state,
// TCSelfDrawComboBox derives from the TCTypedComboBox template to include
// the same type-cast free usage.
//
// See Also: TCTypedComboBox, TCSelfDrawComboBox
class TCComboBoxWindow : public CWindowImpl<TCComboBoxWindow>
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef CWindowImpl<TCComboBoxWindow> TCComboBoxWindow_Base;

// Declarations
public:
  DECLARE_WND_SUPERCLASS(NULL, TEXT("COMBOBOX"))

// Message Map
public:
  BEGIN_MSG_MAP(TCComboBoxWindow)
  END_MSG_MAP()

// Construction / Destruction
public:
  TCComboBoxWindow();
  virtual ~TCComboBoxWindow();

// Attributes
public:
  // for entire combo box
  int GetCount() const;
  int GetCurSel() const;
  int SetCurSel(int nSelect);
  DWORD GetCurSelRaw() const;
  int SetCurSelRaw(DWORD dwItemData);
  LCID GetLocale() const;
  LCID SetLocale(LCID nNewLocale);
  // Win4
  int GetTopIndex() const;
  int SetTopIndex(int nIndex);
  int InitStorage(int nItems, UINT nBytes);
  void SetHorizontalExtent(UINT nExtent);
  UINT GetHorizontalExtent() const;
  int SetDroppedWidth(UINT nWidth);
  int GetDroppedWidth() const;

  // for edit control
  DWORD GetEditSel() const;
  void GetEditSel(int& nStartChar, int& nEndChar) const;
  bool SetEditSel(int nStartChar, int nEndChar);
  bool LimitText(int nMaxChars);

  // for combobox item
  DWORD GetItemDataRaw(int nIndex) const;
  int SetItemDataRaw(int nIndex, DWORD dwItemData);
  int GetLBText(int nIndex, LPTSTR pszText) const;
  int GetLBText(int nIndex, BSTR* pbstr) const;
  int GetLBTextLen(int nIndex) const;

  int SetItemHeight(int nIndex, UINT cyItemHeight);
  int GetItemHeight(int nIndex) const;
  int SetExtendedUI(bool bExtended = true);
  bool GetExtendedUI() const;
  void GetDroppedControlRect(LPRECT lprect) const;
  bool GetDroppedState() const;

// Operations
public:
  // for drop-down combo boxes
  void ShowDropDown(bool bShowIt = true);

  // manipulating listbox items
  int AddString(LPCTSTR pszString);
  int AddStringItemRaw(LPCTSTR pszString, DWORD dwItemData);
  int DeleteString(UINT nIndex);
  int InsertString(int nIndex, LPCTSTR pszString);
  int InsertStringItemRaw(int nIndex, LPCTSTR pszString, DWORD dwItemData);
  void ResetContent();
  int Dir(UINT attr, LPCTSTR pszWildCard);

  // selection helpers
  int FindString(int nStartAfter, LPCTSTR pszString) const;
  int SelectString(int nStartAfter, LPCTSTR pszString);
  int FindStringExact(int nIndexStart, LPCTSTR pszFind) const;
  int FindItemDataRaw(int nIndexStart, DWORD dwFind) const;
  int SelectItemDataRaw(int nIndexStart, DWORD dwFind);

  // Clipboard operations
  void Clear();
  void Copy();
  void Cut();
  void Paste();

// Overrides (must override draw, measure and compare for owner draw)
public:
  virtual bool DrawItem(DRAWITEMSTRUCT* pdis);
  virtual bool MeasureItem(MEASUREITEMSTRUCT* pmis);
  virtual bool CompareItem(COMPAREITEMSTRUCT* pcis, int* pnResult);
  virtual bool DeleteItem(DELETEITEMSTRUCT* pdis);
};


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// TCTypedComboBox is a template class, derived from TCComboBoxWindow, that
// provides access to a list item's data as the specified type, /I/.
//
// For more information on when to use this class, see the overview for the
// TCComboBoxWindow class.
//
// Parameters:
//   I - The data type associated with each combo box list item. The size of
// this parameter must be less than or equal to the sizeof(DWORD).
//
// See Also: TCComboBoxWindow, TCSelfDrawComboBox
template <class I>
class TCTypedComboBox : public TCComboBoxWindow
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef TCTypedComboBox<I> TCTypedComboBox_Base;

// Construction
public:
  #ifdef _DEBUG
    TCTypedComboBox();
  #endif // _DEBUG

// Attributes
public:
  I GetItemData(int nIndex) const;
  int SetItemData(int nIndex, I itemData);
  I GetCurSelItemData() const;
  int SetCurSelItemData(I itemData);

// Operations
public:
  int AddStringItem(LPCTSTR pszString, I itemData);
  int InsertStringItem(int nIndex, LPCTSTR pszString, I itemData);
  int FindItemData(int nIndexStart, I itemData) const;
  int SelectItemData(int nIndexStart, I itemData);

// Operators
public:
  const TCTypedComboBox& operator=(HWND hwnd);
};


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// TCSelfDrawComboBox is a template class, derived from TCTypedComboBox, that
// subclasses the control's parent window so that it can process the
// owner-draw window messages and handle them in the derived class.
//
// For more information on when to use this class, see the overview for the
// TCComboBoxWindow class.
//
// Parameters:
//   T - The class which derives from TCSelfDrawComboBox.
//   I - The data type associated with each combo box list item. The size of
// this type must be less than or equal to the sizeof(DWORD).
//
// See Also: TCTypedComboBox, TCComboBoxWindow
template <class T, class I>
class TCSelfDrawComboBox : public TCTypedComboBox<I>
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef TCSelfDrawComboBox<T, I> TCSelfDrawComboBox_Base;

// Message Map
public:
  enum {e_MessageMap, e_ParentMessageMap};
  BEGIN_MSG_MAP(TCSelfDrawComboBox)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    ALT_MSG_MAP(e_ParentMessageMap)
    MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
    MESSAGE_HANDLER(WM_COMPAREITEM, OnCompareItem)
    MESSAGE_HANDLER(WM_DELETEITEM, OnDeleteItem)
    CHAIN_MSG_MAP(TCSelfDrawComboBox_Base)
  END_MSG_MAP()

// Construction
public:
  TCSelfDrawComboBox();

// Operations
public:
  BOOL SubclassWindow(HWND hWnd);

// Overrides
public:
  bool DrawItem(DRAWITEMSTRUCT* pdis, I itemData);
  bool MeasureItem(MEASUREITEMSTRUCT* pmis, I itemData);
  bool CompareItem(COMPAREITEMSTRUCT* pcis, I itemData1, I itemData2,
    int* pnResult);
  bool DeleteItem(DELETEITEMSTRUCT* pdis, I itemData);

// Message Handlers
public:
  LRESULT OnCreate(UINT, WPARAM, LPARAM lp, BOOL& bHandled);
  LRESULT OnDrawItem(UINT, WPARAM, LPARAM lp, BOOL& bHandled);
  LRESULT OnMeasureItem(UINT, WPARAM, LPARAM lp, BOOL& bHandled);
  LRESULT OnCompareItem(UINT, WPARAM, LPARAM lp, BOOL& bHandled);
  LRESULT OnDeleteItem(UINT, WPARAM, LPARAM lp, BOOL& bHandled);

// Group=Data Members
protected:
  // The object used to subclass the parent window.
  CContainedWindow m_wndParent;
};


/////////////////////////////////////////////////////////////////////////////
// TCComboBoxWindow


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

inline TCComboBoxWindow::TCComboBoxWindow()
{
}

inline TCComboBoxWindow::~TCComboBoxWindow()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the number of items in the list box of a combo box.
//
// Call this member function to retrieve the number of items in the list-box
// portion of a combo box.
//
// Return Value: The number of items. The returned count is one greater than
// the index value of the last item (the index is zero-based). It is
// *CB_ERR* if an error occurs.
//
// See Also: CB_GETCOUNT
inline int TCComboBoxWindow::GetCount() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the index of the currently selected item, if any,
// in the list box of a combo box.
//
// Call this member function to determine which item in the combo box is
// selected. GetCurSel returns an index into the list.
//
// Return Value: The zero-based index of the currently selected item in the
// list box of a combo box, or *CB_ERR* if no item is selected.
//
// See Also: TCComboBoxWindow::SetCurSel, CB_GETCURSEL
inline int TCComboBoxWindow::GetCurSel() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Selects a string in the list box of a combo box.
//
// Selects a string in the list box of a combo box. If necessary, the list
// box scrolls the string into view (if the list box is visible). The text in
// the edit control of the combo box is changed to reflect the new selection.
// Any previous selection in the list box is removed.
//
// Parameters:
//   nSelect - Specifies the zero-based index of the string to select. If –1,
// any current selection in the list box is removed and the edit control is
// cleared.
//
// Return Value: The zero-based index of the item selected if the message is
// successful. The return value is *CB_ERR* if nSelect is greater than the
// number of items in the list or if nSelect is set to –1, which clears the
// selection.
//
// See Also: TCComboBoxWindow::GetCurSel, CB_SETCURSEL
inline int TCComboBoxWindow::SetCurSel(int nSelect)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the application-supplied DWORD (32-bit) value
// associated with the selected combo-box item.
//
// Retrieves the application-supplied DWORD (32-bit) value associated with
// the selected combo-box item. The 32-bit value can be set with the
// dwItemData parameter of the SetItemDataRaw or SetCurSelRaw method calls.
//
// Return Value: The DWORD (32-bit) value associated with the selected item,
// or *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::GetCurSel, TCComboBoxWindow::GetItemDataRaw,
// TCComboBoxWindow::SetItemDataRaw, TCComboBoxWindow::SetCurSelRaw,
// TCTypedComboBox
inline DWORD TCComboBoxWindow::GetCurSelRaw() const
{
  return GetItemDataRaw(GetCurSel());
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the DWORD (32-bit) value associated with the selected
// item in a combo box.
//
// Sets the DWORD (32-bit) value associated with the selected item in a
// combo box.
//
// Parameters:
//   dwItemData - Contains the new value to associate with the selected item.
//
// Return Value: *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::GetCurSel, TCComboBoxWindow::SetItemDataRaw,
// TCComboBoxWindow::GetItemDataRaw, TCComboBoxWindow::GetCurSelRaw,
// TCTypedComboBox
inline int TCComboBoxWindow::SetCurSelRaw(DWORD dwItemData)
{
  return SetItemDataRaw(GetCurSel(), dwItemData);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the locale identifier for a combo box.
//
// Retrieves the locale used by the combo box. The locale is used, for
// example, to determine the sort order of the strings in a sorted combo box.
//
// Return Value: The locale identifier (LCID) value for the strings in the
// combo box.
//
// See Also: TCComboBoxWindow::SetLocale, ::GetStringTypeW,
// ::GetSystemDefaultLCID, ::GetUserDefaultLCID, CB_GETLOCALE
inline LCID TCComboBoxWindow::GetLocale() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the locale identifier for a combo box.
//
// Sets the locale identifier for this combo box. If SetLocale is not called,
// the default locale is obtained from the system. This system default locale
// can be modified by using Control Panel's Regional (or International)
// application.
//
// Parameters:
//   nNewLocale - The new locale identifier (LCID) value to set for the combo
// box.
//
// Return Value: The previous locale identifier (LCID) value for this combo
// box.
//
// See Also: TCComboBoxWindow::GetLocale, CB_SETLOCALE
inline LCID TCComboBoxWindow::SetLocale(LCID nNewLocale)
{
  ZAssert(::IsWindow(m_hWnd));
  return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the index of the first visible item in the list-box
// portion of the combo box.
//
// Retrieves the zero-based index of the first visible item in the list-box
// portion of the combo box. Initially, item 0 is at the top of the list box,
// but if the list box is scrolled, another item may be at the top.
//
// Return Value: The zero-based index of the first visible item in the
// list-box portion of the combo box if successful, *CB_ERR* otherwise.
//
// See Also: TCComboBoxWindow::SetTopIndex, CB_GETTOPINDEX
inline int TCComboBoxWindow::GetTopIndex() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Tells the list-box portion of the combo box to display the
// item with the specified index at the top.
//
// Ensures that a particular item is visible in the list-box portion of the
// combo box.
//
// The system scrolls the list box until either the item specified by nIndex
// appears at the top of the list box or the maximum scroll range has been
// reached.
//
// Parameters:
//   nIndex - Specifies the zero-based index of the list-box item.
//
// Return Value: Zero if successful, or *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::GetTopIndex, CB_SETTOPINDEX
inline int TCComboBoxWindow::SetTopIndex(int nIndex)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETTOPINDEX, nIndex, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Preallocates blocks of memory for items and strings in the
// list-box portion of the combo box.
//
// Allocates memory for storing list box items in the list-box portion of the
// combo box. Call this function before adding a large number of items to the
// list-box portion of the TCComboBoxWindow.
//
// Windows 95 only: The /wParam/ parameter is limited to 16-bit values. This
// means list boxes cannot contain more than 32,767 items. Although the
// number of items is restricted, the total size of the items in a list box
// is limited only by available memory.
//
// This function helps speed up the initialization of list boxes that have a
// large number of items (more than 100). It preallocates the specified
// amount of memory so that subsequent AddString, InsertString, and Dir
// functions take the shortest possible time. You can use estimates for the
// parameters. If you overestimate, some extra memory is allocated; if you
// underestimate, the normal allocation is used for items that exceed the
// preallocated amount.
//
// Parameters:
//   nItems - Specifies the number of items to add.
//   nBytes - Specifies the amount of memory, in bytes, to allocate for item
// strings.
//
// Return Value: If successful, the maximum number of items that the list-box
// portion of the combo box can store before a memory reallocation is needed,
// otherwise *CB_ERR*, meaning not enough memory is available.
//
// See Also: TCComboBoxWindow::ResetContent, CB_INITSTORAGE
inline int TCComboBoxWindow::InitStorage(int nItems, UINT nBytes)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, nBytes);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the width in pixels that the list-box portion of the
// combo box can be scrolled horizontally.
//
// Sets the width, in pixels, by which the list-box portion of the combo box
// can be scrolled horizontally. If the width of the list box is smaller than
// this value, the horizontal scroll bar will horizontally scroll items in
// the list box. If the width of the list box is equal to or greater than
// this value, the horizontal scroll bar is hidden or, if the combo box has
// the *CBS_DISABLENOSCROLL* style, disabled.
//
// Parameters:
//   nExtent - Specifies the number of pixels by which the list-box portion
// of the combo box can be scrolled horizontally.
//
// See Also: TCComboBoxWindow::GetHorizontalExtent, CB_SETHORIZONTALEXTENT
inline void TCComboBoxWindow::SetHorizontalExtent(UINT nExtent)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, nExtent, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the width in pixels that the list-box portion of the
// combo box can be scrolled horizontally.
//
// Retrieves from the combo box the width in pixels by which the list-box
// portion of the combo box can be scrolled horizontally. This is applicable
// only if the list-box portion of the combo box has a horizontal scroll bar.
//
// Return Value: The scrollable width of the list-box portion of the combo
// box, in pixels.
//
// See Also: TCComboBoxWindow::SetHorizontalExtent, CB_GETHORIZONTALEXTENT
inline UINT TCComboBoxWindow::GetHorizontalExtent() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (UINT)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the minimum allowable width for the drop-down list-box
// portion of a combo box.
//
// Call this function to set the minimum allowable width, in pixels, of the
// list box portion of a combo box. This function only applies to combo boxes
// with the *CBS_DROPDOWN* or *CBS_DROPDOWNLIST* style.
//
// By default, the minimum allowable width of the drop-down list box is 0.
// When the list-box portion of the combo box is displayed, its width is the
// larger of the minimum allowable width or the combo box width.
//
// Parameters:
//   nWidth - The minimum allowable width of the list-box portion of the
// combo box, in pixels.
//
// Return Value: If successful, the new width of the list box, otherwise
// *CB_ERR*.
//
// See Also: TCComboBoxWindow::GetDroppedWidth, CB_SETDROPPEDWIDTH
inline int TCComboBoxWindow::SetDroppedWidth(UINT nWidth)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, nWidth, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the minimum allowable width for the drop-down
// list-box portion of a combo box.
//
// Call this function to retrieve the minimum allowable width, in pixels, of
// the list box portion of a combo box. This function only applies to combo
// boxes with the *CBS_DROPDOWN* or *CBS_DROPDOWNLIST* style.
//
// By default, the minimum allowable width of the drop-down list box is 0.
// The minimum allowable width can be set by calling SetDroppedWidth. When
// the list-box portion of the combo box is displayed, its width is the
// larger of the minimum allowable width or the combo box width.
//
// Return Value: If successful, the minimum allowable width, in pixels;
// otherwise, *CB_ERR*.
//
// See Also: TCComboBoxWindow::SetDroppedWidth, CB_GETDROPPEDWIDTH
inline int TCComboBoxWindow::GetDroppedWidth() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the starting and ending character positions of the
// current selection in the edit control of a combo box.
//
// Gets the starting and ending character positions of the current selection
// in the edit control of a combo box.
//
// Return Value: A 32-bit value that contains the starting position in the
// low-order word and the position of the first nonselected character after
// the end of the selection in the high-order word. If this function is used
// on a combo box without an edit control, CB_ERR is returned.
//
// See Also: TCComboBoxWindow::SetEditSel, CB_GETEDITSEL 
inline DWORD TCComboBoxWindow::GetEditSel() const
{
  ZAssert(::IsWindow(m_hWnd));
  return ::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   nStartChar - A reference to an int variable that will receive the
// starting position of the edit selection.
//   nEndChar - A reference to an int variable that will receive the ending
// position of the edit selection.
inline void TCComboBoxWindow::GetEditSel(int& nStartChar, int& nEndChar)
  const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, CB_GETEDITSEL,(WPARAM)&nStartChar,(LPARAM)&nEndChar);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Selects characters in the edit control of a combo box.
//
// Selects characters in the edit control of a combo box. 
//
// The positions are zero-based. To select the first character of the edit
// control, you specify a starting position of 0. The ending position is for
// the character just after the last character to select. For example, to
// select the first four characters of the edit control, you would use a
// starting position of 0 and an ending position of 4.
//
// Parameters:
//   nStartChar - Specifies the starting position. If the starting position
// is set to –1, then any existing selection is removed.
//   nEndChar - Specifies the ending position. If the ending position is set
// to –1, then all text from the starting position to the last character in
// the edit control is selected.
//
// Return Value: true if the member function is successful; otherwise false.
inline bool TCComboBoxWindow::SetEditSel(int nStartChar, int nEndChar)
{
  ZAssert(::IsWindow(m_hWnd));
  return 0 != ::SendMessage(m_hWnd, CB_SETEDITSEL, 0,
    MAKELONG(nStartChar, nEndChar));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Limits the length of the text that the user can enter into
// the edit control of a combo box.
//
// Limits the length in bytes of the text that the user can enter into the
// edit control of a combo box. 
//
// If the combo box does not have the style *CBS_AUTOHSCROLL*, setting the
// text limit to be larger than the size of the edit control will have no
// effect. 
//
// LimitText only limits the text the user can enter. It has no effect on any
// text already in the edit control when the message is sent, nor does it
// affect the length of the text copied to the edit control when a string in
// the list box is selected.
//
// Parameters:
//   nMaxChars - Specifies the length (in bytes) of the text that the user
// can enter. If this parameter is 0, the text length is set to 65,535 bytes.
//
// Return Value: true if the member function is successful; otherwise false.
inline bool TCComboBoxWindow::LimitText(int nMaxChars)
{
  ZAssert(::IsWindow(m_hWnd));
  return 0 != ::SendMessage(m_hWnd, CB_LIMITTEXT, nMaxChars, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the application-supplied DWORD (32-bit) value
// associated with the specified combo-box item.
//
// Retrieves the application-supplied DWORD (32-bit) value associated with
// the specified combo-box item. The 32-bit value can be set with the
// dwItemData parameter of the SetItemDataRaw or SetCurSelRaw method calls.
//
// Parameters:
//   nIndex - Contains the zero-based index of an item in the combo box's
// list box.
//
// Return Value: The DWORD (32-bit) value associated with the specified item,
// or *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::GetCurSelRaw,
// TCComboBoxWindow::SetItemDataRaw, TCComboBoxWindow::SetCurSelRaw,
// TCTypedComboBox
inline DWORD TCComboBoxWindow::GetItemDataRaw(int nIndex) const
{
  ZAssert(::IsWindow(m_hWnd));
  return ::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the DWORD (32-bit) value associated with the specified
// item in a combo box.
//
// Sets the DWORD (32-bit) value associated with the specified item in a
// combo box.
//
// Parameters:
//   nIndex - Contains a zero-based index to the item to set.
//   dwItemData - Contains the new value to associate with the specified
// item.
//
// Return Value: *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::SetCurSelRaw,
// TCComboBoxWindow::GetItemDataRaw, TCComboBoxWindow::GetCurSelRaw,
// TCTypedComboBox
inline int TCComboBoxWindow::SetItemDataRaw(int nIndex, DWORD dwItemData)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex,
    (LPARAM)dwItemData);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets a string from the list box of a combo box.
//
// Gets a string from the list box of a combo box. The second form of this
// member function creates a BSTR object with the item's text.
//
// Parameters:
//   nIndex - Contains the zero-based index of the list-box string to be
// copied.
//   pszText - Points to a buffer that is to receive the string. The buffer
// must have sufficient space for the string and a terminating null
// character.
//
// Return Value: The length (in bytes) of the string, excluding the
// terminating null character. If /nIndex/ does not specify a valid index,
// the return value is *CB_ERR*.
//
// See Also: TCComboBoxWindow::GetLBTextLen, CB_GETLBTEXT
inline int TCComboBoxWindow::GetLBText(int nIndex, LPTSTR pszText) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)pszText);
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   pbstr - The address of a BSTR variable to receive the text string of the
// specified list item. The caller must free this BSTR using ::SysFreeString
// when it no longer needs it.
inline int TCComboBoxWindow::GetLBText(int nIndex, BSTR* pbstr) const
{
  ZAssert(::IsWindow(m_hWnd));
  *pbstr = NULL;

  int cchLine = GetLBTextLen(nIndex);
  if (!cchLine)
    return 0;

  LPTSTR psz = (LPTSTR)_alloca(cchLine + sizeof(TCHAR));
  int nResult = GetLBText(nIndex, psz);
  psz[nResult] = TEXT('\0');

  USES_CONVERSION;
  *pbstr = SysAllocString(T2OLE(psz));

  return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the length of a string in the list box of a combo box.
//
// Gets the length of a string in the list box of a combo box.
//
// Parameters:
//   nIndex - Contains the zero-based index of the list-box string.
//
// Return Value: The length of the string in characters, excluding the
// terminating null character. If /nIndex/ does not specify a valid index,
// the return value is *CB_ERR*.
//
// See Also: TCComboBoxWindow::GetLBText, CB_GETLBTEXTLEN
inline int TCComboBoxWindow::GetLBTextLen(int nIndex) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the height of list items in a combo box or the height of
// the edit-control (or static-text) portion of a combo box.
//
// Call the SetItemHeight member function to set the height of list items in
// a combo box or the height of the edit-control (or static-text) portion of
// a combo box.
//
// The height of the edit-control (or static-text) portion of the combo box
// is set independently of the height of the list items. An application must
// ensure that the height of the edit-control (or static-text) portion is not
// smaller than the height of a particular list-box item.
//
// Parameters:
//   nIndex - Specifies whether the height of list items or the height of the
// edit-control (or static-text) portion of the combo box is set. If the
// combo box has the *CBS_OWNERDRAWVARIABLE* style, /nIndex/ specifies the
// zero-based index of the list item whose height is to be set; otherwise,
// /nIndex/ must be 0 and the height of all list items will be set. If
// /nIndex/ is –1, the height of the edit-control or static-text portion of
// the combo box is to be set.
//   cyItemHeight - Specifies the height, in pixels, of the combo-box
// component identified by nIndex.
//
// Return Value: *CB_ERR* if the index or height is invalid; otherwise 0.
//
// See Also: TCComboBoxWindow::GetItemHeight, WM_MEASUREITEM,
// CB_SETITEMHEIGHT
inline int TCComboBoxWindow::SetItemHeight(int nIndex, UINT cyItemHeight)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex,
    MAKELONG(cyItemHeight, 0));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the height of list items in a combo box.
//
// Call the GetItemHeight member function to retrieve the height of list
// items in a combo box.
//
// Parameters:
//   nIndex - Specifies the component of the combo box whose height is to be
// retrieved. If the /nIndex/ parameter is –1, the height of the edit-control
// (or static-text) portion of the combo box is retrieved. If the combo box
// has the *CBS_OWNERDRAWVARIABLE* style, /nIndex/ specifies the zero-based
// index of the list item whose height is to be retrieved. Otherwise,
// /nIndex/ should be set to 0.
//
// Return Value: The height, in pixels, of the specified item in a combo box.
// The return value is *CB_ERR* if an error occurs.
//
// See Also: TCComboBoxWindow::SetItemHeight, WM_MEASUREITEM,
// CB_GETITEMHEIGHT 
inline int TCComboBoxWindow::GetItemHeight(int nIndex) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Selects either the default user interface or the extended
// user interface for a combo box that has the *CBS_DROPDOWN* or
// *CBS_DROPDOWNLIST* style.
//
// Call the SetExtendedUI member function to select either the default user
// interface or the extended user interface for a combo box that has the
// *CBS_DROPDOWN* or *CBS_DROPDOWNLIST* style. 
//
// The extended user interface can be identified in the following ways:
//
// + Clicking the static control displays the list box only for combo boxes
// with the *CBS_DROPDOWNLIST* style.
// + Pressing the DOWN ARROW key displays the list box (F4 is disabled).
//
// Scrolling in the static control is disabled when the item list is not
// visible (the arrow keys are disabled).
//
// Parameters:
//   bExtended - Specifies whether the combo box should use the extended user
// interface or the default user interface. A value of *true* selects the
// extended user interface; a value of *false* selects the standard user
// interface.
//
// Return Value: *CB_OKAY* if the operation is successful, or *CB_ERR* if an
// error occurs.
//
// See Also: TCComboBoxWindow::GetExtendedUI, CB_SETEXTENDEDUI
inline int TCComboBoxWindow::SetExtendedUI(bool bExtended )
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, bExtended, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines whether a combo box has the default user interface
// or the extended user interface.
//
// Call the GetExtendedUI member function to determine whether a combo box
// has the default user interface or the extended user interface. The
// extended user interface can be identified in the following ways:
// + Clicking the static control displays the list box only for combo boxes
// with the *CBS_DROPDOWNLIST* style.
// + Pressing the DOWN ARROW key displays the list box (F4 is disabled). 
//
// Scrolling in the static control is disabled when the item list is not
// visible (arrow keys are disabled).
//
// Return Value: *true* if the combo box has the extended user interface;
// otherwise *false*.
//
// See Also: TCComboBoxWindow::SetExtendedUI, CB_GETEXTENDEDUI
inline bool TCComboBoxWindow::GetExtendedUI() const
{
  ZAssert(::IsWindow(m_hWnd));
  return 0 != ::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the screen coordinates of the visible
// (dropped-down) list box of a drop-down combo box.
//
// Call the GetDroppedControlRect member function to retrieve the screen
// coordinates of the visible (dropped-down) list box of a drop-down combo
// box.
//
// Parameters:
//   prect - Points to the RECT structure that is to receive the coordinates.
//
// See Also: CB_GETDROPPEDCONTROLRECT
inline void TCComboBoxWindow::GetDroppedControlRect(LPRECT prect) const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (DWORD)prect);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines whether the list box of a drop-down combo box is
// visible (dropped down).
//
// Call the GetDroppedState member function to determine whether the list box
// of a drop-down combo box is visible (dropped down).
//
// Return Value: *true* if the list box is visible; otherwise *false*.
//
// See Also: TCComboBoxWindow::ShowDropDown, CB_SHOWDROPDOWN,
// CB_GETDROPPEDSTATE
inline bool TCComboBoxWindow::GetDroppedState() const
{
  ZAssert(::IsWindow(m_hWnd));
  return 0 != ::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Shows or hides the list box of a combo box that has the
// *CBS_DROPDOWN* or *CBS_DROPDOWNLIST* style.
//
// Shows or hides the list box of a combo box that has the *CBS_DROPDOWN* or
// *CBS_DROPDOWNLIST* style. By default, a combo box of this style will show
// the list box.
//
// This member function has no effect on a combo box created with the
// *CBS_SIMPLE* style.
//
// Parameters:
//   bShowIt - Specifies whether the drop-down list box is to be shown or
// hidden. A value of *true* shows the list box. A value of *false* hides the
// list box.
//
// See Also: TCComboBoxWindow::GetDroppedState, CB_SHOWDROPDOWN 
inline void TCComboBoxWindow::ShowDropDown(bool bShowIt)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, CB_SHOWDROPDOWN, bShowIt, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Adds a string to the end of the list in the list box of a
// combo box or at the sorted position for list boxes with the
// *CBS_SORT* style.
//
// Adds a string to the list box of a combo box. If the list box was not
// created with the *CBS_SORT* style, the string is added to the end of the
// list. Otherwise, the string is inserted into the list, and the list is
// sorted. 
//
// To insert a string into a specific location within the list, use the
// InsertString member function.
//
// Parameters:
//   pszString - Points to the null-terminated string that is to be added.
//
// Return Value: If the return value is greater than or equal to 0, it is the
// zero-based index to the string in the list box. The return value is
// *CB_ERR* if an error occurs; the return value is *CB_ERRSPACE* if
// insufficient space is available to store the new string.
//
// See Also: TCComboBoxWindow::AddStringItemRaw,
// TCComboBoxWindow::InsertString, TCComboBoxWindow::InsertStringItemRaw,
// TCComboBoxWindow::DeleteString, CB_ADDSTRING 
inline int TCComboBoxWindow::AddString(LPCTSTR pszString)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)pszString);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Adds a string to the end of the list in the list box of a
// combo box or at the sorted position for list boxes with the
// *CBS_SORT* style. The specified DWORD (32-bit) value is then associated
// with the list item.
//
// Adds a string to the list box of a combo box and associates the specified
// DWORD (32-bit) value with the item. If the list box was not created with
// the *CBS_SORT* style, the string is added to the end of the list.
// Otherwise, the string is inserted into the list, and the list is sorted. 
//
// To insert a string into a specific location within the list, use the
// InsertStringItemRaw member function.
//
// Parameters:
//   pszString - Points to the null-terminated string that is to be added.
//   dwItemData - Contains the value to associate with the added string.
//
// Return Value: If the return value is greater than or equal to 0, it is the
// zero-based index to the string in the list box. The return value is
// *CB_ERR* if an error occurs; the return value is *CB_ERRSPACE* if
// insufficient space is available to store the new string.
//
// See Also: TCComboBoxWindow::AddStringItemRaw,
// TCComboBoxWindow::InsertString, TCComboBoxWindow::InsertStringItemRaw,
// TCComboBoxWindow::DeleteString, TCTypedComboBox::AddStringItem,
// TCTypedComboBox, CB_ADDSTRING, CB_SETITEMDATA
inline int TCComboBoxWindow::AddStringItemRaw(LPCTSTR pszString,
  DWORD dwItemData)
{
  ZAssert(::IsWindow(m_hWnd));
  int i = AddString(pszString);
  return (i >= 0 && SetItemDataRaw(i, dwItemData) >= 0) ? i : CB_ERR;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Deletes a string from the list box of a combo box.
//
// Deletes a string in the list box of a combo box.
//
// Parameters:
//   nIndex - Specifies the index to the string that is to be deleted.
//
// Return Value: If the return value is greater than or equal to 0, then it
// is a count of the strings remaining in the list. The return value is
// *CB_ERR* if nIndex specifies an index greater then the number of items in
// the list.
//
// See Also: TCComboBoxWindow::InsertString,
// TCComboBoxWindow::InsertStringItemRaw, TCComboBoxWindow::AddString,
// TCComboBoxWindow::AddStringItemRaw, CB_DELETESTRING 
inline int TCComboBoxWindow::DeleteString(UINT nIndex)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Inserts a string into the list box of a combo box.
//
// Inserts a string into the list box of a combo box. Unlike the AddString
// member function, the InsertString member function does not cause a list
// with the *CBS_SORT* style to be sorted.
//
// Parameters:
//   nIndex - Contains the zero-based index to the position in the list box
// that will receive the string. If this parameter is –1, the string is
// added to the end of the list.
//   pszString - Points to the null-terminated string that is to be inserted.
//
// Return Value: The zero-based index of the position at which the string was
// inserted. The return value is *CB_ERR* if an error occurs. The return
// value is *CB_ERRSPACE* if insufficient space is available to store the new
// string.
//
// See Also: TCComboBoxWindow::InsertStringItemRaw,
// TCComboBoxWindow::AddString, TCComboBoxWindow::DeleteString,
// TCComboBoxWindow::ResetContent, CB_INSERTSTRING 
inline int TCComboBoxWindow::InsertString(int nIndex, LPCTSTR pszString)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndex,
    (LPARAM)pszString);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Inserts a string into the list box of a combo box. The
// specified DWORD (32-bit) value is then associated with the list item.
//
// Inserts a string into the list box of a combo box and associates the
// specified DWORD (32-bit) value with the item. Unlike the AddString
// member function, the InsertString member function does not cause a list
// with the *CBS_SORT* style to be sorted.
//
// Parameters:
//   nIndex - Contains the zero-based index to the position in the list box
// that will receive the string. If this parameter is –1, the string is
// added to the end of the list.
//   pszString - Points to the null-terminated string that is to be inserted.
//   dwItemData - Contains the value to associate with the inserted string.
//
// Return Value: The zero-based index of the position at which the string was
// inserted. The return value is *CB_ERR* if an error occurs. The return
// value is *CB_ERRSPACE* if insufficient space is available to store the new
// string.
//
// See Also: TCComboBoxWindow::InsertString, TCComboBoxWindow::AddString,
// TCComboBoxWindow::DeleteString, TCComboBoxWindow::ResetContent,
// TCTypedComboBox, CB_INSERTSTRING 
inline int TCComboBoxWindow::InsertStringItemRaw(int nIndex,
  LPCTSTR pszString, DWORD dwItemData)
{
  ZAssert(::IsWindow(m_hWnd));
  int i = InsertString(nIndex, pszString);
  return (i >= 0 && SetItemDataRaw(i, dwItemData) >= 0) ? i : CB_ERR;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Removes all items from the list box and edit control of a
// combo box.
//
// Removes all items from the list box and edit control of a combo box.
//
// See Also: CB_RESETCONTENT
inline void TCComboBoxWindow::ResetContent()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Adds a list of filenames to the list box of a combo box.
//
// Adds a list of filenames and/or drives to the list box of a combo box. The
// /attr/ parameter can be any combination of the following values:
//
//   DDL_READWRITE - File can be read from or written to.
//   DDL_READONLY - File can be read from but not written to.
//   DDL_HIDDEN - File is hidden and does not appear in a directory listing.
//   DDL_SYSTEM - File is a system file.
//   DDL_DIRECTORY - The name specified by lpszWildCard specifies a
// directory.
//   DDL_ARCHIVE - File has been archived.
//   DDL_DRIVES - Include all drives that match the name specified by
// /pszWildCard/.
//   DDL_EXCLUSIVE - Exclusive flag. If the exclusive flag is set, only files
// of the specified type are listed. Otherwise, files of the specified type
// are listed in addition to "normal" files.
//
// Parameters:
//   attr - Specifies the attributes of the files that should be listed in
// the combo box. See the remarks for the possible values of this parameter.
//   pszWildCard - Points to a file-specification string. The string can
// contain wildcards (for example, *.*).
//
// Return Value: If the return value is greater than or equal to 0, it is the
// zero-based index of the last filename added to the list. The return value
// is *CB_ERR* if an error occurs; the return value is *CB_ERRSPACE* if
// insufficient space is available to store the new strings.
//
// See Also: CB_DIR
inline int TCComboBoxWindow::Dir(UINT attr, LPCTSTR pszWildCard)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_DIR, attr, (LPARAM)pszWildCard);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Finds the first string that contains the specified prefix in
// the list box of a combo box.
//
// Finds, but doesn't select, the first string that contains the specified
// prefix in the list box of a combo box.
//
// To find an item based on it's associated DWORD (32-bit) value, use the
// FindItemDataRaw method.
//
// Parameters:
//   nStartAfter - Contains the zero-based index of the item before the first
// item to be searched. When the search reaches the bottom of the list box,
// it continues from the top of the list box back to the item specified by
// /nStartAfter/. If –1, the entire list box is searched from the beginning.
//   pszString - Points to the null-terminated string that contains the
// prefix to search for. The search is case independent, so this string can
// contain any combination of uppercase and lowercase letters.
//
// Return Value: If the return value is greater than or equal to 0, it is the
// zero-based index of the matching item. It is *CB_ERR* if the search was
// unsuccessful.
//
// See Also: TCComboBoxWindow::FindStringExact,
// TCComboBoxWindow::FindItemDataRaw, TCComboBoxWindow::SelectItemDataRaw,
// TCComboBoxWindow::SelectString, TCComboBoxWindow::SetCurSel, CB_FINDSTRING
inline int TCComboBoxWindow::FindString(int nStartAfter, LPCTSTR pszString)
  const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_FINDSTRING, nStartAfter,
    (LPARAM)pszString);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Searches for a string in the list box of a combo box and, if
// the string is found, selects the string in the list box and copies the
// string to the edit control.
//
// Searches for a string in the list box of a combo box, and if the string
// is found, selects the string in the list box and copies it to the edit
// control. 
//
// A string is selected only if its initial characters (from the starting
// point) match the characters in the prefix string. 
//
// Note that the SelectString and FindString member functions both find a
// string, but the SelectString member function also selects the string.
//
// To find and select an item based on it's associated DWORD (32-bit) value,
// use the SelectItemDataRaw method.
//
// Parameters:
//   nStartAfter - Contains the zero-based index of the item before the first
// item to be searched. When the search reaches the bottom of the list box,
// it continues from the top of the list box back to the item specified by
// /nStartAfter/. If –1, the entire list box is searched from the beginning.
//   pszString - Points to the null-terminated string that contains the
// prefix to search for. The search is case independent, so this string can
// contain any combination of uppercase and lowercase letters.
//
// Return Value: The zero-based index of the selected item if the string was
// found. If the search was unsuccessful, the return value is *CB_ERR* and
// the current selection is not changed.
//
// See Also: TCComboBoxWindow::FindString, TCComboBoxWindow::FindStringExact,
// TCComboBoxWindow::FindItemDataRaw, TCComboBoxWindow::SelectItemDataRaw,
// CB_SELECTSTRING 
inline int TCComboBoxWindow::SelectString(int nStartAfter, LPCTSTR pszString)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_SELECTSTRING, nStartAfter,
    (LPARAM)pszString);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Finds the first list-box string (in a combo box) that matches
// the specified string.
//
// Call the FindStringExact member function to find the first list-box string
// (in a combo box) that matches the string specified in pszFind.
//
// If the combo box was created with an owner-draw style but without the
// *CBS_HASSTRINGS* style, FindStringExact attempts to match the DWORD
// (32-bit) value against the value of pszFind. A more convenient way to
// search for an associated DWORD (32-bit) value it to use the
// FindItemDataRaw method.
//
// Parameters:
//   nIndexStart - Specifies the zero-based index of the item before the
// first item to be searched. When the search reaches the bottom of the list
// box, it continues from the top of the list box back to the item specified
// by /nIndexStart/. If /nIndexStart/ is –1, the entire list box is searched
// from the beginning.
//   pszFind - Points to the null-terminated string to search for. This
// string can contain a complete filename, including the extension. The
// search is not case sensitive, so this string can contain any combination
// of uppercase and lowercase letters.
//
// Return Value: The zero-based index of the matching item, or *CB_ERR* if
// the search was unsuccessful.
//
// See Also: TCComboBoxWindow::FindString, TCComboBoxWindow::FindItemDataRaw,
// TCComboBoxWindow::SelectItemDataRaw, CB_FINDSTRINGEXACT 
inline int TCComboBoxWindow::FindStringExact(int nIndexStart,
  LPCTSTR pszFind) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, nIndexStart,
    (LPARAM)pszFind);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Finds the first list-box string (in a combo box) that matches
// the specified DWORD (32-bit) value.
//
// Searches for the first list-box item (in a combo box) that has an
// associated DWORD (32-bit) value equal to the one specified in the
// /dwFind/ parameter.
//
// TODO: Perhaps this should be modified so that the /nIndexStart/ parameter
// acts the same as it does in the other searching methods.
//
// Parameters:
//   nIndexStart - Specifies the zero-based index of the first item in the
// list box to be searched.
//   dwFind - The DWORD (32-bit) value to find in the list box.
//
// Return Value: The zero-based index of the matching item, or -1 if the
// search was unsuccessful.
//
// See Also: TCComboBoxWindow::SelectItemDataRaw, TCTypedComboBox
inline int TCComboBoxWindow::FindItemDataRaw(int nIndexStart, DWORD dwFind)
  const
{
  ZAssert(::IsWindow(m_hWnd));
  int nCount = GetCount();
  for (int i = nIndexStart; i < nCount; ++i)
    if (GetItemDataRaw(i) == dwFind)
      return i;
  return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Searches for the specified DWORD (32-bit) value in the list
// box of a combo box and, if the item is found, selects the string in the
// list box and copies the string to the edit control.
//
// Searches for the first list-box item (in a combo box) that has an
// associated DWORD (32-bit) value equal to the one specified in the
// /dwFind/ parameter and selects it.
//
// TODO: Perhaps this should be modified so that the /nIndexStart/ parameter
// acts the same as it does in the other searching methods.
//
// Parameters:
//   nIndexStart - Specifies the zero-based index of the first item in the
// list box to be searched.
//   dwFind - The DWORD (32-bit) value to find in the list box.
//
// Return Value: The zero-based index of the matching item, or -1 if the
// search was unsuccessful, and the selection is not changed.
//
// See Also: TCComboBoxWindow::FindItemDataRaw, TCTypedComboBox
inline int TCComboBoxWindow::SelectItemDataRaw(int nIndexStart, DWORD dwFind)
{
  ZAssert(::IsWindow(m_hWnd));
  return SetCurSel(FindItemDataRaw(nIndexStart, dwFind));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Deletes (clears) the current selection (if any) in the edit
// control.
//
// Deletes (clears) the current selection, if any, in the edit control of the
// combo box. 
//
// To delete the current selection and place the deleted contents onto the
// Clipboard, use the Cut member function.
//
// See Also: TCComboBoxWindow::Copy, TCComboBoxWindow::Cut,
// TCComboBoxWindow::Paste, WM_CLEAR
inline void TCComboBoxWindow::Clear()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_CLEAR, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Copies the current selection (if any) onto the Clipboard in
// *CF_TEXT* format.
//
// Copies the current selection, if any, in the edit control of the combo box
// onto the Clipboard in *CF_TEXT* format.
//
// See Also: TCComboBoxWindow::Clear, TCComboBoxWindow::Cut,
// TCComboBoxWindow::Paste, WM_COPY
inline void TCComboBoxWindow::Copy()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_COPY, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Deletes (cuts) the current selection, if any, in the edit
// control and copies the deleted text onto the Clipboard in
// *CF_TEXT* format.
//
// Deletes (cuts) the current selection, if any, in the combo-box edit
// control and copies the deleted text onto the Clipboard in
// *CF_TEXT* format.
//
// To delete the current selection without placing the deleted text onto the
// Clipboard, call the Clear member function.
//
// See Also: TCComboBoxWindow::Clear, TCComboBoxWindow::Copy,
// TCComboBoxWindow::Paste, WM_CUT
inline void TCComboBoxWindow::Cut()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_CUT, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Inserts the data from the Clipboard into the edit control at
// the current cursor position. Data is inserted only if the Clipboard
// contains data in *CF_TEXT* format.
//
// Inserts the data from the Clipboard into the edit control of the combo box
// at the current cursor position. Data is inserted only if the Clipboard
// contains data in *CF_TEXT* format.
//
// See Also: TCComboBoxWindow::Clear, TCComboBoxWindow::Copy,
// TCComboBoxWindow::Cut, WM_PASTE
inline void TCComboBoxWindow::Paste()
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, WM_PASTE, 0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// {secret}
// TODO: Since this does not appear to be used, it should be removed. For any
// owner-draw functionality, use TCSelfDrawComboBox.
inline bool TCComboBoxWindow::DrawItem(DRAWITEMSTRUCT* pdis)
{
  ZAssert(false);
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// {secret}
// TODO: Since this does not appear to be used, it should be removed. For any
// owner-draw functionality, use TCSelfDrawComboBox.
inline bool TCComboBoxWindow::MeasureItem(MEASUREITEMSTRUCT* pmis)
{
  ZAssert(false);
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// {secret}
// TODO: Since this does not appear to be used, it should be removed. For any
// owner-draw functionality, use TCSelfDrawComboBox.
inline bool TCComboBoxWindow::CompareItem(COMPAREITEMSTRUCT* pcis,
  int* pnResult)
{
  ZAssert(false);
  *pnResult = 0;
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// {secret}
// TODO: Since this does not appear to be used, it should be removed. For any
// owner-draw functionality, use TCSelfDrawComboBox.
inline bool TCComboBoxWindow::DeleteItem(DELETEITEMSTRUCT* pdis)
{
  // Does nothing by default
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// TCTypedComboBox

/////////////////////////////////////////////////////////////////////////////
// Group=Construction

#ifdef _DEBUG
  ///////////////////////////////////////////////////////////////////////////
  // Remarks: Under _DEBUG builds, the template parameter, /I/, is tested to
  // ensure that its size is less than or equal to sizeof(DWORD). If not,
  // the ZAssert macro will indicate this.
  template <class I>
  inline TCTypedComboBox<I>::TCTypedComboBox()
  {
    ZAssert(sizeof(I) <= sizeof(DWORD));
  }
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the typed application-supplied value associated
// with the specified combo-box item.
//
// Retrieves the application-supplied typed  value associated with the
// specified combo-box item. The 32-bit value can be set with the itemData
// parameter of the SetItemData or SetCurSelItemData method calls.
//
// Parameters:
//   nIndex - Contains the zero-based index of an item in the combo box's
// list box.
//
// Return Value: The typed value associated with the specified item, or
// *CB_ERR* if an error occurs.
//
// See Also: TCTypedComboBox::SetItemData,
// TCTypedComboBox::GetCurSelItemData, TCTypedComboBox::SetCurSelItemData,
// TCComboBoxWindow::GetItemDataRaw, TCComboBoxWindow
template <class I>
inline I TCTypedComboBox<I>::GetItemData(int nIndex) const
{
  return (I)(GetItemDataRaw(nIndex));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the typed value associated with the specified item in a
// combo box.
//
// Sets the typed value associated with the specified item in a combo box.
//
// Parameters:
//   nIndex - Contains a zero-based index of the item to set.
//   itemData - Contains the new value to associate with the specified item.
//
// Return Value: *CB_ERR* if an error occurs.
//
// See Also: TCTypedComboBox::GetItemData,
// TCTypedComboBox::SetCurSelItemData, TCTypedComboBox::GetCurSelItemData,
// TCComboBoxWindow::SetItemDataRaw, TCComboBoxWindow
template <class I>
inline int TCTypedComboBox<I>::SetItemData(int nIndex, I itemData)
{
  return SetItemDataRaw(nIndex, DWORD(itemData));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the typed application-supplied value associated
// with the selected combo-box item.
//
// Retrieves the application-supplied typed  value associated with the
// selected combo-box item. The 32-bit value can be set with the itemData
// parameter of the SetItemData or SetCurSelItemData method calls.
//
// Return Value: The typed value associated with the selected item, or
// *CB_ERR* if an error occurs.
//
// See Also: TCTypedComboBox::SetCurSelItemData,
// TCTypedComboBox::GetItemData, TCTypedComboBox::SetItemData,
// TCComboBoxWindow::GetCurSelRaw, TCComboBoxWindow
template <class I>
inline I TCTypedComboBox<I>::GetCurSelItemData() const
{
  return (I)GetCurSelRaw();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the typed value associated with the selected item in a
// combo box.
//
// Sets the typed value associated with the selected item in a combo box.
//
// Parameters:
//   itemData - Contains the new value to associate with the selected item.
//
// Return Value: *CB_ERR* if an error occurs.
//
// See Also: TCTypedComboBox::GetCurSelItemData,
// TCTypedComboBox::SetItemData, TCTypedComboBox::GetItemData,
// TCComboBoxWindow::GetItemDataRaw, TCComboBoxWindow
template <class I>
inline int TCTypedComboBox<I>::SetCurSelItemData(I itemData)
{
  return SetCurSelRaw((DWORD)itemData);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Adds a string to the end of the list in the list box of a
// combo box or at the sorted position for list boxes with the
// *CBS_SORT* style. The specified typed value is then associated with the
// list item.
//
// Adds a string to the list box of a combo box and associates the specified
// typed value with the item. If the list box was not created with the
// *CBS_SORT* style, the string is added to the end of the list. Otherwise
// the string is inserted into the list, and the list is sorted. 
//
// To insert a string into a specific location within the list, use the
// InsertStringItem member function.
//
// Parameters:
//   pszString - Points to the null-terminated string that is to be added.
//   itemData - Contains the typed value to associate with the added string.
//
// Return Value: If the return value is greater than or equal to 0, it is the
// zero-based index to the string in the list box. The return value is
// *CB_ERR* if an error occurs; the return value is *CB_ERRSPACE* if
// insufficient space is available to store the new string.
//
// See Also: TCTypedComboBox::InsertStringItem,
// TCComboBoxWindow::AddStringItemRaw, TCComboBoxWindow::AddString,
// TCComboBoxWindow::DeleteString, TCComboBoxWindow, CB_ADDSTRING,
// CB_SETITEMDATA
template <class I>
inline int TCTypedComboBox<I>::AddStringItem(LPCTSTR pszString, I itemData)
{
  return AddStringItemRaw(pszString, DWORD(itemData));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Inserts a string into the list box of a combo box. The
// specified typed value is then associated with the list item.
//
// Inserts a string into the list box of a combo box and associates the
// specified typed value with the item. Unlike the AddStringItem member
// function, the InsertStringItem member function does not cause a list
// with the *CBS_SORT* style to be sorted.
//
// Parameters:
//   nIndex - Contains the zero-based index to the position in the list box
// that will receive the string. If this parameter is –1, the string is
// added to the end of the list.
//   pszString - Points to the null-terminated string that is to be inserted.
//   itemData - Contains the typed value to associate with the inserted
// string.
//
// Return Value: The zero-based index of the position at which the string was
// inserted. The return value is *CB_ERR* if an error occurs. The return
// value is *CB_ERRSPACE* if insufficient space is available to store the new
// string.
//
// See Also: TCTypedComboBox::AddStringItem, TCComboBoxWindow::InsertString,
// TCComboBoxWindow::DeleteString, TCComboBoxWindow::ResetContent,
// TCComboBoxWindow, CB_INSERTSTRING, CB_SETITEMDATA
template <class I>
inline int TCTypedComboBox<I>::InsertStringItem(int nIndex,
  LPCTSTR pszString, I itemData)
{
  return InsertStringItemRaw(nIndex, pszString, DWORD(itemData));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Finds the first list-box string (in a combo box) that matches
// the specified typed value.
//
// Searches for the first list-box item (in a combo box) that has an
// associated typed value equal to the one specified in the
// /itemData/ parameter.
//
// TODO: Perhaps this should be modified so that the /nIndexStart/ parameter
// acts the same as it does in the other searching methods.
//
// Parameters:
//   nIndexStart - Specifies the zero-based index of the first item in the
// list box to be searched.
//   itemData - The typed value to find in the list box.
//
// Return Value: The zero-based index of the matching item, or -1 if the
// search was unsuccessful.
//
// See Also: TCTypedComboBox::SelectItemData,
// TCComboBoxWindow::FindItemDataRaw, TCComboBoxWindow
template <class I>
inline int TCTypedComboBox<I>::FindItemData(int nIndexStart,I itemData) const
{
  return FindItemDataRaw(nIndexStart, DWORD(itemData));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Searches for the specified typed value in the list box of a
// combo box and, if the item is found, selects the string in the list box
// and copies the string to the edit control.
//
// Searches for the first list-box item (in a combo box) that has an
// associated typed value equal to the one specified in the
// /itemData/ parameter and selects it.
//
// TODO: Perhaps this should be modified so that the /nIndexStart/ parameter
// acts the same as it does in the other searching methods.
//
// Parameters:
//   nIndexStart - Specifies the zero-based index of the first item in the
// list box to be searched.
//   itemData - The typed value to find in the list box.
//
// Return Value: The zero-based index of the matching item, or -1 if the
// search was unsuccessful, and the selection is not changed.
//
// See Also: TCTypedComboBox::FindItemDataRaw,
// TCComboBoxWindow::SelectItemDataRaw, TCComboBoxWindow
template <class I>
inline int TCTypedComboBox<I>::SelectItemData(int nIndexStart, I itemData)
{
  return SelectItemDataRaw(nIndexStart, DWORD(itemData));
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Subclasses the specified window.
//
// Parameters:
//   hwnd - the handle of the window to be subclassed by this object.
template <class I>
inline const TCTypedComboBox<I>& TCTypedComboBox<I>::operator=(HWND hwnd)
{
  SubclassWindow(hwnd);
  return *this;
}


/////////////////////////////////////////////////////////////////////////////
// TCSelfDrawComboBox


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

template <class T, class I>
inline TCSelfDrawComboBox<T, I>::TCSelfDrawComboBox() :
  m_wndParent(NULL, this, e_ParentMessageMap)
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Subclasses a combo box window.
//
// Subclasses the window identified by /hWnd/ and attaches it to the
// CWindowImpl object. The subclassed window now uses
// CWindowImpl::WindowProc. The original window procedure is saved in
// m_pfnSuperWindowProc.
//
// The parent window of the combo box window is also subclassed so that the
// owner draw messages are handled as self draw messages.
//
// Parameters:
//   hWnd - [in] The handle to the window being subclassed.
//
// Return Value: *TRUE* if the window is successfully subclassed;
// otherwise, *FALSE*.
//
// See Also: CWindowImpl::UnsubclassWindow, TCComboBoxWindow
template <class T, class I>
inline BOOL TCSelfDrawComboBox<T, I>::SubclassWindow(HWND hWnd)
{
  // Perform default processing
  if (!TCSelfDrawComboBox_Base::SubclassWindow(hWnd))
    return FALSE;

  // Subclass the parent window so that we can handle our own notifications
  if (!m_wndParent.SubclassWindow(GetParent()))
    return FALSE;

  // Indicate success
  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Description: Non-virtual override called to draw a list box item of an
// owner-draw combo box.
//
// Called by the framework when a visual aspect of an owner-draw combo box
// changes. The itemAction member of the *DRAWITEMSTRUCT* structure defines
// the drawing action that is to be performed. See *WM_DRAWITEM* for a
// description of this structure.
//
// By default, this member function does nothing. Override this member
// function to implement drawing for an owner-draw TCComboBoxWindow object.
// Before this member function terminates, the application should restore all
// graphics device interface (GDI) objects selected for the display context
// supplied in /pdis/.
//
// The default implementation ZAssert's under _DEBUG builds and returns
// false. The ZAssert indicates that the method has not been overridden.
//
// Parameters:
//   pdis - A pointer to a *DRAWITEMSTRUCT* structure that contains
// information about the type of drawing required.
//   itemData - The typed value associated with the item to be drawn.
//
// Return Value: *true* if the message was processed, otherwise *false*.
//
// See Also: TCSelfDrawComboBox::MeasureItem,
// TCSelfDrawComboBox::CompareItem, TCSelfDrawComboBox::DeleteItem,
// WM_DRAWITEM
template <class T, class I>
inline bool TCSelfDrawComboBox<T, I>::DrawItem(DRAWITEMSTRUCT* pdis,
  I itemData)
{
  #ifdef _DEBUG
    bool DrawItemHasNotBeenOverriddenInDerivedClass = false;
    ZAssert(DrawItemHasNotBeenOverriddenInDerivedClass);
  #endif
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Non-virtual override called to get the dimensions of a list
// box item of an owner-draw combo box.
//
// Called by the framework when a combo box with an owner-draw style is
// created. 
//
// By default, this member function does nothing. Override this member
// function and fill in the *MEASUREITEMSTRUCT* structure to inform Windows
// of the dimensions of the list box in the combo box. If the combo box is
// created with the *CBS_OWNERDRAWVARIABLE* style, the framework calls this
// member function for each item in the list box. Otherwise, this member is
// called only once.
//
// The default implementation ZAssert's under _DEBUG builds and returns
// false. The ZAssert indicates that the method has not been overridden.
//
// Parameters:
//   pmis - A pointer to a *MEASUREITEMSTRUCT* structure.
//   itemData - The typed value associated with the item to be measured.
//
// Return Value: *true* if the message was processed, otherwise *false*.
//
// See Also: TCSelfDrawComboBox::DrawItem, TCSelfDrawComboBox::CompareItem,
// TCSelfDrawComboBox::DeleteItem, WM_MEASUREITEM
template <class T, class I>
inline bool TCSelfDrawComboBox<T, I>::MeasureItem(MEASUREITEMSTRUCT* pmis,
  I itemData)
{
  #ifdef _DEBUG
    bool MeasureItemHasNotBeenOverriddenInDerivedClass = false;
    ZAssert(MeasureItemHasNotBeenOverriddenInDerivedClass);
  #endif
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Non-virtual override called to compare two items of the list
// box an owner-draw combo box.
//
// Called by the framework to determine the relative position of a new item
// in the list-box portion of a sorted owner-draw combo box. By default, this
// member function does nothing. If you create an owner-draw combo box with
// the *CBS_SORT* style, you must override this member function to assist the
// framework in sorting new items added to the list box.
//
// The default implementation ZAssert's under _DEBUG builds and returns
// false. The ZAssert indicates that the method has not been overridden.
//
// The /pnResult/ parameter should be set to indicate the relative position
// of the two items described in the *COMPAREITEMSTRUCT* structure and the
// /itemData1/ and /itemData2/ parameters. It can be any of the
// following values:
//
// + -1………Item 1 sorts before item 2.
//
// + 0…………Item 1 and item 2 sort the same.
//
// + 1…………Item 1 sorts after item 2.
//
// Parameters:
//   pcis - [in] A pointer to a *COMPAREITEMSTRUCT* structure.
//   pnResult - [out] Indicates the relative position of the two items
// described in the *COMPAREITEMSTRUCT* structure. See the Remarks for the
// possible values.
//   itemData1 - [in] The typed value associated with the first item of the
// comparison.
//   itemData2 - [in] The typed value associated with the second item of the
// comparsion.
//
// Return Value: *true* if the message was processed, otherwise *false*.
//
// See Also: TCSelfDrawComboBox::DrawItem, TCSelfDrawComboBox::MeasureItem,
// TCSelfDrawComboBox::DeleteItem, WM_COMPAREITEM
template <class T, class I>
inline bool TCSelfDrawComboBox<T, I>::CompareItem(COMPAREITEMSTRUCT* pcis,
  I itemData1, I itemData2, int* pnResult)
{
  #ifdef _DEBUG
    bool CompareItemHasNotBeenOverriddenInDerivedClass = false;
    ZAssert(CompareItemHasNotBeenOverriddenInDerivedClass);
  #endif
  *pnResult = 0;
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Non-virtual override called to delete an item of the list box
// of an owner-draw combo box.
//
// Called by the framework when the user deletes an item from an owner-draw
// TCSelfDrawComboBox object or destroys the combo box. Override this
// function to release any data associated with the deleted item.
//
// The default implementation ZAssert's under _DEBUG builds and returns
// false. The ZAssert indicates that the method has not been overridden.
//
// Parameters:
//   pdis - A pointer to a Windows *DELETEITEMSTRUCT* structure that
// contains information about the deleted item.
//   itemData - The typed value associated with the item to be deleted.
//
// Return Value: *true* if the message was processed, otherwise *false*.
//
// See Also: TCSelfDrawComboBox::DrawItem, TCSelfDrawComboBox::MeasureItem,
// TCSelfDrawComboBox::CompareItem, WM_DELETEITEM
template <class T, class I>
inline bool TCSelfDrawComboBox<T, I>::DeleteItem(DELETEITEMSTRUCT* pdis,
  I itemData)
{
  #ifdef _DEBUG
    bool DeleteItemHasNotBeenOverriddenInDerivedClass = false;
    ZAssert(DeleteItemHasNotBeenOverriddenInDerivedClass);
  #endif
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Message Handlers

/////////////////////////////////////////////////////////////////////////////
// Description: Handles the WM_CREATE message.
//
// This message handler subclasses the parent window so that this object can
// handle owner-draw notification.
//
// Parameters:
//   bHandled - Set to *FALSE* to allow further processing.
//
// Return Value: Returns *FALSE*.
//
// See Also: TCSelfDrawComboBox::SubclassWindow, TCComboBoxWindow
template <class T, class I>
inline LRESULT TCSelfDrawComboBox<T, I>::OnCreate(UINT, WPARAM, LPARAM,
  BOOL& bHandled)
{
  // Subclass the parent window so that we can handle our own notifications
  if (!m_wndParent.SubclassWindow(GetParent()))
    return -1;

  // Allow default processing
  return bHandled = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Handles the WM_DRAWITEM message of the parent window.
//
// This message handler simply delegates to the non-virtual override of
// DrawItem in the most derived class, /T/.
//
// Parameters:
//   lp - Pointer to a *DRAWITEMSTRUCT*.
//   bHandled - Set to the return value of the override.
//
// Return Value: Always returns zero.
//
// See Also: TCSelfDrawComboBox::DrawItem, TCComboBoxWindow
template <class T, class I>
inline LRESULT TCSelfDrawComboBox<T, I>::OnDrawItem(UINT, WPARAM, LPARAM lp,
  BOOL& bHandled)
{
  // Delegate to most-derived class, if we are the target
  DRAWITEMSTRUCT* pdis = (DRAWITEMSTRUCT*)lp;
  bHandled = (m_hWnd == pdis->hwndItem) ?
    static_cast<T*>(this)->DrawItem(pdis, (I)pdis->itemData) : false;

  // Obligatory return code
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Handles the WM_MEASUREITEM message of the parent window.
//
// This message handler simply delegates to the non-virtual override of
// MeasureItem in the most derived class, /T/.
//
// Parameters:
//   lp - Pointer to a *MEASUREITEMSTRUCT*.
//   bHandled - Set to the return value of the override.
//
// Return Value: Always returns zero.
//
// See Also: TCSelfDrawComboBox::MeasureItem, TCComboBoxWindow
template <class T, class I>
inline LRESULT TCSelfDrawComboBox<T, I>::OnMeasureItem(UINT, WPARAM,
  LPARAM lp, BOOL& bHandled)
{
  // Delegate to most-derived class, if we are the target
  MEASUREITEMSTRUCT* pmis = (MEASUREITEMSTRUCT*)lp;
  bHandled = (GetDlgCtrlID() == pmis->CtlID) ?
    static_cast<T*>(this)->MeasureItem(pmis, (I)pmis->itemData) : false;

  // Obligatory return code
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Handles the WM_COMPAREITEM message of the parent window.
//
// This message handler simply delegates to the non-virtual override of
// CompareItem in the most derived class, /T/.
//
// Parameters:
//   lp - Pointer to a *COMPAREITEMSTRUCT*.
//   bHandled - Set to the return value of the override.
//
// Return Value: Returns the comparison result specified by the override.
//
// See Also: TCSelfDrawComboBox::CompareItem, TCComboBoxWindow
template <class T, class I>
inline LRESULT TCSelfDrawComboBox<T, I>::OnCompareItem(UINT, WPARAM,
  LPARAM lp, BOOL& bHandled)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Delegate to most-derived class, if we are the target
  COMPAREITEMSTRUCT* pcis = (COMPAREITEMSTRUCT*)lp;
  int nr = 0;
  bHandled = (m_hWnd == pcis->hwndItem) ?
    pThis->CompareItem(pcis, (I)pcis->itemData1, (I)pcis->itemData2, &nr)
      : false;

  // Obligatory return code
  return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Handles the WM_DELETEITEM message of the parent window.
//
// This message handler simply delegates to the non-virtual override of
// DeleteItem in the most derived class, /T/.
//
// Parameters:
//   lp - Pointer to a *DELETEITEMSTRUCT*.
//   bHandled - Set to the return value of the override.
//
// Return Value: Always returns zero.
//
// See Also: TCSelfDrawComboBox::DeleteItem, TCComboBoxWindow
template <class T, class I>
inline LRESULT TCSelfDrawComboBox<T, I>::OnDeleteItem(UINT, WPARAM,
  LPARAM lp, BOOL& bHandled)
{
  // Delegate to most-derived class, if we are the target
  DELETEITEMSTRUCT* pdis = (DELETEITEMSTRUCT*)lp;
  bHandled = (m_hWnd == pdis->hwndItem) ?
    static_cast<T*>(this)->DeleteItem(pdis, (I)pdis->itemData) : false;

  // Obligatory return code
  return 0;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__ComboBoxWindow_h__
