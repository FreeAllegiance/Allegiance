#ifndef __PageEntry_h__
#define __PageEntry_h__

/////////////////////////////////////////////////////////////////////////////
// PageEntry.h | Declaration of the XInsidePageEntry and XInsidePage classes
// used solely by the TCPropertyPageImpl class.
//
// Originally, I tried embedding these within the TCPropertyPageImpl class,
// but since it is a template class, the compiler messages were to too
// unbearable to read.
//


/////////////////////////////////////////////////////////////////////////////
// XInsidePageEntry declares the format of one entry in a table of embedded
// property pages. This structure is used soley by the TCPropertyPageBase
// class. See the Embedded Property Page Table Macros for more information
// about the purpose of each structure member.
//
// Originally, this structure was embedded within the TCPropertyPageImpl
// class, but since it is a template class, the compiler messages were to too
// unbearable to read.
//
// TODO: Now that the embedded property page features of TCPropertyPageImpl
// have been moved into the non-template TCPropertyPageBase class, this
// structure should perhaps be moved there.
//
// See Also: Embedded Property Page Table Macros, TCPropertyPageBase,
// TCPropertyPageImpl
struct XInsidePageEntry
{
  UINT         idPosCtrl;         // {secret}
  const CLSID* pclsid;            // {secret}
  DWORD        dwData;            // {secret}
  LPCTSTR      pszText;           // {secret}
  bool         bVisible : 1;      // {secret}
  bool         bSizeToCtrl : 1;   // {secret}
  bool         bAlwaysApply : 1;  // {secret}
};


