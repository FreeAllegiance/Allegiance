#pragma once

/////////////////////////////////////////////////////////////////////////////
// Range.h : Declaration and implementation of the range<T> class template.
//

#include <algorithm>


/////////////////////////////////////////////////////////////////////////////
//
template <class _Tp, class _Compare = std::less<_Tp> >
class range
{
// Types
public:
  typedef _Tp      value_type;
  typedef _Compare value_compare;

// Construction
public:
  range() :
    m_lower(value_type()),
    m_upper(value_type()),
    m_value_comp(value_compare())
  {
  }
  range(const value_compare& comp) :
    m_lower(value_type()),
    m_upper(value_type()),
    m_value_comp(comp)
  {
  }
  range(const value_type& t1, const value_type& t2,
    const value_compare& comp = value_compare()) :
    m_lower(t1),
    m_upper(t2),
    m_value_comp(comp)
  {
    if (value_comp()(m_upper, m_lower))
      std::swap(m_lower, m_upper);
  }
  range(const range<_Tp, _Compare>& that) :
    m_lower(that.lower()),
    m_upper(that.upper()),
    m_value_comp(that.value_comp())
  {
  }

// Attributes
public:
  const value_type& lower() const {return m_lower;}
  const value_type& upper() const {return m_upper;}
  bool empty() const
  {
    return !value_comp()(lower(), upper())
        && !value_comp()(upper(), lower());
  }
  bool intersects(const value_type& t) const
  {
    return !value_comp()(t, lower())
         && value_comp()(t, upper());
  }
  bool intersects(const range<_Tp, _Compare>& that) const
  {
    return value_comp()(lower(), that.upper())
        && value_comp()(that.lower(), upper());
  }

// Observers
public:
  const value_compare& value_comp() const {return m_value_comp;}

// Operators
public:
  range& operator=(const range<_Tp, _Compare>& that)
  {
    m_lower = that.lower();
    m_upper = that.upper();
    return *this;
  }

// Data Members
protected:
  value_type    m_lower;
  value_type    m_upper;
  value_compare m_value_comp;
};


/////////////////////////////////////////////////////////////////////////////
//
template <class _Tp>
inline range<_Tp> make_range(const _Tp& t1, const _Tp& t2)
{
  return range<_Tp>(t1, t2);
}


/////////////////////////////////////////////////////////////////////////////