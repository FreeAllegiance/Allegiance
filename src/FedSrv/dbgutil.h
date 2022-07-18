/*----------------------------------------------------------------------------
	Dbgutil.H
		Exported header file for Dbgutil module.

	Copyright (C) 1993 Microsoft Corporation
	All rights reserved.

	Authors:
		kennt	Kenn Takara
		GaryBu	Gary S. Burd

	History:
		05/07/93 suryanr Created.
		06/09/93 kennt	 Reorganized.
		07/06/93 kennt	 YAR (Yet Another Reorg), refactoring code
 ----------------------------------------------------------------------------*/

#ifndef _DBGUTIL_H
#define _DBGUTIL_H

//#ifndef _UTILCORE_H
//#include "utilcore.h"
//#endif

#ifdef __cplusplus
extern "C" {
#else
#define NOENTRYEXITTRACE	1	
#endif

#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif


#define DBG_STRING(var, val) \
	static TCHAR var[] = TEXT(val);
#define DBG_STRING_NL(var, val) \
	static TCHAR var[] = TEXT(val "\r\n");

#ifndef DllExport
#define DllExport	__declspec ( dllexport )
#endif

#ifndef EXPORT
#define EXPORT
#endif

#define DBG_API(type)		DllExport type FAR PASCAL EXPORT
#define DBG_APIV(type)		DllExport type FAR CDECL EXPORT

/*---------------------------------------------------------------------------
	IfBUILD macros
 ---------------------------------------------------------------------------*/
#ifdef DEBUG
#define IfDebug(x)	x
#define IfNotDebug(x)
#else
#define IfDebug(x)
#define IfNotDebug(x) x
#endif

#ifdef RELEASE
#define IfRelease(x)	x
#define IfNotRelease(x)
#else
#define IfRelease(x)
#define IfNotRelease(x) x
#endif

/*---------------------------------------------------------------------------
	Assert
 ---------------------------------------------------------------------------*/
	
#define Panic()						Assert0(FDbgFalse(), TEXT("Panic"))
#define Panic0(szFmt)				Assert0(FDbgFalse(), TEXT(szFmt))
#define Panic1(szFmt, p1)			Assert1(FDbgFalse(), TEXT(szFmt), p1)
#define Panic2(szFmt, p1, p2)		Assert2(FDbgFalse(), TEXT(szFmt), p1, p2)
#define Panic3(szFmt, p1, p2, p3)	Assert3(FDbgFalse(), TEXT(szFmt), p1, p2, p3)
#define Panic4(szFmt, p1, p2, p3, p4)	Assert4(FDbgFalse(), TEXT(szFmt), p1, p2, p3, p4)
#define Panic5(szFmt, p1, p2, p3, p4, p5)	Assert5(FDbgFalse(), TEXT(szFmt), p1, p2, p3, p4, p5)
#define SideAssert(f)				Verify(f)
#define AssertSz(f, sz)				Assert0(f, TEXT(sz))

#if !defined(DEBUG) && !defined(DEBUGASSERT)

#define Verify(f)							((void)(f))

#define Assert(f)							((void)0)
#define Assert0(f, szFmt)					((void)0)
#define Assert1(f, szFmt, p1)				((void)0)
#define Assert2(f, szFmt, p1, p2)			((void)0)
#define Assert3(f, szFmt, p1, p2, p3)		((void)0)
#define Assert4(f, szFmt, p1, p2, p3, p4)	((void)0)
#define Assert5(f, szFmt, p1, p2, p3, p4, p5)	((void)0)
#define AssertFL0(szFile, iLine, f, szFmt)	((void)0)

#else   //!defined(DEBUG) && !defined(DEBUGASSERT)

#ifndef THIS_FILE
#define THIS_FILE __FILE__
#endif

#define Verify(f) Assert(f)

DBG_APIV(void)	DbgAssert(LPCTSTR szFileName, int iLine, LPCTSTR szFmt, ...);

// If you use the utilna.lib version of util (No Assert), you must define
// the following function

void DoAssert(LPCSTR szFile, int iLine, LPCSTR szAssert);

#ifdef FDBGFALSE_API
DBG_API(BOOL)	FDbgFalse(void);
#else
#define FDbgFalse()		(0)
#endif

#define Assert(f) \
	do { DBG_STRING(_sz, #f) \
		 DBG_STRING(_szFmt, "%s") \
	if (!(f)) DbgAssert(THIS_FILE, __LINE__,_szFmt,(LPSTR)_sz); } while (FDbgFalse())
#define Assert0(f, szFmt) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz); } while (FDbgFalse())
#define Assert1(f, szFmt, p1) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz, p1); } while (FDbgFalse())
#define Assert2(f, szFmt, p1, p2) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz, p1, p2); } while (FDbgFalse())
#define Assert3(f, szFmt, p1, p2, p3) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz, p1, p2, p3); } while (FDbgFalse())
#define Assert4(f, szFmt, p1, p2, p3, p4) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz, p1, p2, p3, p4); } while (FDbgFalse())
#define Assert5(f, szFmt, p1, p2, p3, p4, p5) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(THIS_FILE, __LINE__, _sz, p1, p2, p3, p4, p5); } while (FDbgFalse())
#define AssertFL0(szFile, iLine, f, szFmt) \
	do { DBG_STRING(_sz, szFmt)\
	if (!(f)) DbgAssert(szFile, iLine, _sz); } while (FDbgFalse())

#endif  //else - !defined(DEBUG) && !defined(DEBUGASSERT)

/*---------------------------------------------------------------------------
	Trace
 ---------------------------------------------------------------------------*/

#if !defined(DEBUG) && !defined(DEBUGTRACE)

#ifdef __cplusplus
inline void CDECL DbgTrace(LPTSTR, ...) {}
#define Trace_ 1 ? (void)0 : ::DbgTrace
#endif

#define ODS(sz)                             /* nothing */

#define Trace0(szFmt)						((void)0)
#define Trace1(szFmt, p1)					((void)0)
#define Trace2(szFmt, p1, p2)				((void)0)
#define Trace3(szFmt, p1, p2, p3)			((void)0)
#define Trace4(szFmt, p1, p2, p3, p4)		((void)0)
#define Trace5(szFmt, p1, p2, p3, p4, p5)	((void)0)

#define TraceN0(szFmt)						((void)0)
#define TraceN1(szFmt, p1)					((void)0)
#define TraceN2(szFmt, p1, p2)				((void)0)
#define TraceN3(szFmt, p1, p2, p3)			((void)0)
#define TraceN4(szFmt, p1, p2, p3, p4)		((void)0)
#define TraceN5(szFmt, p1, p2, p3, p4, p5)	((void)0)

#define DBG_PROC_ENTRY(szFunctionName)      /* nothing */

#else   //!defined(DEBUG) && !defined(DEBUGTRACE)

DBG_API(void)		OutputTrace(LPCTSTR szTrace);

DBG_APIV(void)		DbgTrace(LPTSTR szFormat, ...);
DBG_API(void)		DbgTraceV(LPCTSTR szFormat, char FAR *pvargs);

#define ODS(sz)     OutputDebugString(sz)


// Trace_ can be called if you are too lazy to count your arguments and call
// the appropriate TraceN macro.
#define Trace_ ::DbgTrace
#define Trace0(szFmt) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz); } while (FDbgFalse())
#define Trace1(szFmt, p1) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz, p1); } while (FDbgFalse())
#define Trace2(szFmt, p1, p2) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz, p1, p2); } while (FDbgFalse())
#define Trace3(szFmt, p1, p2, p3) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz, p1, p2, p3); } while (FDbgFalse())
#define Trace4(szFmt, p1, p2, p3, p4) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz, p1, p2, p3, p4); } while (FDbgFalse()) 
#define Trace5(szFmt, p1, p2, p3, p4, p5) \
	 do { DBG_STRING_NL(_sz, szFmt) DbgTrace(_sz, p1, p2, p3, p4, p5); } while (FDbgFalse())

#define TraceN0(szFmt) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz); } while (FDbgFalse())
#define TraceN1(szFmt, p1) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz, p1); } while (FDbgFalse())
#define TraceN2(szFmt, p1, p2) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz, p1, p2); } while (FDbgFalse())
#define TraceN3(szFmt, p1, p2, p3) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz, p1, p2, p3); } while (FDbgFalse())
#define TraceN4(szFmt, p1, p2, p3, p4) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz, p1, p2, p3, p4); } while (FDbgFalse()) 
#define TraceN5(szFmt, p1, p2, p3, p4, p5) \
	 do { DBG_STRING(_sz, szFmt) DbgTrace(_sz, p1, p2, p3, p4, p5); } while (FDbgFalse())

#ifndef NOENTRYEXITTRACE

    //
    // @class This class is used in the DBG_PROC_ENTRY macro below.
    //
    class DbgProcEntryClass
    {
      public:                                                            
        DbgProcEntryClass(LPCSTR szFunctionName)  { m_lpszName = szFunctionName; Trace1("> Enter %s", szFunctionName); }
        ~DbgProcEntryClass()                      { Trace1("< Exit %s", m_lpszName); }
      private:
        LPCSTR m_lpszName;
    };

    //
    // @func Macro that causes the name of a function to get printed to debug output when the function is entered
    //  and when it is exited.  This is done by using C++ contructor and destructor.  Place the call to DBG_PROC_ENTRY
    //  at the to of each function that you wish to see debug traces for its entry and exit.  The uCategory parameter
    //  is used to clasify the module that this function is in.
    //
    #define DBG_PROC_ENTRY(szFunctionName)                    DbgProcEntryClass _DbgProcEntryClassObject(szFunctionName)
#else
#define DBG_PROC_ENTRY(szFunctionName)  /* nothing */	
#endif  //NOENTRYEXITTRACE

#endif  //else - !defined(DEBUG) && !defined(DEBUGTRACE)

/*---------------------------------------------------------------------------
	Debug Options Structure and APIs
 ---------------------------------------------------------------------------*/

#ifdef DEBUG
typedef struct dbiTag
{
	// Trace enable
	BOOL	fDoTrace;

	// Startup with NO tls trace buffering
	int		itlsTraceRGB;

	// No special trace function (defaults to OutputDebugString if NULL)
	void (CALLBACK * pfnTrace)(LPCTSTR);

	// UI Assert enable (determines whether DoAssert will be called)
	// Asserts will always be output as Trace (as per trace settings)
	BOOL	fShowAsserts;

	// Number of asserts since startup
	ULONG	cAssert;

	// Trigger ZAssert on allocation failure
	BOOL	fAssertOnAllocFail;

	// This setting determines whether an ZAssert calls DbgStop,
	// which triggers an INT 3. If no debugger is running, the INT 3
	// is handled internally, and execution continues.
	BOOL	fStopOnAssert;
	
} DebugInfo;

typedef struct
{
	DWORD	cchBuf;
	DWORD	ichStart;
	DWORD	ichEnd;
	char	rgch[1];
} DbgTraceBuf;

extern DebugInfo g_dbi;
#endif

/*---------------------------------------------------------------------------
	Misc
 ---------------------------------------------------------------------------*/

#ifdef DEBUG

DBG_API(void)	DbgStop(void);
DBG_API(BOOL)	FDbgFail(void);
DBG_API(long)	DbgSetFail(long iFail);
DBG_API(long)	DbgGetFail();

// Call this function first with (fSecondChunk = FALSE), then with
// (fSecondChunk = TRUE) to get the full trace buffer output
// If TLS trace buffering is not enabled, these will return error strings.
#ifdef __cplusplus
#define ITLSTRACE	itlsTrace = (DWORD)-1
#endif

DBG_API(DWORD)	        DbgSetThreadTraceBuffer(DWORD ITLSTRACE);
DBG_API(const char *)	DbgGetTraceBuffer(BOOL fSecondChunk);
DBG_API(BOOL)	        DbgSetTraceBuffer(void *rgbTrace, int cbTrace);

#endif  //DEBUG

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _DBGUTIL_H
