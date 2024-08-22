#pragma once

/////////////////////////////////////////////////////////////////////////////
// RangeValueSlider.h | Declaration of the TCRangeValueSlider class.

#include <CodecDriver.h>
#include "SliderWindow.h"


/////////////////////////////////////////////////////////////////////////////
// TCRangeValueSlider

class TCRangeValueSlider : public TCSliderWindow
{
// Attributes
public:
  bool IsAtDefaultPos() const;
  void SetToDefaultPos();

// Operations
public:
  bool Update(ULONG nObjects, ITCRangeValue** ppObjects);
  HRESULT Apply(ULONG nObjects, ITCRangeValue** ppObjects);

// Operators
public:
  const TCRangeValueSlider& operator=(HWND hwnd);

// Data Members
protected:
  long m_nValueDefault;
};


/////////////////////////////////////////////////////////////////////////////
// Inline Attributes

inline bool TCRangeValueSlider::IsAtDefaultPos() const
{
  return GetPos() == m_nValueDefault;
}

inline void TCRangeValueSlider::SetToDefaultPos()
{
  SetPos(m_nValueDefault);
}


/////////////////////////////////////////////////////////////////////////////
// Inline Operators

inline const TCRangeValueSlider& TCRangeValueSlider::operator=(HWND hwnd)
{
  SubclassWindow(hwnd);
  return *this;
}


/////////////////////////////////////////////////////////////////////////////