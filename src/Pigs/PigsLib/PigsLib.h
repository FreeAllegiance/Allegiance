

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Mon Jan 18 21:14:07 2038
 */
/* Compiler settings for PigsLib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __PigsLib_h__
#define __PigsLib_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __PigsInterfaces_FWD_DEFINED__
#define __PigsInterfaces_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigsInterfaces PigsInterfaces;
#else
typedef struct PigsInterfaces PigsInterfaces;
#endif /* __cplusplus */

#endif 	/* __PigsInterfaces_FWD_DEFINED__ */


#ifndef __PigSession_FWD_DEFINED__
#define __PigSession_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigSession PigSession;
#else
typedef struct PigSession PigSession;
#endif /* __cplusplus */

#endif 	/* __PigSession_FWD_DEFINED__ */


#ifndef __PigBehaviorScript_FWD_DEFINED__
#define __PigBehaviorScript_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigBehaviorScript PigBehaviorScript;
#else
typedef struct PigBehaviorScript PigBehaviorScript;
#endif /* __cplusplus */

#endif 	/* __PigBehaviorScript_FWD_DEFINED__ */


#ifndef __PigBehaviorScriptType_FWD_DEFINED__
#define __PigBehaviorScriptType_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigBehaviorScriptType PigBehaviorScriptType;
#else
typedef struct PigBehaviorScriptType PigBehaviorScriptType;
#endif /* __cplusplus */

#endif 	/* __PigBehaviorScriptType_FWD_DEFINED__ */


#ifndef __PigBehaviorHost_FWD_DEFINED__
#define __PigBehaviorHost_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigBehaviorHost PigBehaviorHost;
#else
typedef struct PigBehaviorHost PigBehaviorHost;
#endif /* __cplusplus */

#endif 	/* __PigBehaviorHost_FWD_DEFINED__ */


#ifndef __PigMissionParams_FWD_DEFINED__
#define __PigMissionParams_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigMissionParams PigMissionParams;
#else
typedef struct PigMissionParams PigMissionParams;
#endif /* __cplusplus */

#endif 	/* __PigMissionParams_FWD_DEFINED__ */


#ifndef __Pig_FWD_DEFINED__
#define __Pig_FWD_DEFINED__

#ifdef __cplusplus
typedef class Pig Pig;
#else
typedef struct Pig Pig;
#endif /* __cplusplus */

#endif 	/* __Pig_FWD_DEFINED__ */


#ifndef __Pigs_FWD_DEFINED__
#define __Pigs_FWD_DEFINED__

#ifdef __cplusplus
typedef class Pigs Pigs;
#else
typedef struct Pigs Pigs;
#endif /* __cplusplus */

#endif 	/* __Pigs_FWD_DEFINED__ */


#ifndef __PigTimer_FWD_DEFINED__
#define __PigTimer_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigTimer PigTimer;
#else
typedef struct PigTimer PigTimer;
#endif /* __cplusplus */

#endif 	/* __PigTimer_FWD_DEFINED__ */


#ifndef __PigShip_FWD_DEFINED__
#define __PigShip_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigShip PigShip;
#else
typedef struct PigShip PigShip;
#endif /* __cplusplus */

#endif 	/* __PigShip_FWD_DEFINED__ */


#ifndef __PigShipEvent_FWD_DEFINED__
#define __PigShipEvent_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigShipEvent PigShipEvent;
#else
typedef struct PigShipEvent PigShipEvent;
#endif /* __cplusplus */

#endif 	/* __PigShipEvent_FWD_DEFINED__ */


#ifndef __PigAccount_FWD_DEFINED__
#define __PigAccount_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigAccount PigAccount;
#else
typedef struct PigAccount PigAccount;
#endif /* __cplusplus */

#endif 	/* __PigAccount_FWD_DEFINED__ */


#ifndef __PigAccountDispenser_FWD_DEFINED__
#define __PigAccountDispenser_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigAccountDispenser PigAccountDispenser;
#else
typedef struct PigAccountDispenser PigAccountDispenser;
#endif /* __cplusplus */

#endif 	/* __PigAccountDispenser_FWD_DEFINED__ */


#ifndef __PigHullTypes_FWD_DEFINED__
#define __PigHullTypes_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigHullTypes PigHullTypes;
#else
typedef struct PigHullTypes PigHullTypes;
#endif /* __cplusplus */

#endif 	/* __PigHullTypes_FWD_DEFINED__ */


#ifndef __PigBehaviorStack_FWD_DEFINED__
#define __PigBehaviorStack_FWD_DEFINED__

#ifdef __cplusplus
typedef class PigBehaviorStack PigBehaviorStack;
#else
typedef struct PigBehaviorStack PigBehaviorStack;
#endif /* __cplusplus */

