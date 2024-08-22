#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigMacro.h : Declaration of macros used in the ActivePigs project.
//

#include <..\TCLib\InvokeArgs.h>


/////////////////////////////////////////////////////////////////////////////
// Macros to simplify the implementation of the scripted behavior methods

#define CPigBehaviorScript_MethodImpl_0(methodName)                         \
  STDMETHODIMP methodName()                                                 \
  {                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);  \
    if (DISPID_UNKNOWN == dispid)                                           \
      return (NULL != m_spBehaviorBase) ?                                   \
        m_spBehaviorBase->methodName() : S_FALSE;                           \
    return TCInvokeArgs(dispid, 0).Invoke(m_spdScript);                     \
  }

#define CPigBehaviorScript_MethodImpl_1(methodName, vt1, t1)                \
  STDMETHODIMP methodName(t1 p1)                                            \
  {                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);  \
    if (DISPID_UNKNOWN == dispid)                                           \
      return (NULL != m_spBehaviorBase) ?                                   \
        m_spBehaviorBase->methodName(p1) : S_FALSE;                         \
    return TCInvokeArgs(dispid, 1, vt1, p1).Invoke(m_spdScript);            \
  }

#define CPigBehaviorScript_MethodImpl_2(methodName, vt1, t1, vt2, t2)       \
  STDMETHODIMP methodName(t1 p1, t2 p2)                                     \
  {                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);  \
    if (DISPID_UNKNOWN == dispid)                                           \
      return (NULL != m_spBehaviorBase) ?                                   \
        m_spBehaviorBase->methodName(p1, p2) : S_FALSE;                     \
    return TCInvokeArgs(dispid, 2, vt1, p1, vt2, p2).Invoke(m_spdScript);   \
  }

#define CPigBehaviorScript_MethodImpl_3(methodName, vt1, t1, vt2, t2, vt3, t3)     \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3)                                     \
  {                                                                                \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);         \
    if (DISPID_UNKNOWN == dispid)                                                  \
      return (NULL != m_spBehaviorBase) ?                                          \
        m_spBehaviorBase->methodName(p1, p2, p3) : S_FALSE;                        \
    return TCInvokeArgs(dispid, 3, vt1, p1, vt2, p2, vt3, p3).Invoke(m_spdScript); \
  }

#define CPigBehaviorScript_MethodImpl_4(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4)     \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4)                                       \
  {                                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                  \
    if (DISPID_UNKNOWN == dispid)                                                           \
      return (NULL != m_spBehaviorBase) ?                                                   \
        m_spBehaviorBase->methodName(p1, p2, p3, p4) : S_FALSE;                             \
    return TCInvokeArgs(dispid, 4, vt1, p1, vt2, p2, vt3, p3, vt4, p4).Invoke(m_spdScript); \
  }

#define CPigBehaviorScript_MethodImpl_5(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4, vt5, t5)     \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5)                                         \
  {                                                                                                  \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                           \
    if (DISPID_UNKNOWN == dispid)                                                                    \
      return (NULL != m_spBehaviorBase) ?                                                            \
        m_spBehaviorBase->methodName(p1, p2, p3, p4, p5) : S_FALSE;                                  \
    return TCInvokeArgs(dispid, 5, vt1, p1, vt2, p2, vt3, p3, vt4, p4, vt5, p5).Invoke(m_spdScript); \
  }

#define CPigBehaviorScript_MethodImpl_6(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4, vt5, t5, vt6, t6)     \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6)                                           \
  {                                                                                                           \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                                    \
    if (DISPID_UNKNOWN == dispid)                                                                             \
      return (NULL != m_spBehaviorBase) ?                                                                     \
        m_spBehaviorBase->methodName(p1, p2, p3, p4, p5, p6) : S_FALSE;                                       \
    return TCInvokeArgs(dispid, 6, vt1, p1, vt2, p2, vt3, p3, vt4, p4, vt5, p5, vt6, p6).Invoke(m_spdScript); \
  }


#define CPigBehaviorScript_MethodImpl_0R(methodName, vtR, tR)               \
  STDMETHODIMP methodName(tR* pR)                                           \
  {                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);  \
    if (DISPID_UNKNOWN == dispid)                                           \
      return (NULL != m_spBehaviorBase) ?                                   \
        m_spBehaviorBase->methodName(, pR) : S_FALSE;                       \
    CComVariant var;                                                        \
    RETURN_FAILED(TCInvokeArgs(dispid, 0).Invoke(m_spdScript, &var));       \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                               \
    {                                                                       \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                          \
      VARIANT varTemp;                                                      \
      var.Detach(&varTemp);                                                 \
    }                                                                       \
    return S_OK;                                                            \
  }

