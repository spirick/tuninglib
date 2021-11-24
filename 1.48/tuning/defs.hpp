
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2021 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.48
// File tuning/defs.hpp

#ifndef TUNING_DEFS_HPP
#define TUNING_DEFS_HPP

#define TL_VERSION 148

#include <stddef.h> // for size_t

//---------------------------------------------------------------------------
// Compiler and target dependence

// TL_DEBUG:     Compiling with debug informations
// TL_GNU:       GNU compiler
// TL_MICROSOFT: Microsoft compiler
// TL_WINDOWS:   Microsoft Windows target
// TL_UNIX:      UNIX target
// TL_LINUX:     LINUX target
// TL_MULTI:     Multitheaded target
// TL_64         64 Bit target
// TL_EXCEPTION: Exception handling available

// DLL export macros
// TL_EXPORT:    Export macro before exported name
// TL_TM_EXPORT: Export macro for templates

//---------------------------------------------------------------------------

#if defined __GNUC__

  #define TL_GNU
  #define TL_TYPENAME typename
  #define TL_TMU_TYPENAME typename

  #define TL_FMT_INT "t"
  #define TL_FMT_UINT "z"
  #define TL_FMT_INT64 "ll"
  #define TL_FMT_UINT64 "ll"

  #if defined __EXCEPTIONS
    #define TL_EXCEPTION
  #endif

  #if (__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
    #define TL_CAN_THROW_ noexcept (false)
  #else
    #define TL_CAN_THROW_
  #endif

  #if (__GNUC__ >= 7)
    #undef TL_TMU_TYPENAME
    #define TL_TMU_TYPENAME
  #endif

  #if defined (__LP64__) || defined (_LP64)
    #define TL_64
  #endif

  __extension__ typedef long long          t_Int64;
  __extension__ typedef unsigned long long t_UInt64;

  #if (__SIZEOF_WCHAR_T__ == 4) || (__WCHAR_MAX__ > 0x10000)
    #define TL_WCHAR_SIZE 4
    #define TL_WCHAR_SHIFT 2
  #else
    #define TL_WCHAR_SIZE 2
    #define TL_WCHAR_SHIFT 1
  #endif

  #if defined (_WIN32) || defined (__CYGWIN__)

    #define TL_WINDOWS
    #define TL_EXPORT
    #define TL_TM_EXPORT

    // #define TL_EXPORT    __declspec (dllexport) leads to linker errors
    // #define TL_TM_EXPORT __declspec (dllexport) leads to linker errors

  #elif defined (unix) || defined (__unix) || defined (__unix__)

    #if defined (linux) || defined (__linux) || defined (__linux__)

      #define TL_LINUX
      #define TL_UNIX
      #define TL_EXPORT
      #define TL_TM_EXPORT

    #else
      #error Unknown unix system for GNU compiler
    #endif

  #else
    #error Unknown operating system for GNU compiler
  #endif

#elif defined _MSC_VER

  #if _MSC_VER < 1400
    #error This compiler version is not supported. Use MSVC 8 (2005) or higher.
  #endif

  #define TL_MICROSOFT
  #define TL_WINDOWS
  #define TL_TYPENAME typename
  #define TL_TMU_TYPENAME typename

  #define TL_FMT_INT "I"
  #define TL_FMT_UINT "I"
  #define TL_FMT_INT64 "I64"
  #define TL_FMT_UINT64 "I64"

  #ifdef TL_STATIC
    #define TL_EXPORT
    #define TL_TM_EXPORT
  #else // TL_STATIC
    #ifdef TL_BUILD_DLL
      #define TL_EXPORT    _declspec (dllexport)
      #define TL_TM_EXPORT _declspec (dllexport)
    #else // TL_BUILD_DLL
      #define TL_EXPORT    //_declspec (dllimport)
      #define TL_TM_EXPORT //_declspec (dllimport)
    #endif // TL_BUILD_DLL
  #endif // TL_STATIC

  #if defined _MT
    #define TL_MULTI
  #endif

  #if defined _CPPUNWIND
    #define TL_EXCEPTION
    #define TL_CAN_THROW_ throw (...)
  #endif

  #if defined _WIN64
    #define TL_64
  #endif

  #ifndef _PTRDIFF_T_DEFINED
    #ifdef _WIN64
      typedef __int64 ptrdiff_t;
    #else
      typedef _W64 int ptrdiff_t;
    #endif
    #define _PTRDIFF_T_DEFINED
  #endif

  typedef __int64          t_Int64;
  typedef unsigned __int64 t_UInt64;

  #define TL_WCHAR_SIZE 2
  #define TL_WCHAR_SHIFT 1

  #pragma inline_depth (32)

