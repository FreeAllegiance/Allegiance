#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#ifndef __WotLib_h__
#define __WotLib_h__

#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////////
// WotLib.h | Declaration of the TCWotLib class used to wrap the
// calling of functions from a single window thread for use with libraries
// which require a single thread of execution.
//

/////////////////////////////////////////////////////////////////////////////
// Encapsulates the use of a DLL's exported functions through a window's
// message thread.
//
// To use this class, first derive your own class from it.  Then, in the
// declaration of your derived class, use the TCWotLib_Fn macros to declare
// the function prototypes of the API exports that you wish to expose from
// your derived class.
//
class TCWotLib
{
// Group=Construction
public:
  TCWotLib();
  TCWotLib(HWND hWnd);
  void SetWnd(HWND hWnd);

// Group=Data Members
protected:
  HWND m_hWnd;    // The window which will receive function msg requests.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

inline TCWotLib::TCWotLib() :
  m_hWnd(NULL)
{
}

inline TCWotLib::TCWotLib(HWND hWnd) :
  m_hWnd(hWnd)
{
}

inline void TCWotLib::SetWnd(HWND hWnd)
{
  m_hWnd = hWnd;
}


/////////////////////////////////////////////////////////////////////////////
// Internal Helper macros

#define TCWOTLIB_HANDLER(Name) \
    MESSAGE_HANDLER(wm_##Name, On##Name) \

/////////////////////////////////////////////////////////////////////////////
// Internal Helper macros

#define TCWotCall(retType, Name,nArgs) \
    return (retType)::SendMessage(m_hWnd, wm_##Name, (WPARAM)&p1, nArgs)

#define TCWotCall_void(Name,nArgs) \
    ::SendMessage(m_hWnd, wm_##Name, (WPARAM)&p1, nArgs)

#define TCWotPreCall(retType, pre, Name, nArgs) \
    return (retType)::SendMessage(m_hWnd, wm_##pre##Name, (WPARAM)&p1, nArgs)

#define TCWotPreCall_void(pre, Name, nArgs) \
    ::SendMessage(m_hWnd, wm_##pre##Name, (WPARAM)&p1, nArgs)

/////////////////////////////////////////////////////////////////////////////
// Non-prefixed functions with return-value

#define TCWotLib_Fn0(retType, Name)                                         \
  retType Name(void)                                                        \
  { return (retType)::SendMessage(m_hWnd, wm_##Name, 0, 0L); }

#define TCWotLib_Fn1(retType, Name, T1)                                     \
  retType Name(T1 p1)                                                       \
  { TCWotCall(retType, Name, 1); }

#define TCWotLib_Fn2(retType, Name, T1,T2)                                  \
  retType Name(T1 p1,T2)                                                    \
  { TCWotCall(retType, Name, 2); }

#define TCWotLib_Fn3(retType, Name, T1,T2,T3)                               \
  retType Name(T1 p1,T2,T3)                                                 \
  { TCWotCall(retType, Name, 3); }

#define TCWotLib_Fn4(retType, Name, T1,T2,T3,T4)                            \
  retType Name(T1 p1,T2,T3,T4)                                              \
  { TCWotCall(retType, Name, 4); }

#define TCWotLib_Fn5(retType, Name, T1,T2,T3,T4,T5)                         \
  retType Name(T1 p1,T2,T3,T4,T5)                                           \
  { TCWotCall(retType, Name, 5); }

#define TCWotLib_Fn6(retType, Name, T1,T2,T3,T4,T5,T6)                      \
  retType Name(T1 p1,T2,T3,T4,T5,T6)                                        \
  { TCWotCall(retType, Name, 6); }

#define TCWotLib_Fn7(retType, Name, T1,T2,T3,T4,T5,T6,T7)                   \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7)                                     \
  { TCWotCall(retType, Name, 7); }

#define TCWotLib_Fn8(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8)                \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8)                                  \
  { TCWotCall(retType, Name, 8); }

#define TCWotLib_Fn9(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9)             \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9)                               \
  { TCWotCall(retType, Name, 9); }

#define TCWotLib_Fn10(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10)        \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10)                           \
  { TCWotCall(retType, Name, 10); }

#define TCWotLib_Fn11(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11)    \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11)                       \
  { TCWotCall(retType, Name, 11); }

#define TCWotLib_Fn12(retType, Name,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12)                   \
  { TCWotCall(retType, Name, 12); }

/////////////////////////////////////////////////////////////////////////////
// Non-prefixed void functions

#define TCWotLib_Fn0_void(retType, Name)                                    \
  void Name(void)                                                           \
  { ::SendMessage(m_hWnd, wm_##Name, 0, 0L); }

#define TCWotLib_Fn1_void(retType, Name, T1)                                \
  void Name(T1 p1)                                                          \
  { TCWotCall_void(Name, 1); }

#define TCWotLib_Fn2_void(retType, Name, T1,T2)                             \
  void Name(T1 p1,T2)                                                       \
  { TCWotCall_void(Name, 2); }

#define TCWotLib_Fn3_void(retType, Name, T1,T2,T3)                          \
  void Name(T1 p1,T2,T3)                                                    \
  { TCWotCall_void(Name, 3); }

#define TCWotLib_Fn4_void(retType, Name, T1,T2,T3,T4)                       \
  void Name(T1 p1,T2,T3,T4)                                                 \
  { TCWotCall_void(Name, 4); }

#define TCWotLib_Fn5_void(retType, Name, T1,T2,T3,T4,T5)                    \
  void Name(T1 p1,T2,T3,T4,T5)                                              \
  { TCWotCall_void(Name, 5); }

#define TCWotLib_Fn6_void(retType, Name, T1,T2,T3,T4,T5,T6)                 \
  void Name(T1 p1,T2,T3,T4,T5,T6)                                           \
  { TCWotCall_void(Name, 6); }

#define TCWotLib_Fn7_void(retType, Name, T1,T2,T3,T4,T5,T6,T7)              \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7)                                        \
  { TCWotCall_void(Name, 7); }

#define TCWotLib_Fn8_void(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8)           \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8)                                     \
  { TCWotCall_void(Name, 8); }

#define TCWotLib_Fn9_void(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9)        \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9)                                  \
  { TCWotCall_void(Name, 9); }

#define TCWotLib_Fn10_void(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10)   \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10)                              \
  { TCWotCall_void(Name, 10); }

#define TCWotLib_Fn11_void(retType, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11)                          \
  { TCWotCall_void(Name, 11); }

#define TCWotLib_Fn12_void(retType, Name,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12)                      \
  { TCWotCall_void(Name, 12); }

/////////////////////////////////////////////////////////////////////////////
// Prefixed functions with return-value

#define TCWotLib_PreFn0(retType, pre, Name)                                 \
  retType Name(void)                                                   \
  { return (retType)::SendMessage(m_hWnd, wm_##pre##Name, 0, 0L); }

#define TCWotLib_PreFn1(retType, pre, Name, T1)                             \
  retType Name(T1 p1)                                                  \
  { TCWotPreCall(retType, pre, Name, 1); }

#define TCWotLib_PreFn2(retType, pre, Name, T1,T2)                          \
  retType Name(T1 p1,T2)                                               \
  { TCWotPreCall(retType, pre, Name, 2); }

#define TCWotLib_PreFn3(retType, pre, Name, T1,T2,T3)                       \
  retType Name(T1 p1,T2,T3)                                            \
  { TCWotPreCall(retType, pre, Name, 3); }

#define TCWotLib_PreFn4(retType, pre, Name, T1,T2,T3,T4)                    \
  retType Name(T1 p1,T2,T3,T4)                                         \
  { TCWotPreCall(retType, pre, Name, 4); }

#define TCWotLib_PreFn5(retType, pre, Name, T1,T2,T3,T4,T5)                 \
  retType Name(T1 p1,T2,T3,T4,T5)                                      \
  { TCWotPreCall(retType, pre, Name, 5); }

#define TCWotLib_PreFn6(retType, pre, Name, T1,T2,T3,T4,T5,T6)              \
  retType Name(T1 p1,T2,T3,T4,T5,T6)                                   \
  { TCWotPreCall(retType, pre, Name, 6); }

#define TCWotLib_PreFn7(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7)           \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7)                                \
  { TCWotPreCall(retType, pre, Name, 7); }

#define TCWotLib_PreFn8(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8)        \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8)                             \
  { TCWotPreCall(retType, pre, Name, 8); }

#define TCWotLib_PreFn9(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9)     \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9)                          \
  { TCWotPreCall(retType, pre, Name, 9); }

#define TCWotLib_PreFn10(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10)                       \
  { TCWotPreCall(retType, pre, Name, 10); }

#define TCWotLib_PreFn11(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11)                  \
  { TCWotPreCall(retType, pre, Name, 11); }

#define TCWotLib_PreFn12(retType, pre, Name,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  retType Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12)              \
  { TCWotPreCall(retType, pre, Name, 12); }

/////////////////////////////////////////////////////////////////////////////
// Prefixed void functions

#define TCWotLib_PreFn0_void(pre, Name)                             \
  void Name(void)                                                      \
  { ::SendMessage(m_hWnd, wm_##pre##Name, 0, 0L); }

#define TCWotLib_PreFn1_void(pre, Name, T1)                         \
  void Name(T1 p1)                                                     \
  { TCWotPreCall_void(pre, Name, 1); }

#define TCWotLib_PreFn2_void(pre, Name, T1,T2)                  \
  void Name(T1 p1,T2)                                                  \
  { TCWotPreCall_void(pre, Name, 2); }

#define TCWotLib_PreFn3_void(pre, Name, T1,T2,T3)                   \
  void Name(T1 p1,T2,T3)                                               \
  { TCWotPreCall_void(pre, Name, 3); }

#define TCWotLib_PreFn4_void(pre, Name, T1,T2,T3,T4)                \
  void Name(T1 p1,T2,T3,T4)                                            \
  { TCWotPreCall_void(pre, Name, 4); }

#define TCWotLib_PreFn5_void(pre, Name, T1,T2,T3,T4,T5)             \
  void Name(T1 p1,T2,T3,T4,T5)                                         \
  { TCWotPreCall_void(pre, Name, 5); }

#define TCWotLib_PreFn6_void(pre, Name, T1,T2,T3,T4,T5,T6)      \
  void Name(T1 p1,T2,T3,T4,T5,T6)                                      \
  { TCWotPreCall_void(pre, Name, 6); }

#define TCWotLib_PreFn7_void(pre, Name, T1,T2,T3,T4,T5,T6,T7)       \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7)                                   \
  { TCWotPreCall_void(pre, Name, 7); }

#define TCWotLib_PreFn8_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8)    \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8)                                \
  { TCWotPreCall_void(pre, Name, 8); }

#define TCWotLib_PreFn9_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9)                             \
  { TCWotPreCall_void(pre, Name, 9); }

#define TCWotLib_PreFn10_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10)                         \
  { TCWotPreCall_void(pre, Name, 10); }

#define TCWotLib_PreFn11_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11)                     \
  { TCWotPreCall_void(pre, Name, 11); }

#define TCWotLib_PreFn12_void(pre, Name,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  void Name(T1 p1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12)                 \
  { TCWotPreCall_void(pre, Name, 12); }

/////////////////////////////////////////////////////////////////////////////
// Non-prefixed handlers with return-values

#define TCWotLib_DecHandler0(DynaLib, Name) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(wParam == 0 && lParam == 0);                                    \
    return (LRESULT)DynaLib.Name();                                         \
  }

#define TCWotLib_DecHandler1(DynaLib, Name, T1) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 1);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    return (LRESULT)DynaLib.Name(p1);   \
  }

#define TCWotLib_DecHandler2(DynaLib, Name, T1,T2) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 2);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    return (LRESULT)DynaLib.Name(p1,p2);   \
  }

#define TCWotLib_DecHandler3(DynaLib, Name, T1,T2,T3) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 3);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3);   \
  }

#define TCWotLib_DecHandler4(DynaLib, Name, T1,T2,T3,T4) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 4);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4);   \
  }

#define TCWotLib_DecHandler5(DynaLib, Name, T1,T2,T3,T4,T5) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 5);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5);   \
  }

#define TCWotLib_DecHandler6(DynaLib, Name, T1,T2,T3,T4,T5,T6) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 6);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6);   \
  }

#define TCWotLib_DecHandler7(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 7);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7);   \
  }

#define TCWotLib_DecHandler8(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 8);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8);   \
  }

#define TCWotLib_DecHandler9(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 9);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9);   \
  }

#define TCWotLib_DecHandler10(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 10);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);   \
  }

#define TCWotLib_DecHandler11(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 11);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);   \
  }

#define TCWotLib_DecHandler12(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 12);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    T12 p12 = va_arg(ArgList, T12);                                         \
    return (LRESULT)DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12);   \
  }

