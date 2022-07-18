#ifndef __RangeSet_h__
#define __RangeSet_h__

/////////////////////////////////////////////////////////////////////////////
// RangeSet.h : Declaration and implementation of the rangeset class template.
//

#include <functional>
#include <set>
#include "range.h"


/////////////////////////////////////////////////////////////////////////////
//
template <class _Key>
struct rangeset_less
{
  typedef typename _Key::value_compare value_compare;
  bool operator()(const _Key& x, const _Key& y) const
  {
    return value_compare()(x.upper(), y.lower());
  }
};


/////////////////////////////////////////////////////////////////////////////
//
#ifndef __STL_ALLOC_INSTANCE
  #define __STL_ALLOC_INSTANCE(_X) _X()
#endif


/////////////////////////////////////////////////////////////////////////////
//
template <class _Key, class _Alloc = std::allocator<_Key> >
class rangeset
{
// Types
public:
  typedef typename _Key::value_type                   data_type;
  typedef _Key                                        key_type;
  typedef _Key                                        value_type;
  typedef typename key_type::value_compare            data_compare;
  typedef rangeset_less<key_type>                     key_compare;
  typedef rangeset_less<value_type>                   value_compare;
private:
  // The internal representation of the collection
  typedef std::multiset<key_type,key_compare,_Alloc>  XRangeSet;
public:
  // All other standard types are just aliases to the internal collection
  typedef typename XRangeSet::allocator_type          allocator_type;
  typedef typename XRangeSet::reference               reference;
  typedef typename XRangeSet::const_reference         const_reference;
  typedef typename XRangeSet::iterator                iterator;
  typedef typename XRangeSet::const_iterator          const_iterator;
  typedef typename XRangeSet::size_type               size_type;
  typedef typename XRangeSet::difference_type         difference_type;
  typedef typename allocator_type::pointer            pointer;
  typedef typename allocator_type::const_pointer      const_pointer;
  typedef typename XRangeSet::reverse_iterator        reverse_iterator;
  typedef typename XRangeSet::const_reverse_iterator  const_reverse_iterator;

// Construction / Destruction
public:
  rangeset() : m_set(key_compare(), allocator_type()) {}
  explicit rangeset(const key_compare& __comp,
	       const allocator_type& __a = __STL_ALLOC_INSTANCE(allocator_type))
    : m_set(__comp, __a) {}

#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  rangeset(_InputIterator __first, _InputIterator __last)
    : m_set(key_compare(), allocator_type())
    { insert(__first, __last); }

  template <class _InputIterator>
  rangeset(_InputIterator __first, _InputIterator __last, const key_compare& __comp,
      const allocator_type& __a = __STL_ALLOC_INSTANCE(allocator_type))
    : m_set(__comp, __a) { insert(__first, __last); }
#else // __STL_MEMBER_TEMPLATES
  rangeset(const value_type* __first, const value_type* __last) 
    : m_set(key_compare(), allocator_type()) 
    { insert(__first, __last); }

  rangeset(const value_type* __first, 
      const value_type* __last, const key_compare& __comp,
      const allocator_type& __a = __STL_ALLOC_INSTANCE(allocator_type))
    : m_set(__comp, __a) { insert(__first, __last); }

  rangeset(const_iterator __first, const_iterator __last)
    : m_set(key_compare(), allocator_type()) 
    { insert(__first, __last); }

  rangeset(const_iterator __first, const_iterator __last, const key_compare& __comp,
      const allocator_type& __a = __STL_ALLOC_INSTANCE(allocator_type))
    : m_set(__comp, __a) { insert(__first, __last); }
#endif // __STL_MEMBER_TEMPLATES

  rangeset(const rangeset<_Key,_Alloc>& that) : m_set(that.m_set) {}
  rangeset<_Key,_Alloc>& operator=(const rangeset<_Key,_Alloc>& that)
  { 
    insert(that.begin(), that.end()); 
    return *this;
  }

// Iterators
public:
  iterator         begin()  { return m_set.begin();  }
  iterator         end()    { return m_set.end();    }
  reverse_iterator rbegin() { return m_set.rbegin(); } 
  reverse_iterator rend()   { return m_set.rend();   }

