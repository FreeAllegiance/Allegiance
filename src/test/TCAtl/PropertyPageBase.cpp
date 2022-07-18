/////////////////////////////////////////////////////////////////////////////
// PropertyPageBase.cpp | Implementation of the TCPropertyPageBase class.

#pragma warning(disable: 4786)
#include <vector>
#include <map>
#include <set>
#include <comdef.h>
#include <typeinfo.h>
#include "..\TCLib\AdviseHolder.h"
#include "InsidePropPage.h"
#include "PageEntry.h"

#include "PropertyPageBase.h"


/////////////////////////////////////////////////////////////////////////////
// TCPropertyPageBase


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

TCPropertyPageBase::TCPropertyPageBase() :
  m_iTable(0),
  m_bInitializing(true),
  m_bUpdating(true)
{
}

TCPropertyPageBase::~TCPropertyPageBase()
{
  // Release any objects saved for inside pages
  ReleaseObjectsForInsidePages();

  // Release any interfaces in the map of supported interfaces
  ReleaseSupportedInterfaces();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a pointer to an embedded property page.
//
// Retrieves a pointer to the embedded property page specified by /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: An TCInsidePropPage pointer to the embedded property page if
// /idCtrl/ is valid. Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup
TCInsidePropPage* TCPropertyPageBase::GetInsidePage(UINT idCtrl)
{
  // Lookup the specified page
  XInsidePage* pPage = _GetInsidePage(idCtrl);
  if (!pPage)
    return pPage;

  // Return the page
  ZAssert(pPage->m_hWnd);
  return pPage;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the application-defined 32-bit (DWORD) value
// associated with an embedded property page.
//
// Retrieves the application-defined 32-bit (DWORD) value associated with the
// embedded property page specified by /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: The application-defined 32-bit (DWORD) value associated with
// the specified embedded property page, if /idCtrl/ is valid. Otherwise,
// zero.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePage
DWORD TCPropertyPageBase::GetInsidePageData(UINT idCtrl)
{
  // Lookup the specified page
  XInsidePage* pPage = _GetInsidePage(idCtrl);

  // Return the page data
  return pPage ? pPage->m_pEntry->dwData : 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the application-defined text associated with an
// embedded property page.
//
// Retrieves the application-defined text associated with the embedded
// property page specified by /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: The application-defined text associated with the specified
// embedded property page, if /idCtrl/ is valid.
// Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroupText
LPCTSTR TCPropertyPageBase::GetInsidePageText(UINT idCtrl)
{
  // Lookup the specified page
  XInsidePage* pPage = _GetInsidePage(idCtrl);

  // Return the page text
  return pPage ? pPage->m_pEntry->pszText : NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the state of the dirty flag of an embedded property
// page.
//
// Retrieves the state of the dirty flag of the embedded property page
// specified by /idCtrl/.
// 
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: *true* if the embedded property page's dirty flag is set and
// /idCtrl/ is valid. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePage,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup
bool TCPropertyPageBase::GetDirtyInsidePage(UINT idCtrl)
{
  TCInsidePropPage* pPage = GetInsidePage(idCtrl);
  return pPage ? pPage->Page_IsDirty() : false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Modifies the state of the dirty flag of an embedded property
// page.
//
// Modifies the state of the dirty flag of the embedded property page
// specified by /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//   bDirty - The new state of the embedded property page's dirty flag. If
// *true*, the embedded property page's state is marked as changed. If
// *false*, it is marked as unchanged.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePage,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup
void TCPropertyPageBase::SetDirtyInsidePage(UINT idCtrl, bool bDirty)
{
  TCInsidePropPage* pPage = GetInsidePage(idCtrl);
  if (pPage)
    pPage->SetDirty(bDirty);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a pointer to an embedded property page within a
// group of embedded property pages.
//
// Retrieves a pointer to the embedded property page specified by /dw/,
// within the embedded property page group specified by /idGroup/.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: An TCInsidePropPage pointer to the embedded property page if
// /idGroup/ and /dw/ are valid. Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePage
TCInsidePropPage* TCPropertyPageBase::GetInsidePageOfGroup(UINT idGroup,
  DWORD dw)
{
  // Lookup the specified group/page
  XInsidePage* pPage = _GetInsidePageOfGroup(idGroup, dw);
  if (!pPage)
    return pPage;

  // Return the page
  ZAssert(pPage->m_hWnd);
  return pPage;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the application-defined text associated with an
// embedded property page within a group of embedded property pages.
//
// Retrieves the application-defined text associated with the embedded
// property page specified by /dw/, within the embedded property page group
// specified by /idGroup/.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: The application-defined text associated with the embedded
// property page, if /idGroup/ and /dw/ are valid. Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageText
LPCTSTR TCPropertyPageBase::GetInsidePageOfGroupText(UINT idGroup, DWORD dw)
{
  // Lookup the specified group/page
  XInsidePage* pPage = _GetInsidePageOfGroup(idGroup, dw);
  if (!pPage)
    return NULL;

  // Return the text pointer
  return pPage->m_pEntry->pszText;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves the state of the dirty flag of an embedded
// property page within a group of embedded property pages.
//
// Retrieves the state of the dirty flag of the embedded property page
// specified by /dw/, within the embedded property page group specified by
// /idGroup/.
// 
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: *true* if the embedded property page's dirty flag is set and
// /idGroup/ and /dw/ are valid. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup,
// TCPropertyPageBase::GetDirtyInsidePage
bool TCPropertyPageBase::GetDirtyInsidePageOfGroup(UINT idGroup, DWORD dw)
{
  TCInsidePropPage* pPage = GetInsidePageOfGroup(idGroup, dw);
  return pPage ? pPage->Page_IsDirty() : false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Modifies the state of the dirty flag of an embedded property
// page within a group of embedded property pages.
//
// Modifies the state of the dirty flag of the embedded property page
// specified by /dw/, within the embedded property page group specified by
// /idGroup/.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//   bDirty - The new state of the embedded property page's dirty flag. If
// *true*, the embedded property page's state is marked as changed. If
// *false*, it is marked as unchanged.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePage
void TCPropertyPageBase::SetDirtyInsidePageOfGroup(UINT idGroup,
  DWORD dw, bool bDirty)
{
  TCInsidePropPage* pPage = GetInsidePageOfGroup(idGroup, dw);
  if (pPage)
    pPage->SetDirty(bDirty);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a pointer to the currently visible embedded
// property page within a group of embedded property pages.
//
// Retrieves a pointer to the currently visible embedded property page within
// a group of embedded property pages.
//
// Only one property page, at most, within a group of embedded property pages
// may be visible. In other words, the visibility of pages within a group is
// mutually exclusive.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//
// Return Value: An TCInsidePropPage pointer to the currently visible
// embedded property page within the group, if any. Otherwise, *NULL*. If
// /idGroup/ is not valid, *NULL* is also returned.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup
TCInsidePropPage* TCPropertyPageBase::GetVisiblePageOfGroup(UINT idGroup)
{
  // Find the specified group ID in the map of groups
  CPageGroupIterator itGroup = m_mapPageGroups.find(idGroup);
  if (m_mapPageGroups.end() == itGroup)
    return NULL;

  // Find the currently visible page in the group
  CPageMap* pmap = itGroup->second;
  for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
  {
    XInsidePage* pPage = it->second;
    if (IsWindow(*pPage) && pPage->IsWindowVisible())
      return pPage;
  }

  // Couldn't find a visible page in the group
  return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Makes the window visible of an embedded property page.
//
// Makes the window visible of the embedded property page specified by
// /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: *true* if /idCtrl/ is valid and the operation completed
// successfully. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::HideInsidePage,
// TCPropertyPageBase::ShowInsidePageOfGroup
bool TCPropertyPageBase::ShowInsidePage(UINT idCtrl)
{
  TCInsidePropPage* pPage = GetInsidePage(idCtrl);
  return pPage ? (IsWindow(*pPage) && pPage->ShowWindow(SW_SHOW)) : false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Hides the window of an embedded property page.
//
// Hides the window of the embedded property page specified by /idCtrl/.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: *true* if /idCtrl/ is valid and the operation completed
// successfully. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::GetInsidePageData,
// TCPropertyPageBase::GetInsidePageText,
// TCPropertyPageBase::GetDirtyInsidePage,
// TCPropertyPageBase::SetDirtyInsidePage,
// TCPropertyPageBase::ShowInsidePage,
// TCPropertyPageBase::HideInsidePageOfGroup
bool TCPropertyPageBase::HideInsidePage(UINT idCtrl)
{
  TCInsidePropPage* pPage = GetInsidePage(idCtrl);
  return pPage ? (IsWindow(*pPage) && pPage->ShowWindow(SW_HIDE)) : false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Makes the window visible of an embedded property page within
// a group of embedded property pages, hiding any previously visible page
// within the same group.
//
// Makes the window visible of the embedded property page specified by
// /dw/, within the embedded property page group specified by /idGroup/. If
// another page within the same group was visible it now becomes hidden.
//
// Only one property page, at most, within a group of embedded property pages
// may be visible. In other words, the visibility of pages within a group is
// mutually exclusive.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: *true* if /idGroup/ and /dw/ are valid and the operation
// completed successfully. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageOfGroupData,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePage
bool TCPropertyPageBase::ShowInsidePageOfGroup(UINT idGroup, DWORD dw)
{
  // Find the specified group ID in the map of groups
  CPageGroupIterator itGroup = m_mapPageGroups.find(idGroup);
  if (m_mapPageGroups.end() == itGroup)
    return false;

  // Find the specified page in the group
  CPageMap* pmap = itGroup->second;
  CPageIterator itPage = pmap->find(dw);
  if (pmap->end() == itPage)
    return false;

  // Buffer the window positioning
  TCDeferWindowPosHandle hdwp = ::BeginDeferWindowPos(2);
  UINT uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER;

  // Find the currently visible page in the group and hide it
  for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
  {
    XInsidePage* pPage = it->second;
    if (IsWindow(*pPage) && pPage->IsWindowVisible())
    {
      if (itPage != it)
        ::DeferWindowPos(hdwp, *pPage, NULL, 0, 0, 0, 0,
          uFlags | SWP_HIDEWINDOW);
      break;
    }
  }

  // Show the specified property page, if not already visible
  if (itPage != it)
  {
    // Create the page, if not already created
    XInsidePage* pPage = itPage->second;
    if (pPage->m_hWnd || CreateInsidePage(pPage, true))
      ::DeferWindowPos(hdwp, *pPage, HWND_TOP, 0, 0, 0, 0,
        uFlags | SWP_SHOWWINDOW);
  }

  // Indicate success
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Hides the window of an embedded property page within a group
// of embedded property pages.
//
// Hides the window of the embedded property page specified by /dw/, within
// the embedded property page group specified by /idGroup/.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: *true* if /idGroup/ and /dw/ are valid and the operation
// completed successfully. Otherwise, *false*.
//
// See Also: Embedded Property Page Table Macros,
// TCInsidePropPage, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::GetInsidePageOfGroupData,
// TCPropertyPageBase::GetInsidePageOfGroupText,
// TCPropertyPageBase::GetDirtyInsidePageOfGroup,
// TCPropertyPageBase::SetDirtyInsidePageOfGroup,
// TCPropertyPageBase::ShowInsidePageOfGroup,
// TCPropertyPageBase::HideInsidePage
bool TCPropertyPageBase::HideInsidePageOfGroup(UINT idGroup, DWORD dw)
{
  // Lookup the specified group/page
  XInsidePage* pPage = _GetInsidePageOfGroup(idGroup, dw);
  if (!pPage)
    return false;

  // Hide the window, if it's valid
  if (IsWindow(*pPage))
    pPage->ShowWindow(SW_HIDE);

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Description: Override to allow derived classes a proper place to
// initialize user interface elements.
//
// A derived class should override this method to perform initialization that
// needs to be done only when the property page dialog is first created. Such
// initialization typically includes attaching member variables to fields on
// the dialog box.
//
// Note: This is *not* the appropriate place to initialize the /values/ of
// the dialog box fields. For more information on such initialization, see
// the Property Page Field Update Macros.
//
// This method is called by the OnInitDialogHandler method, which is the
// message handler for the *WM_INITDIALOG* message. When this method is
// called, the following conditions are guaranteed:
//
// + The m_bInitializing flag is set to *true*.
// + Embedded property pages specified in the property page table, if any,
//   have been created.
// + Advisory connections with the set of objects being edited/viewed, if
//   any, have been established.
//
// Following the return from this method, the UpdateFields method is called,
// which, in turn, will call the update methods specified in the entries of
// the property field map.
//
// Return Value: The default return value is *false*, indicating that the
// input focus should not be set to the first tabstop control.
//
// Note: The return value is effectively *void* since the base class
// currently ignores it. Input focus control is not given to the derived
// classes.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::OnInitDialogHandler
bool TCPropertyPageBase::OnInitDialog()
{
  // Return false to not set focus to the first tabstop control
  return false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to allow derived classes to specify a table of
// embedded property pages.
//
// Overridden by derived classes to specify a table of embedded property
// pages. This table is used by the CreateInsidePages method.
//
// Note: A derived class should *not* override this method directly. Instead,
// the derived class should use the Embedded Property Page Table Macros,
// which provide the proper override of this method.
//
// Parameters:
//   ppTable - [out] A pointer to where the address of the table is to be
// returned.
//   iTable - [in] Specifies the index of the page table which is being
// requested. Most property pages have only 1 table, with an index of zero.
// For more information on alternate page tables, see the ALT_INSIDE_PAGE_MAP
// macro.
//
// Return Value: The number of entries in the page table returned thru the
// /ppTable/ parameter. The default implementation returns zero.
//
// See Also: Embedded Property Page Table Macros, XInsidePageEntry
// TCPropertyPageBase::CreateInsidePages
UINT TCPropertyPageBase::GetInsidePageTable(
  const XInsidePageEntry** ppTable, UINT iTable)
{
  UNUSED(iTable);

  // Default does nothing
  if (ppTable)
    *ppTable = NULL;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to allow derived classes to establish any further
// advisory connections with the objects being edited/viewed.
//
// This method is called to allow a derived class an appropriate place to
// establish advisory connections with the objects being edited/viewed.
//
// Note: It is *not* necessary to connect *IPropertyNotifySink* advises here,
// since that is done automatically by the SetAllAdvises method, based on the
// entries in the property field map. Therefore, this override is provided
// only for the possible cases when connections *other* than
// *IPropertyNotifySink* need to be established.
//
// Any advisory connections established in this method should be disconnected
// in the derived class's override of OnClearAllAdvises.
//
// This method is called by the SetAllAdvises method, which is called from
// both the SetObjects and OnInitDialogHandler methods. When this method is
// called, the following conditions are guaranteed:
//
// + The property page is active, meaning that a window has been created for
//   the page.
// + All *IPropertyNotifySink* advisory connections, if any, have already
//   been established, based on the entries in the property field map.
// + The set of objects specified by the /cObjects/ and /ppUnk/ parameters
//   has been filtered down to only the set of objects that support the
//   interfaces specified by the entries of the property field map. This does
//   *not* include the property field map of any embedded property pages.
//
// Note: To assist with establishing advisory connections, consider using the
// TCAdviseHolder class, of which instances can be stored in a collection.
//
// The default implementation of this method does nothing.
//
// Parameters:
//   cObjects - [in] Number of *IUnknown* pointers in the array pointed to by
// /ppUnk/.
//   ppUnk - [in] Pointer to an array of *IUnknown* interface pointers, which
// represents the set of objects being edited/viewed by this property page.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::OnClearAllAdvises,
// TCPropertyPageBase::SetAllAdvises,
// TCPropertyPageBase::OnInitDialogHandler
void TCPropertyPageBase::OnSetAllAdvises(ULONG cObjects, IUnknown** ppUnk)
{
  UNUSED(cObjects);
  UNUSED(ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to allow derived classes to disconnect any advisory
// connections established in OnSetAllAdvises.
//
// This method is called to allow a derived class an appropriate place to
// disconnect advisory connections previously established with the objects
// being edited/viewed.
//
// Note: It is *not* necessary to disconnect *IPropertyNotifySink* advises
// here, since that is done automatically by the ClearAllAdvises method.
//
// The advisory connections disconnected in this method should exactly match
// those established in the derived class's override of OnSetAllAdvises.
//
// This method is called by the ClearAllAdvises method, which is called from
// both the SetObjects and OnNcDestroyHandler methods. When this method is
// called, the following condition is guaranteed:
//
// + All established *IPropertyNotifySink* advisory connections, if any, have
//   not yet been broken.
//
// Following the return from this method, all established
// *IPropertyNotifySink* advisory connections, if any, will be disconnected.
//
// Note: If the TCAdviseHolder class was used to establish a connection, it
// will be broken when the instance is destroyed. So, if a collection of them
// were being maintained, simply erasing the contents of the collection would
// break the connections. This assumes, of course, that the collection holds
// actual instances, rather than pointers to instances.
//
// The default implementation of this method does nothing.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::OnSetAllAdvises,
// TCPropertyPageBase::SetAllAdvises,
// TCPropertyPageBase::OnNcDestroyHandler
void TCPropertyPageBase::OnClearAllAdvises()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Creates the window of an embedded property page.
//
// This method is used by the implementation to create the window of the
// specified embedded property page.
//
// Parameters:
//   pPage - The embedded property page instance for which a window is to be
// created.
//   bOfGroup - Indicator of whether or not /pPage/ is a page belonging to a
// mutually exclusive group of embedded property pages. *true* indicates
// that it is a member of a group; *false* indicates that it is not.
//
// Return Value: *true* if the page was successfully created; otherwise
// *false*.
//
// See Also: XInsidePage, TCPropertyPageBase::CreateInsidePages
bool TCPropertyPageBase::CreateInsidePage(XInsidePage* pPage, bool bOfGroup)
{
  // Save the focus window
  HWND hwndFocus = GetFocus();

  // Create the page
  HRESULT hr = SetObjectsToInsidePage(pPage);
  bool bCreated = pPage->Create(GetPageWindow(),
    IPropertyPageSitePtr(GetPageUnknown()), pPage->m_pEntry->idPosCtrl,
      *pPage->m_pEntry->pclsid, pPage->m_pEntry->bVisible,
        pPage->m_pEntry->bSizeToCtrl, !bOfGroup);

  #ifdef _DEBUG
    if (!bCreated)
    {
      char sz1[64], sz3[64];
      sprintf(sz1, "TCPropertyPageImpl<%hs>::CreateInsidePage", TypeName());
      LPCSTR sz2 = "Failed to create child property page ";
      if (!bOfGroup)
        sprintf(sz3, "with control id 0x%08X.", pPage->m_pEntry->idPosCtrl);
      else
        sprintf(sz3, "in page group 0x%08X with DWORD value 0x%08X.",
          pPage->m_pEntry->idPosCtrl, pPage->m_pEntry->dwData);
      _TRACE3("%hs%hs%hs\n", sz1, sz2, sz3);
      ZAssert(bCreated);
    }
  #endif

  // Restore the focus to the previous focus window
  SetFocus(hwndFocus);
  return bCreated;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Creates the embedded property pages specified in the property
// page map.
//
// This method is used by the implementation to create all the embedded
// property pages specified in the property page map.
//
// This method loops thru the entries of the derived class's property page
// map and creates an XInsidePage instance. Throughout this process, the
// entries that represent pages within a group are seperated out and stored
// appropriately.
//
// Once the objects are created, grouped, and stored, the pages that are to
// be initially visible are created by calling the CreateInsidePage method.
//
// See Also: Embedded Property Page Table Macros, XInsidePageEntry,
// XInsidePage, TCPropertyPageBase::CPageMap,
// TCPropertyPageBase::CreateInsidePage,
// TCPropertyPageBase::DestroyInsidePages
void TCPropertyPageBase::CreateInsidePages()
{
  // Get the table of child property pages
  const XInsidePageEntry* pEntries = NULL;
  UINT nCount = GetInsidePageTable(&pEntries, m_iTable);
  if (!nCount)
    return;

  // Seperate the group entries
  for (UINT i = 0; i < nCount; i++)
  {
    // Get the next entry in the table
    const XInsidePageEntry& entry = pEntries[i];
    UINT idCtrl = entry.idPosCtrl;

    // Find the entry's control ID in the map of pages
    CPageIterator it = m_mapPages.find(idCtrl);
    if (m_mapPages.end() == it)
    {
      // Find the entry's control ID in the map of page groups
      CPageGroupIterator itGroup = m_mapPageGroups.find(idCtrl);
      if (m_mapPageGroups.end() == itGroup)
      {
        // Map the control ID to a new page
        XInsidePage* pPage = new XInsidePage(&entry);
        CPageMap::value_type value(idCtrl, pPage);
        m_mapPages.insert(value);
      }
      else
      {
        // Get the page map for the group
        CPageMap* pmap = itGroup->second;

        // Map the DWORD value to a new page
        DWORD dw = entry.dwData;
        XInsidePage* pPage = new XInsidePage(&entry);
        CPageMap::value_type value(dw, pPage);
        bool bInserted = pmap->insert(value).second;
        if (!bInserted)
        {
          delete pPage;
          #ifdef _DEBUG
            char sz[_MAX_PATH];
            sprintf(sz, "TCPropertyPageImpl<%hs>::CreateInsidePages(): "
              "More than one entry\n\tin page group 0x%08X, was specified "
              "with a DWORD value of 0x%08x.\n\tDuplicate is being ignored.",
              TypeName(), idCtrl, dw);
            _TRACE1("%hs\n", sz);
            ZAssert(bInserted);
          #endif
        }
      }
    }
    else
    {
      // Map the control ID to a new page group
      CPageMap* pPageMap = new CPageMap;
      CPageGroupMap::value_type valuePageMap(idCtrl, pPageMap);
      m_mapPageGroups.insert(valuePageMap);

      // Move the located page to the new page group
      XInsidePage* pPage = it->second;
      CPageMap::value_type valuePage(pPage->m_pEntry->dwData, pPage);
      pPageMap->insert(valuePage);
      m_mapPages.erase(it);

      // Repeat the loop iteration to add the new page to the new page group
      --i;
    }
  }

  // Create each page marked as visible
  for (CPageIterator it = m_mapPages.begin(); it != m_mapPages.end(); ++it)
  {
    XInsidePage* pPage = it->second;
    const XInsidePageEntry& entry = *pPage->m_pEntry;
    if (entry.bVisible)
      CreateInsidePage(pPage, false);
  }

  // Create, at most, one page marked as visible in each page group
  CPageGroupIterator itGroup = m_mapPageGroups.begin();
  for (; itGroup != m_mapPageGroups.end(); ++itGroup)
  {
    #ifdef _DEBUG
      bool bOneIsVisible = false;
    #endif // _DEBUG
    
    CPageMap* pmap = itGroup->second;
    for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
    {
      XInsidePage* pPage = it->second;
      const XInsidePageEntry& entry = *pPage->m_pEntry;
      if (entry.bVisible)
      {
        #ifdef _DEBUG
          if (bOneIsVisible)
          {
            char sz[_MAX_PATH];
            sprintf(sz, "TCPropertyPageImpl<%hs>::CreateInsidePages(): "
              "More than one entry\n\tin page group 0x%08X was specified"
              "with a bVisible value of true.\n\tOnly the first one has"
              "been created.", TypeName(), entry.idPosCtrl);
            _TRACE1("%hs\n", sz);
            ZAssert(!bOneIsVisible);
          }
        #endif // _DEBUG

        if (CreateInsidePage(pPage, true))
        {
          #ifdef _DEBUG
            bOneIsVisible = true;
          #else // _DEBUG
            break;
          #endif // _DEBUG
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Destroys all instances of embedded property pages.
//
// This method is used by the implementation to destroy all instances of
// embedded property pages, if any. This includes page groups and the pages
// within them.
//
// See Also: TCPropertyPageBase::DestroyInsidePageMap
void TCPropertyPageBase::DestroyInsidePages()
{
  // Destroy the main page map
  DestroyInsidePageMap(&m_mapPages);

  // Destroy the page map of each page group
  CPageGroupIterator it;
  for (it = m_mapPageGroups.begin(); it != m_mapPageGroups.end(); ++it)
  {
    CPageMap* pmap = it->second;
    DestroyInsidePageMap(pmap);
    delete pmap;
  }

  // Clear the page group map
  m_mapPageGroups.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Destroys all embedded property pages within a specified map.
//
// This method is used by the implementation to destroy all embedded property
// pages within the specified map and clear the map of all entries.
//
// Parameters:
//   pmap - A pointer to the page map for whose pages are to be destroyed.
//
// See Also: TCPropertyPageBase::DestroyInsidePages,
// TCPropertyPageBase::CPageMap
void TCPropertyPageBase::DestroyInsidePageMap(
  TCPropertyPageBase::CPageMap* pmap)
{
  // Loop thru each page of the specified map
  for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
  {
    // Get the next page
    XInsidePage* pPage = it->second;

    // Ensure that the page window is destroyed
    ZAssert(!IsWindow(*pPage));

    // Delete the page object
    delete pPage;
  }

  // Clear the map of all dead page entries
  pmap->clear();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a pointer to an embedded property page, and ensures
// that a window is created for the page.
//
// This method is used by the implementation to retrieve a pointer to the
// embedded property page specified by /idCtrl/. If the page is not active
// (does not have a window created for it), it calls CreateInsidePage to
// create the window.
//
// Parameters:
//   idCtrl - The identifier of the embedded property page.
//
// Return Value: An XInsidePage pointer to the embedded property page if
// /idCtrl/ is valid. Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros, XInsidePage,
// TCPropertyPageBase::GetInsidePage,
// TCPropertyPageBase::_GetInsidePageOfGroup
XInsidePage* TCPropertyPageBase::_GetInsidePage(UINT idCtrl)
{
  // Find the specified control ID in the map of pages
  CPageIterator it = m_mapPages.find(idCtrl);
  if (m_mapPages.end() == it)
    return NULL;

  // Create the page, if not already created
  XInsidePage* pPage = it->second;
  if (!pPage->m_hWnd)
    return CreateInsidePage(pPage, false) ? pPage : NULL;
  return pPage;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Retrieves a pointer to an embedded property page within a
// group of embedded property pages, and ensures that a window is created for
// the page.
//
// This method is used by the implementation to retrieve a pointer to the
// embedded property page specified by /dw/, within the embedded property
// page group specified by /idGroup/. If the page is not active (does not
// have a window created for it), it calls CreateInsidePage to create the
// window.
//
// Parameters:
//   idGroup - The identifier of the embedded property page group.
//   dw - The application-defined 32-bit (DWORD) value that uniquely
// identifies the page within the page group.
//
// Return Value: An XInsidePage pointer to the embedded property page if
// /idGroup/ and /dw/ are valid. Otherwise, *NULL*.
//
// See Also: Embedded Property Page Table Macros, XInsidePage,
// TCPropertyPageBase::GetInsidePageOfGroup,
// TCPropertyPageBase::_GetInsidePage
XInsidePage* TCPropertyPageBase::_GetInsidePageOfGroup(UINT idGroup,
  DWORD dw)
{
  // Find the specified group ID in the map of groups
  CPageGroupIterator itGroup = m_mapPageGroups.find(idGroup);
  if (m_mapPageGroups.end() == itGroup)
    return NULL;

  // Find the specified DWORD value in the map of pages for the group
  CPageMap* pmap = itGroup->second;
  CPageIterator it = pmap->find(dw);
  if (pmap->end() == it)
    return NULL;

  // Return the page
  XInsidePage* pPage = it->second;
  return pPage;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Filters a set of objects based on the entries in the property
// field map.
//
// This method is used by the implementation to filter the specified set of
// objects. Only the objects that support interfaces specified in the entries
// of the property field map are copied to the destination array.
//
// This method first forwards the specified set of objects to each embedded
// property page, including those within groups. It does this by calling the
// SetObjectsOfInsidePages method. This gives each embedded property page the
// opportunity to perform its own filtering from the original set of objects.
//
// Next, the method releases the previous set of interface pointers specified
// by the /cObjectsDest/ and /ppUnkDest/ parameters and deletes the array. A
// new array is then created and stored in the same parameters. The new array
// is populated with the filtered set of objects.
//
// Finally, if the property page is active (a window has been created for
// it), the UpdateFields and SetAllAdvises methods are called. If the page is
// not active, those method calls are deferred to OnInitDialogHandler, which
// will be called when the page does become active.
//
// TODO: Shouldn't the advises be set /prior/ to UpdateFields?
//
// Parameters:
//   cObjectsDest - [in, out] A reference to the number of *IUnknown* „
// pointers in the array pointed to by /ppUnkDest/. The referenced variable
// also receives the count of filtered objects upon return.
//   ppUnkDest - [in, out] A reference to a pointer to an array of
// *IUnknown* interface pointers which is replaced with a pointer to the
// filtered array of objects upon return.
//   cObjects - [in] The number of *IUnknown* pointers in the array pointed
// to by /ppUnk/.
//   ppUnk - [in] A pointer to an array of *IUnknown* interface pointers
// where each pointer identifies a unique object affected by the property
// page. This array is considered the /original/ set, from which the
// /filtered/ set is determined.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::SetObjectsOfInsidePages,
// TCPropertyPageBase::UpdateFields,
// TCPropertyPageBase::SetAllAdvises,
// TCPropertyPageBase::OnInitDialogHandler
HRESULT TCPropertyPageBase::SetObjects(ULONG& cObjectsDest,
  IUnknown**& ppUnkDest, ULONG cObjects, IUnknown** ppUnk)
{
  // Only save each object that this page knows how to talk to
  HRESULT hr = S_OK;
  CUnkVector pVec = NULL;
  __try
  {
    // Forward the method to each embedded property page, including groups
    if (SUCCEEDED(hr = SetObjectsOfInsidePages(cObjects, ppUnk)))
    {
      // Loop thru the specified objects, filtering out the unknown ones
      ReleaseSupportedInterfaces();
      CUnkVector& vec = *(pVec = new CUnkVector);
      for (ULONG i = 0; i < cObjects; i++)
        if (ppUnk[i] && IsObjectKnown(ppUnk[i]))
          vec.push_back(ppUnk[i]);

      // Release the previous set of destination interface pointers
      for (UINT iObj = 0; iObj < cObjectsDest; ++iObj)
        ppUnkDest[iObj]->Release();
      if (ppUnkDest != NULL)
      {
        delete [] ppUnkDest;
        ppUnkDest = NULL;
        cObjectsDest = 0;
      }
      ZAssert(0 == cObjectsDest);

      // Save the filtered objects in the specified destination fields
      if (vec.size())
      {
        ATLTRY(ppUnkDest = new IUnknown*[vec.size()]);
        if (NULL == ppUnkDest)
          hr = E_OUTOFMEMORY;
        else
        {
          for (i = 0; i < vec.size(); ++i)
            (ppUnkDest[i] = vec[i])->AddRef();
          cObjectsDest = vec.size();
        }
      }

      // Populate the dialog fields and set all advises, if the window exists
      if (SUCCEEDED(hr) && ::IsWindow(GetPageWindow()))
      {
        // TODO: Shouldn't the advises be set /prior/ to UpdateFields?
        UpdateFields();
        SetAllAdvises(cObjectsDest, ppUnkDest);
      }
    }
  }
  __except(1)
  {
    _TRACE1(
      "TCPropertyPageBase[%hs]::SetObjects(): Unknown Exception Caught!\n",
        TypeName());

    // Remove and unadvise all connections
    ClearAllAdvises();

    // Release any objects saved for inside pages
    ReleaseObjectsForInsidePages();

    // Release the previous set of interface pointers
    ReleaseSupportedInterfaces();
    for (UINT iObj = 0; iObj < cObjectsDest; ++iObj)
      ppUnkDest[iObj]->Release();
    if (ppUnkDest != NULL)
    {
      delete [] ppUnkDest;
      ppUnkDest = NULL;
      cObjectsDest = 0;
    }

    // Update dialog fields from objects, if the window exists
    if (::IsWindow(GetPageWindow()))
      UpdateFields();

    // Interpret the exception as a server exception
    hr = RPC_E_SERVERFAULT;
  }

  // Delete the vector object, if we used one
  if (pVec)
    delete pVec;

  // Return last result
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Forwards the original set of objects to each embedded
// property page, if any.
//
// This method is used by the implementation to forward the original set of
// objects to each embedded property page, if any. This gives each embedded
// property page the opportunity to perform its own filtering from the
// original set of objects.
//
// Parameters:
//   cObjects - [in] The number of *IUnknown* pointers in the array pointed
// to by /ppUnk/.
//   ppUnk - [in] A pointer to an array of *IUnknown* interface pointers
// where each pointer identifies a unique object affected by the property
// page.
//
// See Also: Embedded Property Page Table Macros, XInsidePage,
// TCPropertyPageBase::SetObjects,
// TCPropertyPageBase::SaveObjectsForInsidePages
HRESULT TCPropertyPageBase::SetObjectsOfInsidePages(ULONG cObjects,
  IUnknown** ppUnk)
{
  // Save the entire array of objects for use by inside pages
  SaveObjectsForInsidePages(cObjects, ppUnk);

  // Forward the method to each embedded property page
  for (CPageIterator it = m_mapPages.begin(); it != m_mapPages.end(); ++it)
  {
    XInsidePage* pPage = it->second;
    if (pPage->m_hWnd)
      RETURN_FAILED(pPage->SetObjects(cObjects, ppUnk));
  }

  // Forward the method to each embedded property page of each group
  CPageGroupIterator itGroup = m_mapPageGroups.begin();
  for (; itGroup != m_mapPageGroups.end(); ++itGroup)
  {
    CPageMap* pmap = itGroup->second;
    for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
    {
      XInsidePage* pPage = it->second;
      if (pPage->m_hWnd)
        RETURN_FAILED(pPage->SetObjects(cObjects, ppUnk));
    }
  }

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Saves the original set of objects in order to be able to
// forward them on to any new embedded property pages, if any.
//
// This method is used by the implementation to save the original set of
// objects. This allows any new embedded property pages to be given the
// opportunity to perform its own filtering from the original set of
// objects.
//
// Parameters:
//   cObjects - [in] The number of *IUnknown* pointers in the array pointed
// to by /ppUnk/.
//   ppUnk - [in] A pointer to an array of *IUnknown* interface pointers
// where each pointer identifies a unique object affected by the property
// page.
//
// See Also: Embedded Property Page Table Macros, XInsidePage,
// TCPropertyPageBase::SetObjects,
// TCPropertyPageBase::SetObjectsOfInsidePages
void TCPropertyPageBase::SaveObjectsForInsidePages(ULONG cObjects,
  IUnknown** ppUnk)
{
  // Do nothing if there are no table entries
  if (!GetInsidePageTable(NULL, m_iTable))
    return;

  // Release any current array of objects
  ReleaseObjectsForInsidePages();

  // Copy the specified array of objects
  m_vecForInsidePages.resize(cObjects, NULL);
  for (ULONG i = 0; i < cObjects; ++i)
  {
    IUnknown* punk = ppUnk[i];
    m_vecForInsidePages[i] = punk;
    if (punk)
      punk->AddRef();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Forwards the original set of objects to an embedded property
// page when it is first created.
//
// This method is used by the implementation to forward the original set of
// objects to the embedded property page specified byt /pPage/. The original
// set of objects is saved by the SaveObjectsForInsidePages method. This
// allows the freshly-created embedded property page the opportunity to
// perform its own filtering from the original set of objects.
//
// Parameters:
//   pPage - A pointer to the embedded property page just create, to which
// the original set of objects will be forwarded.
//
// See Also: Embedded Property Page Table Macros, TCInsidePropPage,
// TCPropertyPageBase::SaveObjectsForInsidePages,
// TCPropertyPageBase::CreateInsidePage
HRESULT TCPropertyPageBase::SetObjectsToInsidePage(TCInsidePropPage* pPage)
{
  ULONG cObjects = m_vecForInsidePages.size();
  IUnknown** ppUnk = m_vecForInsidePages.begin();
  return pPage->SetObjects(cObjects, ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Forwards the /Apply/ operation to each embedded property
// page, if any.
//
// This method is used by the implementation to forward the /Apply/ operation
// to each embedded property page, if any.
//
// See Also: Embedded Property Page Table Macros, XInsidePage,
// TCPropertyPageImpl::Apply
HRESULT TCPropertyPageBase::ApplyToInsidePages()
{
  // Forward the method to each embedded property page
  for (CPageIterator it = m_mapPages.begin(); it != m_mapPages.end(); ++it)
  {
    XInsidePage* pPage = it->second;
    if ((IsWindow(*pPage) && pPage->IsWindowVisible())
      || pPage->m_pEntry->bAlwaysApply)
        RETURN_FAILED(pPage->Page_Apply());
  }

  // Forward the method to each embedded property page of each group
  CPageGroupIterator itGroup = m_mapPageGroups.begin();
  for (; itGroup != m_mapPageGroups.end(); ++itGroup)
  {
    CPageMap* pmap = itGroup->second;
    CPageIterator itVisible = pmap->end();
    for (CPageIterator it = pmap->begin(); it != pmap->end(); ++it)
    {
      XInsidePage* pPage = it->second;
      if (IsWindow(*pPage))
      {
        if (pPage->IsWindowVisible())
          itVisible = it;
        else if (pPage->m_pEntry->bAlwaysApply)
          RETURN_FAILED(pPage->Page_Apply());
      }
    }
    if (pmap->end() != itVisible)
    {
      XInsidePage* pPage = itVisible->second;
      RETURN_FAILED(pPage->Page_Apply());
    }
  }

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Releases the original set of objects that are maintained for
// when a new embedded property page is created.
//
// This method is used by the implementation to release the original set of
// objects that were saved by the SaveObjectsForInsidePages method.
//
// See Also: Embedded Property Page Table Macros,
// TCPropertyPageBase::SaveObjectsForInsidePages
void TCPropertyPageBase::ReleaseObjectsForInsidePages()
{
  CUnkIterator it = m_vecForInsidePages.begin();
  for (; it != m_vecForInsidePages.end(); ++it)
  {
    IUnknown* punk = *it;
    if (punk)
      punk->Release();
  }
  m_vecForInsidePages.clear();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Releases the set of interfaces supported by this property
// page.
//
// This method is used by the implementation to release the set of interfaces
// supported by this property page, as previously determined from the
// property field map by the CreateInsidePages method.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::CreateInsidePages
void TCPropertyPageBase::ReleaseSupportedInterfaces()
{
  CUnkVectorMap::iterator it = m_mapInterfaces.begin();
  for (; m_mapInterfaces.end() != it; ++it)
  {
    CUnkVector& vec = it->second;
    for (CUnkIterator itUnk = vec.begin(); vec.end() != itUnk; ++itUnk)
    {
      IUnknown* punk = *itUnk;
      punk->Release();
    }
    vec.clear();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Establishes *IPropertyNotifySink* advisory connections with
// each of the specified objects.
//
// This method is used by the implementation to establish
// *IPropertyNotifySink* advisory connections with each of the specified
// objects. Special care is taken to ensure that an objects that is already
// connected is not broken and immediately reconnected, which would incur
// unnecessary DCOM round-trips.
// 
// After established all of the connections, the OnSetAllAdvises virtual
// override is called to allow the derived class an opportunity to establish
// any other advisory connections it may need.
//
//
// See Also: TCPropertyPageBase::ClearAllAdvises,
// TCPropertyPageBase::SetAdvises, TCPropertyPageBase::OnSetAllAdvises
void TCPropertyPageBase::SetAllAdvises(ULONG cObjects, IUnknown** ppUnk)
{
  // Keep a set of IConnectionPointContainer pointers
  typedef std::set<IConnectionPointContainer*> CCPCSet;
  typedef CCPCSet::iterator CCPCSetIterator;
  CCPCSet setCPC;

  // Set advises on each object
  IUnknown* punkThis = GetPageUnknown();
  IUnknown** ppunkEnd = ppUnk + cObjects;
  for (IUnknown** it = ppUnk; it != ppunkEnd; ++it)
  {
    IConnectionPointContainer* pCPC = SetAdvises(*it, punkThis);
    if (NULL != pCPC)
      setCPC.insert(pCPC);
  }

  // Remove and unadvise each map item not in the array of objects
  CAdviseIterator itAdv = m_mapAdvise.begin();
  while (itAdv != m_mapAdvise.end())
  {
    // Lookup the object in the set
    CCPCSetIterator itCPC = setCPC.find(itAdv->first);
    if (setCPC.end() == itCPC)
      itAdv = m_mapAdvise.erase(itAdv);
    else
      ++itAdv;
  }

  // Allow derived classes to set advises
  OnSetAllAdvises(cObjects, ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Establishes a single *IPropertyNotifySink* advisory
// connection with the specified object.
//
// This method is used by the implementation to establish a single
// *IPropertyNotifySink* advisory connection with the specified object.
//
// See Also: TCPropertyPageBase::SetAdvises
IConnectionPointContainer* TCPropertyPageBase::SetAdvises(
  IUnknown* punk, IUnknown* punkThis)
{
  IConnectionPointContainer* pResult = NULL;
  TCAdviseHolder pAdv = new TCAdviseHolder;
  __try
  {
    // Find the IID_IPropertyNotifySink connection point
    TCAdviseHolder& adv = *pAdv;
    hr = adv.FindConnectionPoint(punk, IID_IPropertyNotifySink)
    if (SUCCEEDED(hr))
    {
      // Ensure that we don't already hold an advise on this object
      if (m_mapAdvise.end() == m_mapAdvise.find(adv.GetCPC()))
      {
        // Insert the connection advise holder into the map
        if (SUCCEEDED(hr = adv.Advise(punkThis)))
          m_mapAdvise.insert(CAdviseMap::value_type(adv.GetCPC(), adv));
      }

      // Return the IConnectionPointContainer pointer as identity
      pResult = adv.GetCPC();
    }
  }
  __except(1)
  {
    _TRACE1("%hs: Caught an unknown exception\n", szFn);
    return NULL;
  }

  // Delete the advise holder
  delete pAdv;

  // Return the last result
  return pResult;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Message Handlers

/////////////////////////////////////////////////////////////////////////////
// Description: Message handler for *WM_INITDIALOG*.
//
// Handles the *WM_INITDIALOG* window message by creating any embedded
// property pages needed, establishing advisory connections, allowing the
// derived class to initialize itself, and updating the fields of the
// property page. Within the scope of this method, the m_bInitializing data
// member is set to *true*, returning to *false* at the end of the scope.
//
// See Also: TCPropertyPageBase::OnInitDialog,
// TCPropertyPageBase::m_bInitializing
void TCPropertyPageBase::OnInitDialogHandler(ULONG cObjects,
  IUnknown** ppUnk)
{
  // Set the initialization flag
  m_bInitializing = true;

  // Create the embedded property pages
  CreateInsidePages();

  // Set the advises for all the objects
  if (!m_mapAdvise.size())
    SetAllAdvises(cObjects, ppUnk);

  // Allow derived class to initialize controls and such
  bool bResult = OnInitDialog();

  // Update dialog fields from objects
  UpdateFields();

  // Reset the initialization flag
  m_bInitializing = false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Message handler for the TCPropertyPageImpl::wm_OnChanged
// message.
//
// Handles the TCPropertyPageImpl::wm_OnChanged window message which is
// posted from TCPropertyPageImpl's implementation of the
// *IPropertyNotifySink::OnChanged* interface method. That method posts the
// message in order to defer processing of the property change notification
// onto the window's main thread.
//
// This method simply calls the UpdateFields method with the DISPID of the
// property that has changed.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::UpdateFields, TCPropertyPageImpl::wm_OnChanged
LRESULT TCPropertyPageBase::OnChangedHandler(UINT, WPARAM wp, LPARAM, BOOL&)
{
  // Parse the message parameters
  DISPID dispid = DISPID(wp);

  _TRACE3("TCPropertyPageBase[%hs]::OnChangedHandler(): dispid = 0x%08X (%d)\n",
    TypeName(), dispid, dispid);

  // Update dialog fields from objects
  UpdateFields(dispid);

  // Obligatory return code
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Message handler for *WM_NCDESTROY*.
//
// Handles the *WM_NCDESTROY* window message by calling ClearAllAdvises and
// DestroyInsidePages.
//
// See Also: TCPropertyPageBase::ClearAllAdvises,
// TCPropertyPageBase::DestroyInsidePages
LRESULT TCPropertyPageBase::OnNcDestroyHandler(UINT, WPARAM, LPARAM,
  BOOL& bHandled)
{
  _TRACE1("TCPropertyPageBase[%hs]::OnNcDestroyHandler()\n", TypeName());

  // Remove and unadvise all connections
  ClearAllAdvises();

  // Destroy the embedded property pages
  DestroyInsidePages();

  // Allow default processing
  return bHandled = FALSE;
}