#else

  #error Unknown C++ compiler

#endif

//---------------------------------------------------------------------------
// Common macros and data types

#if !defined NDEBUG
  #define TL_DEBUG
#endif

#if defined TL_DEBUG

  #include <assert.h>
  #define TL_ASSERT(x) assert(x)
  #define TL_DEBUG_CODE(x) x

#else

  #define TL_ASSERT(x)
  #define TL_DEBUG_CODE(x)

#endif

typedef ptrdiff_t      t_Int;
typedef size_t         t_UInt;
typedef signed char    t_Int8;
typedef unsigned char  t_UInt8;
typedef signed short   t_Int16;
typedef unsigned short t_UInt16;
typedef signed int     t_Int32;
typedef unsigned int   t_UInt32;

typedef void *       t_VoidPtr;
typedef const void * t_ConstVoidPtr;
typedef char *       t_CharPtr;
typedef const char * t_ConstCharPtr;
typedef wchar_t *       t_WCharPtr;
typedef const wchar_t * t_ConstWCharPtr;

#if defined TL_64
  const t_UInt u_MaxIntu = 0x7FFFFFFFFFFFFFFFull;
#else
  const t_UInt u_MaxIntu = 0x7FFFFFFFul;
#endif

#if defined TL_EXCEPTION

  #define TL_TRY(code)     try { code }
  #define TL_CATCH(code)   catch (...) { code }
  #define TL_NO_THROW      throw ()
  #define TL_CAN_THROW     TL_CAN_THROW_

#else

  #define TL_TRY(code)     { code }
  #define TL_CATCH(code)
  #define TL_NO_THROW
  #define TL_CAN_THROW

#endif

//---------------------------------------------------------------------------
// Min/Max functions

template <class t_comp>
  t_comp tl_Min (t_comp o_value1, t_comp o_value2)
    {
    if (o_value1 < o_value2)
      return o_value1;
    else
      return o_value2;
    }

template <class t_comp>
  t_comp tl_Max (t_comp o_value1, t_comp o_value2)
    {
    if (o_value1 > o_value2)
      return o_value1;
    else
      return o_value2;
    }

//---------------------------------------------------------------------------
// Global stores

#define ADDROF_DCL_INLINE                                                  \
  static inline void * AddrOf (t_Position o_pos) { return o_pos; }

#define ADDROF_DCL_STATIC                                                  \
  static void * AddrOf (t_Position o_pos);

#define ADDROF_DEF_INLINE(Obj, Size)