  const_iterator         begin()  const { return m_set.begin();  }
  const_iterator         end()    const { return m_set.end();    }
  const_reverse_iterator rbegin() const { return m_set.rbegin(); } 
  const_reverse_iterator rend()   const { return m_set.rend();   }

// Capacity
public:
  bool      empty()    const { return m_set.empty();    }
  size_type size()     const { return m_set.size();     }
  size_type max_size() const { return m_set.max_size(); }

// Modifiers
public:
  iterator insert(data_type t1, data_type t2)
  {
    return insert(make_range(t1, t2));
  }
  iterator insert(const value_type& r)
  {
    // Do nothing if specified range is empty
    if (r.empty())
      return end();

    // Find the range of iterators that intersect/adjacent with r
    std::pair<iterator, iterator> itPair = m_set.equal_range(r);

    // If no intersection, just insert the specified range
    if (itPair.second == itPair.first)
      return m_set.insert(r);

    // Intersected with one or more ranges, replace it/them with the union
    iterator itLast = itPair.second; itLast--;
    data_type tLower(r.value_comp()(itPair.first->lower(), r.lower()) ?
      itPair.first->lower() : r.lower());
    data_type tUpper(r.value_comp()(itLast->upper(), r.upper()) ?
      r.upper() : itLast->upper());
    iterator itInsert(itPair.second);
    m_set.erase(itPair.first, itPair.second);
    return m_set.insert(itInsert, make_range(tLower, tUpper));
  }
#ifdef __STL_MEMBER_TEMPLATES
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last)
  {
    for (_InputIterator it = __first; it != __last; ++it)
      insert(*it);
  }
#else
  void insert(const_iterator __first, const_iterator __last)
  {
    for (const_iterator it = __first; it != __last; ++it)
      insert(*it);
  }
  void insert(const value_type* __first, const value_type* __last)
  {
    for (const value_type* it = __first; it != __last; ++it)
      insert(*it);
  }
#endif /* __STL_MEMBER_TEMPLATES */

  void erase(iterator position)
  {
    return m_set.erase(position);
  }
  size_type erase(data_type t1, data_type t2)
  {
    return erase(make_range(t1, t2));
  }
  size_type erase(const key_type& r)
  {
    // Do nothing if specified range is empty
    if (r.empty())
      return 0;

    // Find the iterators that intersect/adjacent with r
    std::pair<iterator, iterator> itPair(equal_range(r));

    // If no intersection, return false
    if (itPair.second == itPair.first)
      return 0;

    // Intersected with one or more ranges, split it/them
    size_type old_size = size();
    for (iterator it = itPair.first; it != itPair.second;)
    {
      // Copy the current iterator and advance it
      iterator itCurrent = it++;

      if (!r.value_comp()(itCurrent->lower(), r.lower()))
      {
        if (!r.value_comp()(r.upper(), itCurrent->upper()))
        {
          iterator itFirst = itCurrent++;
          while (it != itPair.second &&
            !r.value_comp()(r.upper(), itCurrent->upper()))
          {
            ++itCurrent;
            ++it;
          }
          m_set.erase(itFirst, itCurrent);
        }
        else
        {
          m_set.insert(itCurrent, make_range(r.upper(), itCurrent->upper()));
          m_set.erase(itCurrent);
        }
      }
      else if (!r.value_comp()(r.upper(), itCurrent->upper()))
      {
        m_set.insert(itCurrent, make_range(itCurrent->lower(), r.lower()));
        m_set.erase(itCurrent);
      }
      else
      {
        m_set.insert(make_range(r.upper(), itCurrent->upper()));
        m_set.insert(itCurrent, make_range(itCurrent->lower(), r.lower()));
        m_set.erase(itCurrent);
      }
    }

    // Return the number of elements erased
    return old_size - size();
  }
  void erase(iterator first, iterator last)
  {
    return m_set.erase(first, last);
  }

  void swap(rangeset<_Key, _Alloc>& that) { m_set.swap(that.m_set); }
  void clear()                            { m_set.clear();          }