/////////////////////////////////////////////////////////////////////////////
// Non-prefixed handlers for void functions

#define TCWotLib_DecHandler0_void(DynaLib, Name) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(wParam == 0 && lParam == 0);                                    \
    DynaLib.Name();                                                         \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler1_void(DynaLib, Name, T1) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 1);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    DynaLib.Name(p1);                                                       \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler2_void(DynaLib, Name, T1,T2) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 2);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    DynaLib.Name(p1,p2);                                                    \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler3_void(DynaLib, Name, T1,T2,T3) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 3);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    DynaLib.Name(p1,p2,p3);                                                 \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler4_void(DynaLib, Name, T1,T2,T3,T4) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 4);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    DynaLib.Name(p1,p2,p3,p4);                                              \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler5_void(DynaLib, Name, T1,T2,T3,T4,T5) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 5);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    DynaLib.Name(p1,p2,p3,p4,p5);                                           \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler6_void(DynaLib, Name, T1,T2,T3,T4,T5,T6) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 6);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    DynaLib.Name(p1,p2,p3,p4,p5,p6);                                        \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler7_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 7);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7);                                     \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler8_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 8);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8);                                  \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler9_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 9);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9);                               \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler10_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 10);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);                           \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler11_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 11);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);                       \
    return 0;                                                               \
  }