#endif 	/* __PigBehaviorStack_FWD_DEFINED__ */


/* header files for imported files */
#include "PigsIDL.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PigsLib_LIBRARY_DEFINED__
#define __PigsLib_LIBRARY_DEFINED__

/* library PigsLib */
/* [helpstring][version][uuid] */ 






EXTERN_C const IID LIBID_PigsLib;

EXTERN_C const CLSID CLSID_PigsInterfaces;

#ifdef __cplusplus

class DECLSPEC_UUID("29E0A264-CC16-11d2-9646-00C04F68DEB0")
PigsInterfaces;
#endif

EXTERN_C const CLSID CLSID_PigSession;

#ifdef __cplusplus

class DECLSPEC_UUID("9BCD384E-C6F2-11D2-85C9-00C04F68DEB0")
PigSession;
#endif

EXTERN_C const CLSID CLSID_PigBehaviorScript;

#ifdef __cplusplus

class DECLSPEC_UUID("29E0A265-CC16-11d2-9646-00C04F68DEB0")
PigBehaviorScript;
#endif

EXTERN_C const CLSID CLSID_PigBehaviorScriptType;

#ifdef __cplusplus

class DECLSPEC_UUID("75A3CFA1-D306-11d2-9646-00C04F68DEB0")
PigBehaviorScriptType;
#endif

EXTERN_C const CLSID CLSID_PigBehaviorHost;

#ifdef __cplusplus

class DECLSPEC_UUID("8962E24C-CD81-11d2-9646-00C04F68DEB0")
PigBehaviorHost;
#endif

EXTERN_C const CLSID CLSID_PigMissionParams;

#ifdef __cplusplus

class DECLSPEC_UUID("15219787-D562-11d2-9646-00C04F68DEB0")
PigMissionParams;
#endif

EXTERN_C const CLSID CLSID_Pig;

#ifdef __cplusplus

class DECLSPEC_UUID("15219788-D562-11d2-9646-00C04F68DEB0")
Pig;
#endif

EXTERN_C const CLSID CLSID_Pigs;

#ifdef __cplusplus

class DECLSPEC_UUID("DC2A26D8-DDC0-11d2-A507-00C04F68DEB0")
Pigs;
#endif

EXTERN_C const CLSID CLSID_PigTimer;

#ifdef __cplusplus

class DECLSPEC_UUID("75A3CFA0-D306-11d2-9646-00C04F68DEB0")
PigTimer;
#endif

EXTERN_C const CLSID CLSID_PigShip;

#ifdef __cplusplus

class DECLSPEC_UUID("E57D7452-E3AC-11d2-A507-00C04F68DEB0")
PigShip;
#endif

EXTERN_C const CLSID CLSID_PigShipEvent;

#ifdef __cplusplus

class DECLSPEC_UUID("4F16D144-EF2F-11d2-A50D-00C04F68DEB0")
PigShipEvent;
#endif

EXTERN_C const CLSID CLSID_PigAccount;

#ifdef __cplusplus

class DECLSPEC_UUID("9C627DCC-E7DC-11d2-A508-00C04F68DEB0")
PigAccount;
#endif

EXTERN_C const CLSID CLSID_PigAccountDispenser;

#ifdef __cplusplus

class DECLSPEC_UUID("8D6763CA-E7CB-11d2-A508-00C04F68DEB0")
PigAccountDispenser;
#endif

EXTERN_C const CLSID CLSID_PigHullTypes;

#ifdef __cplusplus

class DECLSPEC_UUID("E1A2D508-F8FD-11d2-A50F-00C04F68DEB0")
PigHullTypes;
#endif

EXTERN_C const CLSID CLSID_PigBehaviorStack;

#ifdef __cplusplus

class DECLSPEC_UUID("4E9FF98A-364D-11d3-A51D-00C04F68DEB0")
PigBehaviorStack;
#endif
#endif /* __PigsLib_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_PigsLib_0000_0001 */
/* [local] */ 

/////////////////////////////////////////////////////////////////////////////
// CATID Declarations

extern "C" const __declspec(selectany) CATID CATID_PigBehaviors =
  {0x6BA550EC,0xC6FD,0x11D2,{0x85,0xC9,0x00,0xC0,0x4F,0x68,0xDE,0xB0}};
extern "C" const __declspec(selectany) CATID CATID_PigObjects =
  {0x72F12FF0,0xC717,0x11D2,{0x85,0xC9,0x00,0xC0,0x4F,0x68,0xDE,0xB0}};



extern RPC_IF_HANDLE __MIDL_itf_PigsLib_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PigsLib_0000_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


