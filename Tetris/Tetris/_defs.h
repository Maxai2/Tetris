/*  _defs.h

    Common definitions for pointer size and calling conventions.

    Calling conventions:
    _RTLENTRY       Specifies the calling convention used by the RTL

    _USERENTRY      Specifies the calling convention the RTL expects user
                    compiled functions to use (for callbacks)

    Export (and size for DOS) information:
    _EXPCLASS       Exports class if building DLL version of library
                    For DOS16 also provides size information

    _EXPDATA        Exports data if building DLL version of library

    _EXPFUNC        Exports function if building DLL version of library
                    For DOS16 also provides size information

    _FAR            Promotes data pointers to far in DLLs (DOS16 only)

    Obsolete versions:
    _Cdecl          Use _RTLENTRY
    _CLASSTYPE      Use _EXPCLASS
    _FARFUNC        Use _EXPFUNC
    _FARCALL        Use _EXPFUNC and declare function explicity __far
*/

/*
 *      C/C++ Run Time Library - Version 17.0
 *
 *      Copyright (c) 1991, 2011 by Embarcadero Technologies, Inc.
 *      All Rights Reserved.
 *
 */


/* $Revision: 31338 $ */

#ifndef ___DEFS_H
#define ___DEFS_H

#if defined(_WIN64)
#define _RTLENTRY
#define _USERENTRY
#else
#define _RTLENTRY  __cdecl
#define _USERENTRY __cdecl
#endif

#if defined(__PAS__)
#    define _RTLENTRYF  __pascal
#    define _USERENTRYF __pascal
#else
#    define _RTLENTRYF  _RTLENTRY
#    define _USERENTRYF _USERENTRY
#endif

#define _FAR
#if defined(_BUILDRTLDLL)
#    define _EXPCLASS  __export
#    define _EXPDATA   __export
#    define _EXPFUNC   __export
#elif defined(_RTLDLL) && !defined(__clang__) /* workaround for Audits */ 
#    define _EXPCLASS  __import
#    define _EXPDATA   __import
#    define _EXPFUNC
#else
#    define _EXPCLASS
#    define _EXPDATA
#    define _EXPFUNC
#endif


#define _EXPFUNC32  _EXPFUNC
#define _EXPFUNC16


#define _Cdecl      _RTLENTRY
#define _CLASSTYPE  _EXPCLASS
#define _FARFUNC    _EXPFUNC
#define _FARCALL    _EXPFUNC __far
#if !defined(__CODEGEARC__)
#define _CType      _RTLENTRYF
#endif

#if !defined(_NO_RETURN_DECL)
  #define _NO_RETURN_DECL	__declspec(noreturn)
#endif

#if defined(__cplusplus)
#   define _PTRDEF(name) typedef name _FAR * P##name;
#   define _REFDEF(name) typedef name _FAR & R##name;
#   define _REFPTRDEF(name) typedef name _FAR * _FAR & RP##name;
#   define _PTRCONSTDEF(name) typedef const name _FAR * PC##name;
#   define _REFCONSTDEF(name) typedef const name _FAR & RC##name;

#if 0
/*********** Obsolete definitions for OWL 1.0 *************/
#   define _CLASSDEF(name) class _EXPCLASS name; \
           _PTRDEF(name) \
           _REFDEF(name) \
           _REFPTRDEF(name) \
           _PTRCONSTDEF(name) \
           _REFCONSTDEF(name)
#endif /* 0 */
#endif

#endif  /* ___DEFS_H */
