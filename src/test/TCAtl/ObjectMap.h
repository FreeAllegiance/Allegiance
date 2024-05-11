#pragma once

/////////////////////////////////////////////////////////////////////////////
// Object Map Macros

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // Macro Group: TC_OBJECT Object Map Macros
  //
  // Declaration:
  // #define TC_OBJECT_EXTERN(theClass)
  // #define TC_OBJECT_EXTERN_IMPL(theClass)
  // #define TC_OBJECT_ENTRY(clsid, theClass)
  // #define TC_OBJECT_ENTRY_STD(theName)
  //
  // Parameters:
  //   theClass - The name of the class that implements the component object.
  //   clsid - The CLSID of the component object.
  //   theName - The simple name of the component object. See the Remarks.
  //
  // Remarks:
  // The ATL framework uses the concept of an /Object/ /Map/ to associated a
  // component object CLSID with the static class method that is used to
  // create an instance of the C++ class that implements the object. The
  // object map is also used for automatic registration and unregistration of
  // each object listed. To simplify the maintenance of the object map, ATL
  // provides the *BEGIN_OBJECT_MAP*, *END_OBJECT_MAP*, *OBJECT_ENTRY*, and
  // *OBJECT_MAP_NONCREATEABLE* macros. The object map is declared with the
  // former two macros, and one *OBJECT_ENTRY* or
  // *OBJECT_MAP_NONCREATEABLE* macro is instantiated inside the map for
  // each component object implemented by the EXE or DLL module.
  //
  // The problem with the ATL *OBJECT_ENTRY* macros are that they refer to
  // *static* members of the component object C++ class. Because of this, it
  // is necessary for the CPP file that declares the object map to include
  // the H file declaration of each class. For a module that contains very
  // few component objects, this does not pose a great problem. But, for
  // modules that implement *many* component objects, the compile time for
  // the module's main CPP file (the one that declares the object map)
  // becomes extremely unbearable. Especially when you consider that the file
  // is dependent upon /every/ header file it includes, meaning that it must
  // be re-compiled even when only one of the header files changes. In
  // addition, I have encountered compiler heap limits in a DLL that had more
  // than 38 (or so) component objects.
  //
  // Upon close examination of the problem, I realized that *the* *only* „
  // *reason* the module's CPP file needs to include each class's H file is
  // to resolve the static member references. For global declarations, this
  // is easily resolved by declaring the global variables as *extern* in the
  // file that references them. A few attempts at declaring the
  // *static* members as *extern* proved futile. Either the C++ language
  // doesn't support it, the compiler doesn't support it, or my own
  // familiarity with such syntax is lacking. Regardless of the reason, the
  // *extern* global variable approach was sure to work. From this idea, the
  // TC_OBJECT macros were developed.
  //
  // To begin using the macros, first take a look at the module's main CPP
  // file (the one that declares the object map). Instead of including the
  // header file of each component object class declaration, use the
  // TC_OBJECT_EXTERN macro to declare the global variables as external:
  //
  //      // #include "TCCodecAddress.h"    <-- comment-out/remove this line
  //      …
  //      // External Declarations          <-- Add this comment section
  //      …
  //      TC_OBJECT_EXTERN(CTCCodecAddress) // <-- Add this declaration
  //
  // Then, replace the *OBJECT_ENTRY* macro declaration with the
  // TC_OBJECT_ENTRY macro declaration. Notice that the ATL
  // *BEGIN_OBJECT_MAP* and *END_OBJECT_MAP* macros are still used to declare
  // the beginning and end of the object map:
  //
  //      // Object Map
  //      …
  //      BEGIN_OBJECT_MAP(ObjectMap)       // <-- Stays the same
  //        TC_OBJECT_ENTRY(CLSID_TCCodecAddress, CTCCodecAddress)
  //      END_OBJECT_MAP()                  // <-- Stays the same
  //
  // Note: Notice that the two parameters to the TC_OBJECT_ENTRY macros are
  // nearly identical, except for the prefixes. The object CLSID is the
  // simple name prefixed with *CLSID_* and the object class name is the
  // simple name prefixed with the letter *C*. When this is the case, the
  // following is an alternate way to list the object in the map, using the
  // TC_OBJECT_ENTRY_STD macro:
  //
  //        TC_OBJECT_ENTRY_STD(TCCodecAddress)
  //
  // Now that the module's main CPP file references some external global
  // variables, the global variables have to actually be declared somewhere,
  // or else linker errors will result. The most logical place to put the
  // declarations is in the CPP file of the component object implementation:
  //
  //        // Near the top of TCCodecAddress.cpp
  //        …
  //        // CTCCodecAddress
  //        …
  //        TC_OBJECT_EXTERN_IMPL(CTCCodecAddress)
  //
  // You would then repeat these steps for each component object implemented
  // by your module.
  //
  // Note: Since four static function pointers are declared global for each
  // TC_OBJECT_ENTRY or TC_OBJECT_ENTRY_STD macro used, this approach adds a
  // total of 16 bytes to the size of the module's global data segment for
  // each component object. This is hardly significant and the reduced
  // compile time is a fair tradeoff, but it does deserve to be noted here.
  // This may lead one to think that the macros should only be implemented
  // using the *extern* globals under _DEBUG builds; that release build
  // should use the stock ATL implementations. However, this still does not
  // address the problem of hitting the compiler heap limits after so many
  // object header files are included. So, it's probably best to leave the
  // macros the same for both _DEBUG and release builds.
  //
  // See Also: *BEGIN_OBJECT_MAP*, *END_OBJECT_MAP* „
  #define TC_OBJECT
