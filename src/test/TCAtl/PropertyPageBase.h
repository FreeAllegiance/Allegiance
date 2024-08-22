#pragma once

/////////////////////////////////////////////////////////////////////////////
// PropertyPageBase.h | Declaration of the TCPropertyPageBase class.

#pragma warning(disable: 4786)
#include <vector>
#include <map>
#include <set>
#include <comdef.h>
#include <typeinfo.h>
#include "..\TCLib\AdviseHolder.h"
#include "InsidePropPage.h"
#include "PageEntry.h"


/////////////////////////////////////////////////////////////////////////////
// Description: Provides the advanced functionality of TCPropertyPageImpl.
//
// TCPropertyPageBase serves as the base class of TCPropertyPageImpl. Since
// that class is a template, the portions of functionality that are not
// affected by the template parameters are implemented in this class to avoid
// duplicated code generation. See TCPropertyPageImpl for more information.
//
// See Also: TCPropertyPageImpl
class ATL_NO_VTABLE TCPropertyPageBase
{
// Group=Types
public:
  typedef std::map<UINT, XInsidePage*> CPageMap;
  typedef CPageMap::iterator CPageIterator;
  typedef std::map<UINT, CPageMap*> CPageGroupMap;
  typedef CPageGroupMap::iterator CPageGroupIterator;
  typedef std::map<IConnectionPointContainer*, TCAdviseHolder> CAdviseMap;
  typedef CAdviseMap::iterator CAdviseIterator;
  typedef std::vector<IUnknown*> CUnkVector;
  typedef CUnkVector::iterator CUnkIterator;
  typedef std::map<IID, CUnkVector> CUnkVectorMap;

// Group=Construction / Destruction
public:
  TCPropertyPageBase();
  virtual ~TCPropertyPageBase();

// Group=Attributes
public:
  TCInsidePropPage* GetInsidePage(UINT idCtrl);
  DWORD GetInsidePageData(UINT idCtrl);
  LPCTSTR GetInsidePageText(UINT idCtrl);
  bool GetDirtyInsidePage(UINT idCtrl);
  void SetDirtyInsidePage(UINT idCtrl, bool bDirty);

  TCInsidePropPage* GetInsidePageOfGroup(UINT idGroup, DWORD dw);
  LPCTSTR GetInsidePageOfGroupText(UINT idGroup, DWORD dw);
  TCInsidePropPage* GetVisiblePageOfGroup(UINT idGroup);
  bool GetDirtyInsidePageOfGroup(UINT idGroup, DWORD dw);
  void SetDirtyInsidePageOfGroup(UINT idGroup, DWORD dw, bool bDirty);

// Group=Operations
public:
  bool ShowInsidePage(UINT idCtrl);
  bool HideInsidePage(UINT idCtrl);
  bool ShowInsidePageOfGroup(UINT idGroup, DWORD dw);
  bool HideInsidePageOfGroup(UINT idGroup, DWORD dw);

// Group=Diagnostic Overrides
protected:
  #ifdef _DEBUG
    virtual LPCSTR TypeName() = 0;
  #endif

// Group=Overrides
protected:
  virtual bool OnInitDialog();
  virtual UINT GetInsidePageTable(const XInsidePageEntry** ppTable,
    UINT iTable);
  virtual HWND GetPageWindow() = 0;
  virtual IUnknown* GetPageUnknown() = 0;
  virtual bool IsObjectKnown(IUnknown* punk) = 0;
  virtual void UpdateFields(DISPID dispid = DISPID_UNKNOWN) = 0;
  virtual void OnSetAllAdvises(ULONG cObjects, IUnknown** ppUnk);
  virtual void OnClearAllAdvises();

// Group=Implementation
protected:
  bool CreateInsidePage(XInsidePage* pPage, bool bOfGroup);
  void CreateInsidePages();
  void DestroyInsidePages();
  void DestroyInsidePageMap(CPageMap* pmap);
  XInsidePage* _GetInsidePage(UINT idCtrl);
  XInsidePage* _GetInsidePageOfGroup(UINT idGroup, DWORD dw);

  HRESULT SetObjects(ULONG& cObjectsDest, IUnknown**& ppUnkDest,
    ULONG cObjects, IUnknown** ppUnk);
  HRESULT SetObjectsOfInsidePages(ULONG cObjects, IUnknown** ppUnk);
  void SaveObjectsForInsidePages(ULONG cObjects, IUnknown** ppUnk);
  HRESULT SetObjectsToInsidePage(TCInsidePropPage*);
  HRESULT ApplyToInsidePages();
  void ReleaseObjectsForInsidePages();
  void ReleaseSupportedInterfaces();

  void SetAllAdvises(ULONG cObjects, IUnknown** ppUnk);
  IConnectionPointContainer* SetAdvises(IUnknown* punk, IUnknown* punkThis);
  void ClearAllAdvises();

// Group=Message Handlers
public:
  void OnInitDialogHandler(ULONG cObjects, IUnknown** ppUnk);
  LRESULT OnChangedHandler(UINT, WPARAM, LPARAM, BOOL&);
  LRESULT OnNcDestroyHandler(UINT, WPARAM, LPARAM, BOOL&);

// Group=Data Members
protected:
  // Specifies the current sub-table in the embedded property page map.
  UINT          m_iTable;
  // Contains the embedded property page instance, indexed by ID.
  CPageMap      m_mapPages;
  // Contains the embedded property page groups, indexed by ID.
  CPageGroupMap m_mapPageGroups;
  // Contains the *IUnknown* pointers passed to *IPropertyPage::SetObects*.
  CUnkVector    m_vecForInsidePages;
  // Contains advisory connections to the objects being viewed/edited,
  // indexed by *IConnectionPointContainer* pointer.
  CAdviseMap    m_mapAdvise;
  // Contains arrays of *IUnknown* pointers that are QI'd to the associated
  // *IID*.
  CUnkVectorMap m_mapInterfaces;
  // Indicates that processing is nested within the OnInitDialogHandler
  // method.
  bool          m_bInitializing : 1;
  // Indicates that processing is nested within the UpdateFields method.
  bool          m_bUpdating : 1;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Disconnects all advisory connections.
//
// This method is used by the implementation to disconnect all advisory
// connections made on the set of objects being viewed/edited.
//
// Prior to disconnecting the advisory connections, the OnClearAllAdvises
// virtual override is called to allow a derived class to disconnect any
// connections specific to that class.
//
// See Also: TCPropertyPageBase::OnClearAllAdvises,
// TCPropertyPageBase::SetAllAdvises
inline void TCPropertyPageBase::ClearAllAdvises()
{
  // Allow derived classes to clear advises
  OnClearAllAdvises();

  // Clearing the map will do automatic cleanup of advise objects
  m_mapAdvise.clear();
}


/////////////////////////////////////////////////////////////////////////////