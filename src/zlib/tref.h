#ifndef _tref_H_
#define _tref_H_

#include "StackTracer.h"

// BT - 9/17 - Hunting down mystery fedsrv crashes.

//////////////////////////////////////////////////////////////////////////////
//
// Base class for reference counted objects
//
//////////////////////////////////////////////////////////////////////////////

// SUPPORT COM AND LINUX
// todo: Should this define go into a project configuration?
#define _WIN32_DCOM
#ifdef _WIN32_DCOM
	#define DWORD_OR_UINT32T DWORD
#else
	#define DWORD_OR_UINT32T uint32_t
#endif

class IObjectSingle {
private:
	DWORD_OR_UINT32T m_count;

protected:
	typedef IObjectSingle QIType;


	void Internal_Release()
	{
		--m_count;
	}

public:
	IObjectSingle() :
		m_count(0)
	{
	}

	virtual ~IObjectSingle()
	{
	}


	DWORD_OR_UINT32T GetCount() const
	{
		return m_count;
	}

	virtual bool IsValid()
	{
		return true;
	}

    static uint64_t& totalAdded() {
        static uint64_t total;
        return total;
    }

    static uint64_t& totalRemoved() {
        static uint64_t total;
        return total;
    }

#ifdef _DEBUG
	virtual
#endif
		DWORD_OR_UINT32T __stdcall AddRef()
	{
        totalAdded()++;
		return ++m_count;
	}

#ifdef _DEBUG
	virtual
#endif
		DWORD_OR_UINT32T __stdcall Release()
	{
#ifndef __GNUC__
		// BT - 9/17 - Hunting down mystery fedsrv crashes.
		__try
		{
#endif
            totalRemoved()++;
			if (--m_count == 0) {
				delete this;
				return 0;
			}
#ifndef __GNUC__
		}
		__except (StackTracer::ExceptionFilter(GetExceptionInformation()))
		{
			StackTracer::OutputStackTraceToDebugF();
		}
#endif
		return m_count;
	}
};
//////////////////////////////////////////////////////////////////////////////
//
// Object Interface
//
//////////////////////////////////////////////////////////////////////////////

class IObject : virtual public IObjectSingle {
public:
};

//////////////////////////////////////////////////////////////////////////////
//
// Reference to a reference counted object
//
//////////////////////////////////////////////////////////////////////////////

template <class Type>
class TRef {
private:
	Type* m_pt;

public:
	//
	// constructors
	//

	TRef() :
		m_pt(NULL)
	{
	}

	TRef(const Type* pt)
	{
		m_pt = (Type*)pt;
		if (m_pt)
			m_pt->AddRef();
	}

	TRef(const TRef& tref) :
		m_pt(tref.m_pt)
	{
		if (m_pt)
			m_pt->AddRef();
	}

	template< class Other > TRef(const TRef<Other>& oref) :
		m_pt(static_cast<Other*>(oref))
	{
		if (m_pt)
			m_pt->AddRef();
	}


	//
	// assignment
	//

	TRef* Pointer()
	{
		return this;
	}

	Type** operator&()
	{
		if (m_pt) {
			m_pt->Release();
			m_pt = NULL;
		}
		return &m_pt;
	}

	TRef& operator=(const TRef& tref)
	{
		Type* ptOld = m_pt;
		m_pt = tref.m_pt;

		if (m_pt)
			m_pt->AddRef();

		if (ptOld && ptOld != (Type *) 0xdddddddd && ptOld != (Type *) 0xcdcdcdcd && ptOld != (Type *) 0xcccccccc)
			ptOld->Release();

		return *this;
	}
	/*
	TRef& operator=(const Type* ptNew)
	{
	Type* ptOld = m_pt;
	m_pt = (Type*)ptNew;

	if (m_pt)
	m_pt->AddRef();

	if (ptOld)
	ptOld->Release();

	return *this;
	}
	*/
	//
	// destructor
	//

	~TRef(void)
	{
		if (m_pt)
			m_pt->Release();
	}

	//
	// members
	//

	operator Type*(void) const { return m_pt; }
	Type& operator*(void) const { return *m_pt; }
	Type* operator->(void) const { return m_pt; }

	friend bool operator==(const TRef& t1, Type* pt)
	{
		return t1.m_pt == pt;
	}

	friend bool operator!=(const TRef& t1, Type* pt)
	{
		return t1.m_pt != pt;
	}


	// define comparison operators for use in associative containers
	inline friend bool operator<(const TRef& t1, Type* pt) { return t1.m_pt < pt; };
	inline friend bool operator>(const TRef& t1, Type* pt) { return t1.m_pt > pt; };
	inline friend bool operator<=(const TRef& t1, Type* pt) { return t1.m_pt <= pt; };
	inline friend bool operator>=(const TRef& t1, Type* pt) { return t1.m_pt >= pt; };
};

#endif