#define TCWotLib_DecHandler12_void(DynaLib, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  LRESULT On##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)      \
  {                                                                         \
    ZAssert(lParam == 12);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    T12 p12 = va_arg(ArgList, T12);                                         \
    DynaLib.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12);                   \
    return 0;                                                               \
  }

/////////////////////////////////////////////////////////////////////////////
// Prefixed handlers with return-values

#define TCWotLib_DecPreHandler0(retType, pre, Name) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(wParam == 0 && lParam == 0);                                    \
    return (LRESULT)m_##pre.Name();                                         \
  }

#define TCWotLib_DecPreHandler1(retType, pre, Name, T1) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 1);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    return (LRESULT)m_##pre.Name(p1);   \
  }

#define TCWotLib_DecPreHandler2(retType, pre, Name, T1,T2) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 2);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    return (LRESULT)m_##pre.Name(p1,p2);   \
  }

#define TCWotLib_DecPreHandler3(retType, pre, Name, T1,T2,T3) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 3);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3);   \
  }

#define TCWotLib_DecPreHandler4(retType, pre, Name, T1,T2,T3,T4) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 4);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4);   \
  }

#define TCWotLib_DecPreHandler5(retType, pre, Name, T1,T2,T3,T4,T5) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 5);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5);   \
  }