#define ADDROF_DEF_STATIC(Obj, Size)                                       \
  void * ct_ ## Obj ## Size ## Store::AddrOf (t_Position o_pos)            \
    { return Get ## Obj ## Store ()-> AddrOf (o_pos); }                    \

#define POSOF_DCL_INLINE                                                   \
  static inline t_Position PosOf (void * pv_adr) { return pv_adr; }

#define POSOF_DCL_STATIC                                                   \
  static t_Position PosOf (void * pv_adr);

#define POSOF_DEF_INLINE(Obj, Size)

#define POSOF_DEF_STATIC(Obj, Size)                                        \
  ct_ ## Obj ## Size ## Store::t_Position                                  \
  ct_ ## Obj ## Size ## Store::PosOf (void * pv_adr)                       \
    { return Get ## Obj ## Store ()-> PosOf (pv_adr); }                    \

#define GLOBAL_STORE_DCL(t_store, Obj, Size, t_size, inl_or_stat)          \
  class TL_EXPORT ct_ ## Obj ## Size ## Store                              \
    {                                                                      \
  public:                                                                  \
    typedef t_size              t_Size;                                    \
    typedef t_store::t_Position t_Position;                                \
    typedef t_store             t_Store;                                   \
    static void          Swap (ct_ ## Obj ## Size ## Store &) { }          \
    static t_UInt        StoreInfoSize ();                                 \
    static t_UInt        MaxAlloc ();                                      \
    static t_Position    Alloc (t_Size);                                   \
    static t_Position    Realloc (t_Position, t_Size);                     \
    static void          Free (t_Position);                                \
    ADDROF_DCL_ ## inl_or_stat                                             \
    POSOF_DCL_ ## inl_or_stat                                              \
    static inline t_Size SizeOf (t_Position o_pos)                         \
                           { return (t_Size) t_Store::SizeOf (o_pos); }    \
    static t_Size        RoundedSizeOf (t_Position);                       \
    static bool          CanFreeAll () { return false; }                   \
    static void          FreeAll () { TL_ASSERT (false); }                 \
    static t_store *     GetStore ();                                      \
    };                                                                     \
  inline t_UInt ct_ ## Obj ## Size ## Store::StoreInfoSize ()              \
    { return Get ## Obj ## Store ()-> StoreInfoSize (); }                  \
  inline t_UInt ct_ ## Obj ## Size ## Store::MaxAlloc ()                   \
    { return tl_Min (Get ## Obj ## Store ()-> MaxAlloc (),                 \
       (t_UInt)((t_size) -1)); }

#define GLOBAL_STORE_DEF(t_store, Obj, Size, inl_or_stat)                  \
  ct_ ## Obj ## Size ## Store::t_Position                                  \
  ct_ ## Obj ## Size ## Store::Alloc (t_Size o_s)                          \
    { return Get ## Obj ## Store ()-> Alloc (o_s); }                       \
  ct_ ## Obj ## Size ## Store::t_Position                                  \
  ct_ ## Obj ## Size ## Store::Realloc (t_Position o_p, t_Size o_s)        \
    { return Get ## Obj ## Store ()-> Realloc (o_p, o_s); }                \
  void ct_ ## Obj ## Size ## Store::Free (t_Position o_p)                  \
    { Get ## Obj ## Store ()-> Free (o_p); }                               \
  ADDROF_DEF_ ## inl_or_stat (Obj, Size)                                   \
  POSOF_DEF_ ## inl_or_stat (Obj, Size)                                    \
  ct_ ## Obj ## Size ## Store::t_Size                                      \
  ct_ ## Obj ## Size ## Store::RoundedSizeOf (t_Position o_pos)            \
    { return (t_Size) Get ## Obj ## Store ()-> RoundedSizeOf (o_pos); }    \
  t_store * ct_ ## Obj ## Size ## Store::GetStore ()                       \
    { return Get ## Obj ## Store (); }

#define GLOBAL_STORE_DCLS(t_store, Obj, inl_or_stat)                       \
  typedef t_store * t_ ## Obj ## StorePtr;                                 \
  void TL_EXPORT Create ## Obj ## Store ();                                \
  void TL_EXPORT Delete ## Obj ## Store ();                                \
  t_ ## Obj ## StorePtr TL_EXPORT Get ## Obj ## Store ();                  \
  GLOBAL_STORE_DCL (t_store, Obj, _,  t_UInt,   inl_or_stat)               \
  GLOBAL_STORE_DCL (t_store, Obj, 8,  t_UInt8,  inl_or_stat)               \
  GLOBAL_STORE_DCL (t_store, Obj, 16, t_UInt16, inl_or_stat)               \
  GLOBAL_STORE_DCL (t_store, Obj, 32, t_UInt32,  inl_or_stat)

#define GLOBAL_STORE_DEFS(t_store, Obj, inl_or_stat)                       \
  static t_store * pco_ ## Obj ## Store;                                   \
  void TL_EXPORT Create ## Obj ## Store ()                                 \
    {                                                                      \
    if (pco_ ## Obj ## Store == 0)                                         \
      {                                                                    \
      pco_ ## Obj ## Store = new t_store;                                  \
      TL_ASSERT (pco_ ## Obj ## Store != 0);                               \
      }                                                                    \
    }                                                                      \
  void TL_EXPORT Delete ## Obj ## Store ()                                 \
    {                                                                      \
    if (pco_ ## Obj ## Store != 0)                                         \
      {                                                                    \
      delete pco_ ## Obj ## Store;                                         \
      pco_ ## Obj ## Store = 0;                                            \
      }                                                                    \
    }                                                                      \
  t_ ## Obj ## StorePtr TL_EXPORT Get ## Obj ## Store ()                   \
    {                                                                      \
    if (pco_ ## Obj ## Store == 0)                                         \
      Create ## Obj ## Store ();                                           \
    return pco_ ## Obj ## Store;                                           \
    }                                                                      \
  GLOBAL_STORE_DEF (t_store, Obj, _,  inl_or_stat)                         \
  GLOBAL_STORE_DEF (t_store, Obj, 8,  inl_or_stat)                         \
  GLOBAL_STORE_DEF (t_store, Obj, 16, inl_or_stat)                         \
  GLOBAL_STORE_DEF (t_store, Obj, 32, inl_or_stat)

//---------------------------------------------------------------------------
// User code

#if defined TL_USER
  #include <tl_user.hpp>
#endif

#endif