/////////////////////////////////////////////////////////////////////////////
// XInsidePage is a derivation of TCInsidePropPage that simply adds a pointer
// to the table entry structure. This class is used solely by the
// TCPropertyPageBase class.
//
// Originally, this class was embedded within the TCPropertyPageImpl class,
// but since it is a template class, the compiler messages were to too
// unbearable to read.
//
// TODO: Now that the embedded property page features of TCPropertyPageImpl
// have been moved into the non-template TCPropertyPageBase class, this
// class should perhaps be moved there.
//
// See Also: TCPropertyPageBase, TCPropertyPageImpl, TCInsidePropPage,
// XInsidePageEntry, Embedded Property Page Table Macros
class XInsidePage : public TCInsidePropPage
{
// Construction
public:
  XInsidePage(const XInsidePageEntry* pEntry);

// Group=Data Members
public:
  // A pointer to the table entry used to create this property page.
  const XInsidePageEntry* m_pEntry;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// {group:Construction}
//
// Constructs an instance of the object by copying the specified table entry
// pointer.
//
// Parameters:
//   pEntry - A pointer to the table entry used to create this property page.
//
// See Also: XInsidePage::m_pEntry
inline XInsidePage::XInsidePage(const XInsidePageEntry* pEntry)
  : m_pEntry(pEntry)
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// {tag:Embedded Property Page Table Macros}
// Macro Group: Embedded Property Page Table Macros
//
// Declaration:
// #define BEGIN_INSIDE_PAGE_MAP()
// #define ALT_INSIDE_PAGE_MAP(pageMapID)
// #define INSIDE_PAGE_ENTRY_EX(id, clsid, dw, bVis, bSize, bApply)
// #define INSIDE_PAGE_ENTRY_EX_TEXT(id, clsid, dw, bVis, bSize, bApply, t)
// #define INSIDE_PAGE_ENTRY_EX_STRID(id, clsid, dw, bVis, bSize, bApply, t)
// #define INSIDE_PAGE_ENTRY(id, clsid, dw)
// #define INSIDE_PAGE_ENTRY_TEXT(id, clsid, dw, t)
// #define INSIDE_PAGE_ENTRY_STRID(id, clsid, dw, t)
// #define INSIDE_PAGEGROUP_ENTRY(id, clsid, dw, bApply)
// #define INSIDE_PAGEGROUP_ENTRY_TEXT(id, clsid, dw, t, bApply)
// #define INSIDE_PAGEGROUP_ENTRY_STRID(id, clsid, dw, t, bApply)
// #define END_INSIDE_PAGE_MAP()
//
// Parameters:
//   pageMapID - A non-zero identifier for the next section of the map. See
// the remarks for how this would be used.
//   id - The child window identifier in the property page's main dialog that
// represents the control used to position the embedded property page.
//   clsid - The *CLSID* of the embedded property page component object.
//   dw - A DWORD (32-bit) value to associate with the embedded property
// page.
//   bVis - Specifies the embedded property page's initial visibility.
// *true* to make the page initially visible, otherwise *false*.
//   bSize - When *true*, the embedded property page will be made the same
// size as the control specified in /id/. Otherwise, *false* specifies that
// the page will be the size indicated by its
// *IPropertyPage::GetPageInfo* method.
//   bApply - When *true*, specifies that the embedded property page's
// *IPropertyPage::Apply* method should be called whenever the outer page's
// changes are applied, /even/ /if/ /no/ /changes/ /were/ /detected/ /in/ „
// /the/ /embedded/ /page/. When *false*, the embedded property page's Apply
// method will only be called when changes have been detected.
//   t - A text string to associate with the embedded property page.
//
// Remarks: These macros provide a simple mechanism for defining a map
// (table) of property pages that are to be embedded within another property
// page. Use these macros within the declaration of classes derived from
// TCPropertyPageImpl.
//
// BEGIN_INSIDE_PAGE_MAP and END_INSIDE_PAGE_MAP define the beginning and end
// of the table.
//
// The INSIDE_PAGE_ENTRY group of macros declare a single property page to be
// embedded within the outer page. See the parameters for the variations.
//
// The INSIDE_PAGEGROUP_ENTRY macros declare a single property page
// /within/ /a/ /mutually/ /exclusive/ /group/ of property pages that all
// share the same position in the outer property page. This is useful, for
// example, when some other control on the outer property page (such as a
// combo box or group of radio buttons) manages the visibility of the pages
// of this group, based on its current selection. When these macros are used,
// the /id/ parameter is used to identify the positioning control and also
// serves as the group's identifier.
//
// For both of the above sets of macros, the /dw/ and /t/ parameters may be
// used for any application-defined purpose.
//
// The ALT_INSIDE_PAGE_MAP macro is used when more than one set of embedded
// property pages needs to be defined. This macro is for advanced use only
// and should not be used without a full understanding of its implementation.
// This macro is not needed for most property pages.
//
// See Also: TCPropertyPageImpl, TCInsidePropPage::Create,
// Property Page Field Update Macros
#define Embedded_Property_Page_Table_Macros

// {partof:Embedded_Property_Page_Table_Macros}
#define BEGIN_INSIDE_PAGE_MAP()                                             \
public:                                                                     \
  virtual UINT GetInsidePageTable(const XInsidePageEntry** ppTable,         \
    UINT iTable)                                                            \
  {                                                                         \
    switch (iTable)                                                         \
    {                                                                       \
      case 0:                                                               \
      {                                                                     \
        static const XInsidePageEntry table[] =                             \
        {

// {partof:Embedded_Property_Page_Table_Macros}
#define ALT_INSIDE_PAGE_MAP(pageMapID)                                      \
          {0, NULL, 0, NULL, false, false, false}                           \
        };                                                                  \
        if (ppTable)                                                        \
          *ppTable = table;                                                 \
        return sizeofArray(table) - 1;                                      \
      }                                                                     \
    case pageMapID:                                                         \
      {                                                                     \
        static const XInsidePageEntry table[] =                             \
        {

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY_EX(id, clsid, dw, bVis, bSize, bApply)            \
          {id, &clsid, DWORD(dw), NULL, bVis, bSize, bApply},

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY_EX_TEXT(id, clsid, dw, bVis, bSize, bApply, t)    \
          {id, &clsid, DWORD(dw), TEXT(t), bVis, bSize, bApply},

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY_EX_STRID(id, clsid, dw, bVis, bSize, bApply, t)   \
          {id, &clsid, DWORD(dw), MAKEINTRESOURCE(t), bVis, bSize, bApply},

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY(id, clsid, dw)                                    \
          INSIDE_PAGE_ENTRY_EX(id, clsid, dw, true, true, true)

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY_TEXT(id, clsid, dw, t)                            \
          INSIDE_PAGE_ENTRY_EX_TEXT(id, clsid, dw, t, true, true, true)

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGE_ENTRY_STRID(id, clsid, dw, t)                           \
          INSIDE_PAGE_ENTRY_EX_STRID(id, clsid, dw, t, true, true, true)

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGEGROUP_ENTRY(id, clsid, dw, bApply)                       \
          INSIDE_PAGE_ENTRY_EX(id, clsid, dw, false, true, bApply)

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGEGROUP_ENTRY_TEXT(id, clsid, dw, t, bApply)               \
          INSIDE_PAGE_ENTRY_EX_TEXT(id, clsid, dw, t, false, true, bApply)

// {partof:Embedded_Property_Page_Table_Macros}
#define INSIDE_PAGEGROUP_ENTRY_STRID(id, clsid, dw, t, bApply)              \
          INSIDE_PAGE_ENTRY_EX_STRID(id, clsid, dw, t, false, true, bApply)

// {partof:Embedded_Property_Page_Table_Macros}
#define END_INSIDE_PAGE_MAP()                                               \
          {0, NULL, 0, NULL, false, false, false}                           \
        };                                                                  \
        if (ppTable)                                                        \
          *ppTable = table;                                                 \
        return sizeofArray(table) - 1;                                      \
      }                                                                     \
    }                                                                       \
    _TRACE1("Invalid inside property page map ID (%i)\n", iTable);          \
    ZAssert(FALSE);                                                        \
    if (ppTable)                                                            \
      *ppTable = NULL;                                                      \
    return 0;                                                               \
  }


/////////////////////////////////////////////////////////////////////////////
// {tag:Property Page Field Update Macros}
// Macro Group: Property Page Field Update Macros
//
// Declaration:
// #define BEGIN_PAGE_FIELD_MAP()
// #define PAGE_FIELD_ENTRY_EX(idCtrl, dispid, iid, fnUpdate, fnApply)
// #define PAGE_FIELD_ENTRY_EX_RO(idCtrl, dispid, iid, fnUpdate)
// #define PAGE_FIELD_ENTRY(idCtrl, dispid, iid_name, fnName)
// #define PAGE_FIELD_ENTRY_RO(idCtrl, dispid, iid_name, fnName)
// #define PAGE_INTERFACE_ENTRY_OLD(iid_name)
// #define PAGE_INTERFACE_ENTRY(iid_name, fnName)
// #define PAGE_INTERFACE_ENTRY_RO(iid_name, fnName)
// #define END_PAGE_FIELD_MAP()
//
// Parameters:
//   idCtrl - Identifier of a control on the property page used to display and/or
// edit this field.
//   dispid - Dispatch identifier of this field.
//   iid - The *IID* (interface identifier) of the interface on which the
// specified /dispid/ identifies this field.
//   iid_name - Same as /iid/, but without the *IID_* prefix.
//   fnUpdate - The name of the member function called to update the property
// page child window(s) with the value of the field.
//   fnApply - The name of the member function called to apply the changes
// made to the property page child windows(s) to the object.
//   fnName - The base name of the *Update* and *Apply* handler member
// functions, as described in /fnUpdate/ and /fnApply/. This base name is
// prefixed with *Apply* and *Update* to refer to the two functions.
//
// Remarks: These macros provide a simple mechanism for mapping the fields
// (properties) of an interface to functions that update the user interface
// with the field value and apply user interface changes to the field value.
// Use these macros within the declaration of classes derived from
// TCPropertyPageImpl.
//
// BEGIN_PAGE_FIELD_MAP and END_PAGE_FIELD_MAP define the beginning and end
// of the table.
//
// The PAGE_FIELD_ENTRY macros associate a field with the functions used to
// update the UI with the field value and apply the UI changes to the field
// value. Although, the /idCtrl/ parameter is also associated with the field,
// it currently does not get used by the base class.
//
// The *Update* and *Apply* handler member functions that you declare in your
// class should be prototyped as follows:
//
//        void UpdateProc(ULONG cObjects, IUnknown** ppunkObjects);
//        void ApplyProc(ULONG cObjects, IUnknown** ppunkObjects);
//
// Note: The macros that have *EX* in their names use the following
// /extended/ function prototypes. Use these macros (and prototypes) when the
// *Update* and *Apply* handlers need more information about which field is to
// be updated or applied.
//
//        void UpdateProcEx(UINT id, DISPID dispid, REFIID riid,
//          ULONG cObjects, IUnknown** ppunkObject);
//        void ApplyProcEx(UINT id, DISPID dispid, REFIID riid,
//          ULONG cObjects, IUnknown** ppunkObject);
//
// Although the normal and extended prototypes shown above specify arrays of
// *IUnknown* pointers, the pointers have already been QI'd for the IID
// specified in the /iid/ or /iid_name/ parameter. So, the *Update* and
// *Apply* handlers need not *QueryInterface* them again. Instead, your
// prototypes should specify an array of the /actual/ interface name pointers,
// as in the following example:
//
//        // Property Page Field Map
//        public:
//          BEGIN_PAGE_FIELD_MAP()
//            PAGE_FIELD_ENTRY(IDC_AllowIt, DISPID_AllowIt, IMyFace, AllowIt)
//          END_PAGE_FIELD_MAP()
//        …
//        // Property Field Exchange
//        protected:
//          void UpdateAllowIt(ULONG cObjects, IMyFace** ppObjects);
//          void ApplyAllowIt(ULONG cObjects, IMyFace** ppObjects);
//
// In most property pages, the *Update* and *Apply* handlers should be the
// only place in code that either initializes (Update) or saves (Apply)
// property values. This is supported in the TCPropertyPageImpl base class.
// For example, when the property page is about to first become visible, the
// base class handles the *WM_INITDIALOG* message by calling the
// *Update* handler for each field entry in the map. It also does this when
// the *IPropertyPage::SetObjects* method is called, once the page is already
// visible. This allows the code that connects an object property to a user
// interface element to only be coded in one place.
//
// Note: This mechanism also gets reused if the object being edited by the
// property page supports the *IPropertyNotifySink* connection point. For
// such objects, when notification of a property change is received, the
// *Update* method is called to reflect the change in the user interface.
//
// TODO: The intention of this design may not fully be realized. For example,
// the *Update* and *Apply* handlers should not need to be concerned with
// exception handling. The base class should handle exceptions at its level.
// This may be in place now, but it should be reviewed to ensure that
// exceptions are being handled in such a way.
//
// The PAGE_INTERFACE_ENTRY macros represent a carry-over from the original
// implementation of the TCPropertyPageImpl base class, which was
// /interface/ based rather than /field/ based. New code should (probably)
// not use these macros. Use the PAGE_FIELD_ENTRY macros instead.
//
// All the macro variants that end with *RO* should be used when a field is
// /read-only/. This simplifies programming by not having to declare and
// implement an *Apply* handler that does nothing.
//
// See Also: TCPropertyPageImpl, Embedded Property Page Table Macros
#define Property_Page_Field_Update_Macros

// {partof:Property_Page_Field_Update_Macros}
#define BEGIN_PAGE_FIELD_MAP()                                              \
public:                                                                     \
  UINT GetPageFieldTable(const XPageFieldEntry** ppTable)                   \
  {                                                                         \
    static const XPageFieldEntry table[] =                                  \
    {

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_FIELD_ENTRY_EX(id, dispid, iid, fnUpdate, fnApply)             \
      {id, dispid, &iid, NULL, NULL, fnUpdate, fnApply},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_FIELD_ENTRY_EX_RO(id, dispid, iid, fnUpdate)                   \
      {id, dispid, &iid, NULL, NULL, fnUpdate, NULL},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_FIELD_ENTRY(id, dispid, iid_name, name)                        \
      {id, dispid, &IID_##iid_name, XFieldUpdateProc(Update##name),         \
        XFieldApplyProc(Apply##name), NULL, NULL},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_FIELD_ENTRY_RO(id, dispid, iid_name, name)                     \
      {id, dispid, &IID_##iid_name, XFieldUpdateProc(Update##name), NULL,   \
        NULL, NULL},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_INTERFACE_ENTRY_OLD(iid_name)                                  \
      {0, 0, &IID_##iid_name, NULL, NULL, NULL, NULL},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_INTERFACE_ENTRY(iid_name, name)                                \
      {0, 0, &IID_##iid_name, XFieldUpdateProc(Update##name),               \
        XFieldApplyProc(Apply##name), NULL, NULL},

// {partof:Property_Page_Field_Update_Macros}
#define PAGE_INTERFACE_ENTRY_RO(iid_name, name)                             \
      {0, 0, &IID_##iid_name, XFieldUpdateProc(Update##name), NULL},

// {partof:Property_Page_Field_Update_Macros}
#define END_PAGE_FIELD_MAP()                                                \
      {0, 0, NULL, NULL, NULL}                                              \
    };                                                                      \
    if (ppTable)                                                            \
      *ppTable = table;                                                     \
    return sizeofArray(table) - 1;                                          \
  }


/////////////////////////////////////////////////////////////////////////////

#endif // !__PageEntry_h__
