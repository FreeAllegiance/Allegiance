#pragma once

#include <tlist.h>
#include <tref.h>
#include <tstack.h>
#include <zstring.h>

template <typename T>
class TStaticValue;

//////////////////////////////////////////////////////////////////////////////
//
// Stack
//
//////////////////////////////////////////////////////////////////////////////

typedef TStack<TRef<IObject> >      ObjectStack;

//////////////////////////////////////////////////////////////////////////////
//
// Functions
//
//////////////////////////////////////////////////////////////////////////////

class IBinaryReaderSite : public IObject {
public:
    virtual DWORD    GetDWORD()             = 0;
    virtual ZString  GetString()            = 0;
    virtual BYTE*    GetPointer()           = 0;
    virtual void     MovePointer(int count) = 0;
    virtual IObject* GetMemoryObject()      = 0;
    virtual IObject* GetObject()            = 0;
	virtual ZString	 GetCurrentFile()		= 0;

    template<class Type>
    void GetStructure(Type*& ptype)
    {
        ptype = (Type*)GetPointer();
        MovePointer(sizeof(Type));
    }

    template<class Type>
    void CopyStructure(Type* ptype)
    {
        ZAssert(ptype);
        memcpy(ptype, (Type*)GetPointer(), sizeof(Type));
        MovePointer(sizeof(Type));
    }
};

class IFunction : public IObject {
public:
    virtual TRef<IObject> Apply(ObjectStack& stack) = 0;
    virtual TRef<IObject> Read(IBinaryReaderSite* psite, ObjectStack& stack);
};

//////////////////////////////////////////////////////////////////////////////
//
// MDL types
//
//////////////////////////////////////////////////////////////////////////////

class MDLType : public IObject {
public:
    virtual int     GetSize()                                                        = 0;
    virtual void    Destruct(BYTE*& pbyte)                                           = 0;
    virtual void    StackRead(BYTE*& pbyte, ObjectStack& stack)                      = 0;
    virtual void    Read(BYTE*& pbyte, IBinaryReaderSite* psite, ObjectStack& stack) = 0;
    virtual ZString GetCPPType()                                                     = 0;
    virtual ZString GetHungarianPrefix()                                             = 0;

    virtual bool IsStruct()
    {
        return false;
    }                

    /*
    virtual void Write(void*& pv, IMDLBinaryFile* pmdlFile)
    {
        ZUnimplemented();
        return NULL;
    }
    */

    bool IsTypeOf(MDLType* ptype)
    {
        return ptype == this;
    }
};

typedef TList<TRef<MDLType> > MDLTypeList;

TRef<MDLType> CreateStringMDLType();
TRef<MDLType> CreateIObjectMDLType(const ZString& strType, const ZString& strPrefix);
ZString GetStructHeader(MDLType* ptype, const ZString& strIdentifier);

//////////////////////////////////////////////////////////////////////////////
//
// BaseMDLType
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class TBaseMDLType : public MDLType {
private:
    ZString m_str;
    ZString m_strPrefix;

public:
    TBaseMDLType(const ZString& str, const ZString& strPrefix) :
        m_str(str),
        m_strPrefix(strPrefix)
    {
    }

    int GetSize()
    {
        return sizeof(Type);
    }

    virtual bool IsStruct()
    {
        return false;
    }

    void Destruct(BYTE*& pbyte)
    {
        pbyte += sizeof(Type);
    }

    void StackRead(BYTE*& pv, ObjectStack& stack)
    {
        TRef<TStaticValue<Type> > pvalue; CastTo(pvalue, (IObject*)stack.Pop());

        *(Type*)pv = pvalue->GetValue();
        pv += sizeof(Type);
    }

    void Read(BYTE*& pv, IBinaryReaderSite* psite, ObjectStack& stack)
    {
        psite->CopyStructure((Type*)pv);
        pv += sizeof(Type);
    }

    ZString GetCPPType()
    {
        return m_str;
    }

    ZString GetHungarianPrefix()
    {
        return m_strPrefix;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// MDL Objects
//
//////////////////////////////////////////////////////////////////////////////

class IMDLObject : public IObject {
private:
    TRef<MDLType> m_ptype;
	int           m_index;

public:
	IMDLObject(MDLType* ptype) :
        m_ptype(ptype),
		m_index(-1)
	{
	}

    ~IMDLObject()
    {
        BYTE* pbyte = GetDataPointer();
        m_ptype->Destruct(pbyte);
    }

    #ifdef _DebugNewDefined_
        #undef new

        void *operator new(size_t alloc)
        {
            return ::new(__FILE__, __LINE__) BYTE[alloc];
        }

        void* operator new(size_t alloc, int size)
        {
            BYTE* pbyte = (BYTE*)::new(__FILE__, __LINE__) BYTE[alloc + size];
            memset(pbyte + alloc, 0, size);
            return pbyte;
        }

        static IMDLObject* Allocate(int size, MDLType* ptype)
        {
            return new(size) IMDLObject(ptype);
        }

        #define new new(__FILE__, __LINE__)
    #else
        void *operator new(size_t alloc)
        {
            return ::new BYTE[alloc];
        }

        void* operator new(size_t alloc, int size)
        {
            BYTE* pbyte = (BYTE*)::new BYTE[alloc + size];
            memset(pbyte + alloc, 0, size);
            return pbyte;
        }

#ifdef VC6_BUILD
		// MJP - Added this to fix compiler warnings.
		void operator delete( void * p )
		{
			delete p;			// Does this actually work - ie. does it call the underlying delete?
		}

		// MJP - Added this to fix compiler warnings.
		void operator delete(void * p, int size )
		{
			delete p;
		}
#endif // VC6_BUILD

        static IMDLObject* Allocate(int size, MDLType* ptype)
        {
            return new(size) IMDLObject(ptype);
        }
    #endif

    BYTE* GetDataPointer()
    {
        return (BYTE*)(this + 1);
    }

	void SetIndex(int index) 
	{
		m_index = index;
	}

	int GetIndex()
	{
		return m_index;
	}

    MDLType* GetType()
    {
        return m_ptype;
    }

    /* !!! write is a Value method not a IObject method
    void Write(IMDLBinaryFile* pmdlFile)
    {
        GetType()->Write(GetDataPointer(), pmdlFile);
    }
    */
};

//////////////////////////////////////////////////////////////////////////////
//
// Object Pair
//
//////////////////////////////////////////////////////////////////////////////

class IObjectPair : public IObject {
    TRef<IObject> m_pfirst;
    TRef<IObject> m_psecond;

public:
    IObjectPair(IObject* pfirst, IObject* psecond) :
        m_pfirst(pfirst),
        m_psecond(psecond)
    {
    }

    IObject* GetFirst()  { return m_pfirst; }
    IObject* GetSecond() { return m_psecond; }
    IObject* GetNth(int index);
    IObject* GetLastNth(int index);
};