#define TCWotLib_DecPreHandler6(retType, pre, Name, T1,T2,T3,T4,T5,T6) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 6);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6);   \
  }

#define TCWotLib_DecPreHandler7(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 7);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7);   \
  }

#define TCWotLib_DecPreHandler8(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 8);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8);   \
  }

#define TCWotLib_DecPreHandler9(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 9);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9);   \
  }

#define TCWotLib_DecPreHandler10(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 10);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);   \
  }

#define TCWotLib_DecPreHandler11(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 11);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);   \
  }

#define TCWotLib_DecPreHandler12(retType, pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 12);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    T12 p12 = va_arg(ArgList, T12);                                         \
    return (LRESULT)m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12);   \
  }

/////////////////////////////////////////////////////////////////////////////
// Non-prefixed handlers for void functions

#define TCWotLib_DecPreHandler0_void(pre, Name) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(wParam == 0 && lParam == 0);                                    \
    m_##pre.Name();                                                         \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler1_void(pre, Name, T1) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 1);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    m_##pre.Name(p1);                                                       \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler2_void(pre, Name, T1,T2) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 2);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    m_##pre.Name(p1,p2);                                                    \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler3_void(pre, Name, T1,T2,T3) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 3);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    m_##pre.Name(p1,p2,p3);                                                 \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler4_void(pre, Name, T1,T2,T3,T4) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 4);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    m_##pre.Name(p1,p2,p3,p4);                                              \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler5_void(pre, Name, T1,T2,T3,T4,T5) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 5);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    m_##pre.Name(p1,p2,p3,p4,p5);                                           \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler6_void(pre, Name, T1,T2,T3,T4,T5,T6) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 6);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    m_##pre.Name(p1,p2,p3,p4,p5,p6);                                        \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler7_void(pre, Name, T1,T2,T3,T4,T5,T6,T7) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 7);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7);                                     \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler8_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 8);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8);                                  \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler9_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 9);                                                   \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9);                               \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler10_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 10);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10);                           \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler11_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 11);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11);                       \
    return 0;                                                               \
  }

#define TCWotLib_DecPreHandler12_void(pre, Name, T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12) \
  LRESULT On##pre##Name(UINT, WPARAM wParam, LPARAM lParam, BOOL &)     \
  {                                                                         \
    ZAssert(lParam == 12);                                                  \
    va_list ArgList = (va_list)wParam;                                      \
    T1  p1  = va_arg(ArgList, T1);                                          \
    T2  p2  = va_arg(ArgList, T2);                                          \
    T3  p3  = va_arg(ArgList, T3);                                          \
    T4  p4  = va_arg(ArgList, T4);                                          \
    T5  p5  = va_arg(ArgList, T5);                                          \
    T6  p6  = va_arg(ArgList, T6);                                          \
    T7  p7  = va_arg(ArgList, T7);                                          \
    T8  p8  = va_arg(ArgList, T8);                                          \
    T9  p9  = va_arg(ArgList, T9);                                          \
    T10 p10 = va_arg(ArgList, T10);                                         \
    T11 p11 = va_arg(ArgList, T11);                                         \
    T12 p12 = va_arg(ArgList, T12);                                         \
    m_##pre.Name(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12);                   \
    return 0;                                                               \
  }

/////////////////////////////////////////////////////////////////////////////

#endif // !__WotLib_h__
