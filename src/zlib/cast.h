//////////////////////////////////////////////////////////////////////////////
//
// cast macros
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "tref.h"

#if !defined(_DEBUG)
    template <class DestType, class SourceType>
    void CastTo(DestType*& pdest, SourceType* psource)
    {
        pdest = (DestType*)(psource);
    }

    template <class DestType, class SourceType>
    void CastTo(TRef<DestType>& pdest, SourceType* psource)
    {
        pdest = (DestType*)(psource);
    }

    template <class DestType, class SourceType>
    void CastTo(DestType*& pdest, const TRef<SourceType>& psource)
    {
        pdest = (DestType*)((SourceType*)psource);
    }

    template <class DestType, class SourceType>
    void CastTo(TRef<DestType>& pdest, const TRef<SourceType>& psource)
    {
        pdest = (DestType*)((SourceType*)psource);
    }
#else
    template <class DestType, class SourceType>
    void CastTo(DestType*& pdest, SourceType* psource)
    {
        if (psource != NULL) {
            pdest = dynamic_cast<DestType*>(psource);
//            ZAssert(pdest != NULL);					// mdvalley: debugging sound, not panes!
        }
        pdest = (DestType*)(psource);
    }

    template <class DestType, class SourceType>
    void CastTo(TRef<DestType>& pdest, SourceType* psource)
    {
        if (psource != NULL) {
            pdest = dynamic_cast<DestType*>(psource);
//            ZAssert(pdest != NULL);
        }
        pdest = (DestType*)(psource);
    }

    template <class DestType, class SourceType>
    void CastTo(DestType*& pdest, const TRef<SourceType>& psource)
    {
        if (psource != NULL) {
            pdest = dynamic_cast<DestType*>((SourceType*)psource);
//            ZAssert(pdest != NULL);
        }
        pdest = (DestType*)((SourceType*)psource);
    }

    template <class DestType, class SourceType>
    void CastTo(TRef<DestType>& pdest, const TRef<SourceType>& psource)
    {
        if (psource != NULL) {
            pdest = dynamic_cast<DestType*>((SourceType*)psource);
//            ZAssert(pdest != NULL);
        }
        pdest = (DestType*)((SourceType*)psource);
    }
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Runtime Casting macros
//
//////////////////////////////////////////////////////////////////////////////

#if 0
template <class DestType, class SourceType>
void RuntimeCastTo(DestType*& pdest, SourceType* psource)
{
    pdest = dynamic_cast<DestType*>(psource);
}

template <class DestType, class SourceType>
void RuntimeCastTo(TRef<DestType>& pdest, SourceType* psource)
{
    pdest = dynamic_cast<DestType*>(psource);
}

template <class DestType, class SourceType>
void RuntimeCastTo(DestType*& pdest, TRef<SourceType>& psource)
{
    pdest = dynamic_cast<DestType*>((SourceType*)psource);
}

template <class DestType, class SourceType>
void RuntimeCastTo(TRef<DestType>& pdest, TRef<SourceType>& psource)
{
    pdest = dynamic_cast<DestType*>((SourceType*)psource);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Cast to a class pointer macro
//
//////////////////////////////////////////////////////////////////////////////

#define DefineCastMember(Type)            \
    template<class TypeSource>            \
    static Type* Cast(TypeSource* pvalue) \
    {                                     \
        Type* p; CastTo(p, pvalue);       \
        return p;                         \
    }