#endif // _DOCJET_ONLY

/////////////////////////////////////////////////////////////////////////////
// {secret}
typedef HRESULT (WINAPI TC_UPDATEREGFUNC)(BOOL bRegister);
typedef void (WINAPI TC_ObjectMainFunc)(bool bStarting);

/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_EXTERN(theClass)                                          \
  extern TC_UPDATEREGFUNC*     g_pfn##theClass##_UpdateRegistry;            \
  extern _ATL_CREATORFUNC*     g_pfn##theClass##_ClassFactoryCreator;       \
  extern _ATL_CREATORFUNC*     g_pfn##theClass##_Creator;                   \
  extern _ATL_DESCRIPTIONFUNC* g_pfn##theClass##_Description;               \
  extern _ATL_CATMAPFUNC*      g_pfn##theClass##_GetCategoryMap;            \
  extern TC_ObjectMainFunc*    g_pfn##theClass##_ObjectMain;

/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_EXTERN_NON_CREATEABLE(theClass)                           \
  extern TC_UPDATEREGFUNC*     g_pfn##theClass##_UpdateRegistry;            \
  extern _ATL_CATMAPFUNC*      g_pfn##theClass##_GetCategoryMap;            \
  extern TC_ObjectMainFunc*    g_pfn##theClass##_ObjectMain;

/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_EXTERN_IMPL(theClass)                                     \
  TC_UPDATEREGFUNC*     g_pfn##theClass##_UpdateRegistry =                  \
    &theClass::UpdateRegistry;                                              \
  _ATL_CREATORFUNC*     g_pfn##theClass##_ClassFactoryCreator =             \
    &theClass::_ClassFactoryCreatorClass::CreateInstance;                   \
  _ATL_CREATORFUNC*     g_pfn##theClass##_Creator =                         \
    &theClass::_CreatorClass::CreateInstance;                               \
  _ATL_DESCRIPTIONFUNC* g_pfn##theClass##_Description =                     \
    &theClass::GetObjectDescription;                                        \
  _ATL_CATMAPFUNC*      g_pfn##theClass##_GetCategoryMap =                  \
    &theClass::GetCategoryMap;                                              \
  TC_ObjectMainFunc*    g_pfn##theClass##_ObjectMain =                      \
    &theClass::ObjectMain;

/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(theClass)                      \
  TC_UPDATEREGFUNC*     g_pfn##theClass##_UpdateRegistry =                  \
    &theClass::UpdateRegistry;                                              \
  _ATL_CATMAPFUNC*      g_pfn##theClass##_GetCategoryMap =                  \
    &theClass::GetCategoryMap;                                              \
  TC_ObjectMainFunc*    g_pfn##theClass##_ObjectMain =                      \
    &theClass::ObjectMain;

/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_ENTRY(clsid, theClass)                                    \
  {                                                                         \
    &clsid,                                                                 \
    g_pfn##theClass##_UpdateRegistry,                                       \
    g_pfn##theClass##_ClassFactoryCreator,                                  \
    g_pfn##theClass##_Creator,                                              \
    NULL,                                                                   \
    0,                                                                      \
    g_pfn##theClass##_Description,                                          \
    g_pfn##theClass##_GetCategoryMap,                                       \
    g_pfn##theClass##_ObjectMain                                            \
  },


/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_ENTRY_NON_CREATEABLE(clsid, theClass)                     \
  {                                                                         \
    &clsid,                                                                 \
    g_pfn##theClass##_UpdateRegistry,                                       \
    NULL,                                                                   \
    NULL,                                                                   \
    NULL,                                                                   \
    0,                                                                      \
    NULL,                                                                   \
    g_pfn##theClass##_GetCategoryMap,                                       \
    g_pfn##theClass##_ObjectMain                                            \
  },


/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_ENTRY_STD(theName)                                        \
  TC_OBJECT_ENTRY(CLSID_##theName, C##theName)


/////////////////////////////////////////////////////////////////////////////
// {partof:TC_OBJECT}
#define TC_OBJECT_ENTRY_STD_NON_CREATEABLE(theName)                         \
  TC_OBJECT_ENTRY_NON_CREATEABLE(CLSID_##theName, C##theName)


/////////////////////////////////////////////////////////////////////////////