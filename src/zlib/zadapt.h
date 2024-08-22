#pragma once

/////////////////////////////////////////////////////////////////////////////
// An adapter to allow classes like TRefs to be stored in STL containers 
// (the operator & was breaking things).  This was shamelessly stolen from
// the ATL class CAdapt and modified for our purposes.
//
template <class T>
class ZAdapt
{
public:
    ZAdapt()
    {
    }

    ZAdapt(const T& rSrc) :
        m_T(rSrc)
    {
    }

    ZAdapt(const ZAdapt& rSrCA) :
        m_T(rSrCA.m_T)
    {
    }

    ZAdapt& operator=(const T& rSrc)
    {
        m_T = rSrc;
        return *this;
    }

    bool operator<(const T& rSrc) const
    {
        return m_T < rSrc;
    }

    bool operator==(const T& rSrc) const
    {
        return m_T == rSrc;
    }

    operator T&()
    {
        return m_T;
    }

    operator const T&() const
    {
        return m_T;
    }

    T& operator->()
    {
        return m_T;
    }

    const T& operator->() const
    {
        return m_T;
    }

    T m_T;
};


/////////////////////////////////////////////////////////////////////////////