// Observers
public:
  key_compare    key_comp()      const { return m_set.key_comp();      }
  value_compare  value_comp()    const { return m_set.key_comp();      }
  allocator_type get_allocator() const { return m_set.get_allocator(); }

// Set Operations
public:
  const_iterator find(const key_type& r) const
  {
    // Find the first iterator that intersects with r
    const_iterator it = lower_bound(r);
    return it;
  }
  size_type count(const key_type& r) const
  {
    // Count the number of iterators that intersect with r
    std::pair<const_iterator, const_iterator> itPair(equal_range(r));
    for (int _count = 0; itPair.first != itPair.second; ++itPair.first)
      ++_count;
    return _count;
  }
  const_iterator lower_bound(const data_type& d) const
  {
    // Find the first iterator that intersects/adjacent with d
    const_iterator it = m_set.lower_bound(make_range(d, d));

    // Increment first iterator unless it intersects with r
    if (end() != it && !it->intersects(d))
      ++it;
    if (end() == it)
      return end();
    return it->intersects(d) ? it : end();
  }
  const_iterator lower_bound(const key_type& r) const
  {
    // If specified range is empty, search for simple value type
    // NOTE: treats empty range, r, as findable
    if (r.empty())
      return lower_bound(r.lower());    

    // Find the first iterator that intersects/adjacent with r
    const_iterator it = m_set.lower_bound(r);

    // Increment first iterator unless it intersects with r
    if (end() != it && !it->intersects(r))
      ++it;
    if (end() == it)
      return end();
    return it->intersects(r) ? it : end();
  }
  const_iterator upper_bound(const data_type& d) const
  {
    // NOT TESTED

    // Find the last iterator that intersects/adjacent with d
    const_reverse_iterator it = const_reverse_iterator(m_set.upper_bound(d));

    // Decrement second iterator unless it intersects with d
    if (rbegin() != it && !it->intersects(d))
      ++it;
    if (end() == it)
      return end();
    return it->intersects(d) ? it : end();
  }
  const_iterator upper_bound(const key_type& r) const
  {
    // NOT TESTED

    // If specified range is empty, search for simple value type
    // NOTE: treats empty range, r, as findable
    if (r.empty())
      return upper_bound(r.lower());    

    // Find the last iterator that intersects/adjacent with r
    const_reverse_iterator it = const_reverse_iterator(m_set.upper_bound(r));

    // Decrement second iterator unless it intersects with r
    if (rbegin() != it && !it->intersects(r))
      ++it;
    if (end() == it)
      return end();
    return it->intersects(r) ? it : end();
  }
  std::pair<const_iterator, const_iterator> equal_range(const key_type& r) const
  {
    // NOT TESTED

    // Find the range of iterators that intersect/adjacent with r
    std::pair<const_iterator, const_iterator> itPair = m_set.equal_range(r);

    // Increment the first iterator unless it intersects with r
    if (itPair.first != itPair.second && !itPair.first->intersects(r))
      ++itPair.first;

    // Decrement the last iterator unless it intersects with r
    const_reverse_iterator itLast = (const_reverse_iterator)itPair.second;
    if (itLast != (const_reverse_iterator)itPair.first && !itLast->intersects(r))
      --itPair.second;
    return itPair;
  }
  std::pair<iterator, iterator> equal_range(const key_type& r)
  {
    // Find the range of iterators that intersect/adjacent with r
    std::pair<iterator, iterator> itPair = m_set.equal_range(r);

    // Increment the first iterator unless it intersects with r
    if (itPair.first != itPair.second && !itPair.first->intersects(r))
      ++itPair.first;

    // Decrement the last iterator unless it intersects with r
    reverse_iterator itLast = (reverse_iterator)itPair.second;
    if (itLast != (reverse_iterator)itPair.first && !itLast->intersects(r))
      --itPair.second;
    return itPair;
  }

// Data Members
private:
  XRangeSet    m_set;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__RangeSet_h__
