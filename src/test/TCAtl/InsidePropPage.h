#pragma once

/////////////////////////////////////////////////////////////////////////////
// InsidePropPage.h | Declaration of the TCInsidePropPage class.

#include "..\TCLib\PropertyPageSite.h"


/////////////////////////////////////////////////////////////////////////////
// TCInsidePropPage allows a property page to be embedded within another
// property page. This ability encourages the reuse of common user interface
// elements.
//
// TCInsidePropPage is used internally by the TCPropertyPageBase class to
// further simplify the task of embedding property pages within another.
//
// See Also: TCPropertyPageSite, TCPropertyPageBase, TCPropertyPageImpl
class TCInsidePropPage :
  public TCPropertyPageSite,
  public CWindowImpl<TCInsidePropPage>
{
// Declarations
public:
  DECLARE_WND_SUPERCLASS(NULL, WC_DIALOG)

// Message Map
public:
  BEGIN_MSG_MAP(TCInsidePropPage)
    MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessage)
  END_MSG_MAP()

// Construction
public:
  TCInsidePropPage();
  bool Create(HWND hwndParent, IPropertyPageSite* pPageSite, UINT idPosCtrl,
    REFCLSID clsid, bool bVisible, bool bSizeToCtrl = true,
    bool bDelCtrl = true);

// Overrides
protected:
  virtual HRESULT OnStatusChange(DWORD dwFlags);
  virtual HRESULT OnGetLocaleID(LCID* pLocaleID);
  virtual HRESULT OnGetPageContainer(IUnknown** ppUnk);
  virtual HRESULT OnTranslateAccelerator(MSG* pMsg);
  virtual BOOL    OnIsModal();
  virtual HWND    OnGetWindow();

// Message Handlers
protected:
  LRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// Group=Data Members
protected:
  // A smart pointer that maintains a reference on the
  // *IPropertyPageSite* interface pointer specified in the Create method.
  //
  // See Also: TCInsidePropertyPage::Create
  IPropertyPageSitePtr m_pPageSite;
};


/////////////////////////////////////////////////////////////////////////////