#define CPigBehaviorScript_MethodImpl_1R(methodName, vt1, t1, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, tR* pR)                                       \
  {                                                                            \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);     \
    if (DISPID_UNKNOWN == dispid)                                              \
      return (NULL != m_spBehaviorBase) ?                                      \
        m_spBehaviorBase->methodName(p1, pR) : S_FALSE;                        \
    CComVariant var;                                                           \
    RETURN_FAILED(TCInvokeArgs(dispid, 1, vt1, p1).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                  \
    {                                                                          \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                             \
      VARIANT varTemp;                                                         \
      var.Detach(&varTemp);                                                    \
    }                                                                          \
    return S_OK;                                                               \
  }

#define CPigBehaviorScript_MethodImpl_2R(methodName, vt1, t1, vt2, t2, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, t2 p2, tR* pR)                                         \
  {                                                                                     \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);              \
    if (DISPID_UNKNOWN == dispid)                                                       \
      return (NULL != m_spBehaviorBase) ?                                               \
        m_spBehaviorBase->methodName(p1, p2, pR) : S_FALSE;                             \
    CComVariant var;                                                                    \
    RETURN_FAILED(TCInvokeArgs(dispid, 2, vt1, p1, vt2, p2).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                           \
    {                                                                                   \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                                      \
      VARIANT varTemp;                                                                  \
      var.Detach(&varTemp);                                                             \
    }                                                                                   \
    return S_OK;                                                                        \
  }

#define CPigBehaviorScript_MethodImpl_3R(methodName, vt1, t1, vt2, t2, vt3, t3, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, tR* pR)                                           \
  {                                                                                              \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                       \
    if (DISPID_UNKNOWN == dispid)                                                                \
      return (NULL != m_spBehaviorBase) ?                                                        \
        m_spBehaviorBase->methodName(p1, p2, p3, pR) : S_FALSE;                                  \
    CComVariant var;                                                                             \
    RETURN_FAILED(TCInvokeArgs(dispid, 3, vt1, p1, vt2, p2, vt3, p3).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                                    \
    {                                                                                            \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                                               \
      VARIANT varTemp;                                                                           \
      var.Detach(&varTemp);                                                                      \
    }                                                                                            \
    return S_OK;                                                                                 \
  }

#define CPigBehaviorScript_MethodImpl_4R(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4, tR* pR)                                             \
  {                                                                                                       \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                                \
    if (DISPID_UNKNOWN == dispid)                                                                         \
      return (NULL != m_spBehaviorBase) ?                                                                 \
        m_spBehaviorBase->methodName(p1, p2, p3, p4, pR) : S_FALSE;                                       \
    CComVariant var;                                                                                      \
    RETURN_FAILED(TCInvokeArgs(dispid, 4, vt1, p1, vt2, p2, vt3, p3, vt4, p4).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                                             \
    {                                                                                                     \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                                                        \
      VARIANT varTemp;                                                                                    \
      var.Detach(&varTemp);                                                                               \
    }                                                                                                     \
    return S_OK;                                                                                          \
  }

#define CPigBehaviorScript_MethodImpl_5R(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4, vt5, t5, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, tR* pR)                                               \
  {                                                                                                                \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                                         \
    if (DISPID_UNKNOWN == dispid)                                                                                  \
      return (NULL != m_spBehaviorBase) ?                                                                          \
        m_spBehaviorBase->methodName(p1, p2, p3, p4, p5, pR) : S_FALSE;                                            \
    CComVariant var;                                                                                               \
    RETURN_FAILED(TCInvokeArgs(dispid, 5, vt1, p1, vt2, p2, vt3, p3, vt4, p4, vt5, p5).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                                                      \
    {                                                                                                              \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                                                                 \
      VARIANT varTemp;                                                                                             \
      var.Detach(&varTemp);                                                                                        \
    }                                                                                                              \
    return S_OK;                                                                                                   \
  }

#define CPigBehaviorScript_MethodImpl_6R(methodName, vt1, t1, vt2, t2, vt3, t3, vt4, t4, vt5, t5, vt6, t6, vtR, tR)         \
  STDMETHODIMP methodName(t1 p1, t2 p2, t3 p3, t4 p4, t5 p5, t6 p6, tR* pR)                                                 \
  {                                                                                                                         \
    DISPID dispid = m_Methods.GetMethodID(m_Methods.EMethod_##methodName);                                                  \
    if (DISPID_UNKNOWN == dispid)                                                                                           \
      return (NULL != m_spBehaviorBase) ?                                                                                   \
        m_spBehaviorBase->methodName(p1, p2, p3, p4, p5, p6, pR) : S_FALSE;                                                 \
    CComVariant var;                                                                                                        \
    RETURN_FAILED(TCInvokeArgs(dispid, 6, vt1, p1, vt2, p2, vt3, p3, vt4, p4, vt5, p5, vt6, p6).Invoke(m_spdScript, &var)); \
    if (pR && SUCCEEDED(var.ChangeType(vtR)))                                                                               \
    {                                                                                                                       \
      *pR = *(reinterpret_cast<tR*>(&V_I4(&var)));                                                                          \
      VARIANT varTemp;                                                                                                      \
      var.Detach();                                                                                                         \
    }                                                                                                                       \
    return S_OK;                                                                                                            \
  }


/////////////////////////////////////////////////////////////////////////////