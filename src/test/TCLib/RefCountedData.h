#pragma once

/////////////////////////////////////////////////////////////////////////////
// RefCountedData.h | Declaration of the TCRefCountedData template class.


/////////////////////////////////////////////////////////////////////////////
// TCRefCountedData provides a base class useful in the implementation of
// classes that maintain reference-counted data items. Such classes, for
// example, do not copy data items when the object is copied. Instead, the
// data item /pointer/ is copied and a reference count, stored alongside the
// data item, is incremented. When either copy of the object is assigned a
// new value, the reference count if first decremented.  When the reference
// count of the data item reaches zero, it is finally deleted.
//
// This template is very lightweight in that it really just adds a reference
// counting implementation to whatever class or structure is specified as the
// /T/ template parameter. It does this by deriving from class /T/.
//
// Parameters:
//   T - The base data type which contains the data members of the reference
//   counted data.
template <class T>
class TCRefCountedData : public T
{
// Construction / Destruction
public:
  TCRefCountedData();
  virtual ~TCRefCountedData();

// Operations
public:
  long AddRef();
  long Release();

// Data Members
protected:
  long m_nRef;  // The current reference count of the data.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Constructs the object by initializing the reference count to 1.
template <class T>
inline TCRefCountedData<T>::TCRefCountedData()
  : m_nRef(1)
{
}

/////////////////////////////////////////////////////////////////////////////
// Does nothing.
template <class T>
inline TCRefCountedData<T>::~TCRefCountedData()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Increments the reference count.
//
// Return Value: The new, incremented reference count.
//
// See Also: TCRefCountedData::Release
template <class T>
inline long TCRefCountedData<T>::AddRef()
{
  return this ? ++m_nRef : 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Decrements the reference count.
//
// Return Value: The new, decremented reference count.
//
// When the reference count reaches zero, this method destroys itself.
//
// See Also: TCRefCountedData::AddRef
template <class T>
inline long TCRefCountedData<T>::Release()
{
  long nRef = 0;
  if (this)
  {
    nRef = --m_nRef;
    if (!nRef)
      delete this;
  }
  return nRef;
}


/////////////////////////////////////////////////////////////////////////////