#pragma once

/////////////////////////////////////////////////////////////////////////////
// CompositeValue.h | Declaration of the TCCompositeValue template class.


/////////////////////////////////////////////////////////////////////////////
// TCCompositeValue provides a mechanism to evaluate a series of values and
// to determine a composite value to represent the series. This process is
// useful in a property page when multiple objects' properties are being
// displayed.
//
// Take the case of a check box that displays the state of a booean property.
// When only one object is being edited by the property page, the check box
// can be set to "checked" if the property is true, or "unchecked" if false.
// However, if the same boolean propery exists in multiple objects, the check
// box should only be set to "checked" if the property is true in *all* of
// the objects. If the property differs in one or more of the objects, the
// check box would be set to "indeterminate". If, when applying changes, the
// check box is still "indeterminate", the boolean property would not be
// applied to any of the objects. But, if the check box had been changed to
// either "checked" or "unchecked", the *same* boolean value would be applied
// to *all* of the objects.
//
// To use this template, declare a local variable in the scope where the
// series is being evaluated:
//
//      int DetermineCheckState(int cBooleans, bool rgBooleans[])
//      {
//        TCCompositeValue<int> bResult(2, 0);
//
// In this example, the int template parameter indicates that the resulting
// value will be an int, suitable as this function's return value, which can
// be passed to the BM_SETCHECK message. The first parameter to the
// constructor is 2, which specifies the resulting value if any value in the
// evaluated series is not equal to the others. The second parameters, 0, is
// the initial resulting value. This can be thought of as the default value
// of the object until, and if, the first value of the series is evaluated.
// This value is *not* compared against when evaluating the series.
//
// Next, you would iterate through the series of values, passing each to the
// Evaluate method:
//
//        for (int i = 0; i < cBooleans; ++i)
//        {
//          if (!bResult.Evaluate(rgBooleans[i]))
//            break;
//        }
//
// This example stops iterating through the values upon finding a value that
// is not equal to the others, indicated by the result of the Evaluate
// method. This is to avoid unnecessary processing, since continuing to
// evaluate the series would not affect the resulting value.
//
// Finally, you would make use of the resulting value:
//
//        return bResult;
//      }
//
// Notice that the object itself is returned. This is made possible by the
// cast operator, which allows the object to be used in place of a type
// /T/, int in this example. Alternately, the GetValue method could be used:
//
//        return bResult.GetValue();
//      }
//
// Note: This template can also be used with other types of values, including
// strings. However, since the class works with values, the values and the
// result must support being passed, copied, and compared by value. The MFC
// CString and the STL std::basic_string both support these semantics.
//
// TODO: Perhaps a second template parameter should be added to supply the
// binary operation to use for comparison, defaulting to std::less. This
// would allow a string comparison, for example, to be case insensitive.
//
// TODO: Perhaps this example function should be added to MMACRtl. If so, it
// should perhaps also coerce the result to the symbolic check box states. If
// the values are the same, the switch statement will probably optimize away:
//
//         switch (bResult)
//         {
//           case false: return BST_UNCHECKED;
//           case true:  return BST_CHECKED;
//           default:    return BST_BST_INDETERMINATE;
//         }
//       }
//
// Parameters:
//   T - The type of the resulting value and each value of the input series.
//
// See Also: TCPropertyPageImpl
template <class T>
class TCCompositeValue
{
// Construction
public:
  TCCompositeValue(const T& tInequalResult = T(), const T& tInitValue = T());

// Attributes
public:
  bool IsEqual() const;
  const T& GetValue() const;

// Operations
public:
  bool Evaluate(const T& tNextValue);
  void Reset(const T& tInitValue = T());

// Operators
public:
  operator const T&() const;

// Data Members
protected:
  bool m_bInit : 1;   // Flag to indicate if any values have been evaluated.
  bool m_bEqual : 1;  // Flag to indicate that all evaluated values are equal.
  T m_tInequalResult; // Resulting value if any evaluated value is unequal.
  T m_tCurrentValue;  // The current value of the object.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// Initializes the object by calling the Reset method.
//
// Internally, the m_bEqual member is set to true, indicating that all values
// evaluated up to this point (none) are equal. The m_bInit member is set to
// false, indicating that Evaluate has not yet been called.
//
// Parameters:
//   tInequalResult - The resulting value if any value in the evaluated
// series is not equal to the others.
//   tInitValue - The default value of the object until the first call to the
// Evaluate method. This value is *never* compared against when evaluating
// the series.
//
// See Also: TCCompositeValue::Evaluate, TCCompositeValue::GetValue,
// TCCompositeValue::IsEqual, TCCompositeValue::Reset
template <class T>
inline TCCompositeValue<T>::TCCompositeValue(const T& tInequalResult,
  const T& tInitValue) : m_tInequalResult(tInequalResult)
{
  Reset(tInitValue);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Tests the equality state of the currently evaluated series.
//
// Tests the equality state of the currently evaluated series.
//
// Return Value: true if all values evaluated since construction (or the most
// recent call to Reset) have been equal. Otherwise false, indicating that at
// least one value in the series is not equal to the rest.
//
// See Also: TCCompositeValue::constructor, TCCompositeValue::Evaluate,
// TCCompositeValue::Reset, TCCompositeValue::m_bEqual
template <class T>
inline bool TCCompositeValue<T>::IsEqual() const
{
  return m_bEqual;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the current value of the object.
//
// Gets the current value of the object.
//
// Return Value: The current value of the object.
//
// See Also: TCCompositeValue::constructor, TCCompositeValue::cast operator,
// TCCompositeValue::m_tCurrentValue
template <class T>
inline const T& TCCompositeValue<T>::GetValue() const
{
  return m_tCurrentValue;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Evaluates the specified value against the current composite
// value, if any.
//
// Evaluates the specified value against the current composite value, if any.
//
// If this is the first time that the method has been called since
// construction (or the most recent call to Reset), the specified value is
// simply stored and the m_bInit flag is set to indicate that at least one
// value has been evaluated.
//
// Parameters:
//   tNextValue - The value to be compared to the current composite value, if
// any.
//
// Return Value: true if no unequal value has been introduced to the
// composite, otherwise false. This can also be determined using the IsEqual
// method.
//
// See Also: TCCompositeValue::constructor, TCCompositeValue::IsEqual,
// TCCompositeValue::Reset, TCCompositeValue::m_bInit
template <class T>
bool TCCompositeValue<T>::Evaluate(const T& tNextValue)
{
  if (m_bEqual)
  {
    if (m_bInit)
    {
      if (m_tCurrentValue != tNextValue)
      {
        m_tCurrentValue = m_tInequalResult;
        m_bEqual = false;
      }
    }
    else
    {
      m_bInit = true;
      m_tCurrentValue = tNextValue;
    }
  }
  return m_bEqual;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Resets the object to an initialized state.
//
// Resets the object to an initialized state. Internally, the m_bEqual member
// is set to true, indicating that all values evaluated up to this point
// (none) are equal. The m_bInit member is set to false, indicating that
// Evaluate has not yet been called.
//
// Parameters:
//   tInitValue - The default value of the object until the first call to the
// Evaluate method. This value is *never* compared against when evaluating
// the series.
//
// See Also: TCCompositeValue::constructor, TCCompositeValue::Evaluate,
// TCCompositeValue::GetValue, TCCompositeValue::IsEqual
template <class T>
inline void TCCompositeValue<T>::Reset(const T& tInitValue)
{
  m_bInit = false;
  m_bEqual = true;
  m_tCurrentValue = tInitValue;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// See Also: TCCompositeValue::GetValue
template <class T>
inline TCCompositeValue<T>::operator const T&() const
{
  return GetValue();
}


/////////////////////////////////////////////////////////////////////////////