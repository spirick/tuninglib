
// Spirick Tuning
//
// A C++ class and template library
// for performance critical applications.
// Copyright (C) 1996-2023 Dietmar Deimling.
// All rights reserved.
// Internet  www.spirick.com
// E-Mail    info@spirick.com
//
// Version 1.49
// File tuning/sys/cmemory.hpp

#ifndef TUNING_SYS_CMEMORY_HPP
#define TUNING_SYS_CMEMORY_HPP

#include <string.h>
#include <wchar.h>
#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

void TL_EXPORT       tl_CopyMemory (char * pc_dst, const char * pc_src,
                       t_UInt u_len);
void TL_EXPORT       tl_CopyMemory (wchar_t * pc_dst, const wchar_t * pc_src,
                       t_UInt u_len);
void TL_EXPORT       tl_MoveMemory (char * pc_dst, const char * pc_src,
                       t_UInt u_len);
void TL_EXPORT       tl_MoveMemory (wchar_t * pc_dst, const wchar_t * pc_src,
                       t_UInt u_len);
t_CharPtr TL_EXPORT  tl_FillMemory (char * pc_dst, t_UInt u_len,
                       char c_fill);
t_WCharPtr TL_EXPORT tl_FillMemory (wchar_t * pc_dst, t_UInt u_len,
                       wchar_t c_fill);

//---------------------------------------------------------------------------

inline int TL_EXPORT tl_CompareChar (char c1, char c2);
inline int TL_EXPORT tl_CompareChar (wchar_t c1, wchar_t c2);
int TL_EXPORT        tl_CompareMemory (const char * pc1, const char * pc2,
                       t_UInt u_len);
int TL_EXPORT        tl_CompareMemory (const wchar_t * pc1, const wchar_t * pc2,
                       t_UInt u_len);

//---------------------------------------------------------------------------

t_ConstCharPtr TL_EXPORT  tl_FirstChar (const char * pc_mem, t_UInt u_len,
                            char c_search);
t_ConstWCharPtr TL_EXPORT tl_FirstChar (const wchar_t * pc_mem, t_UInt u_len,
                            wchar_t c_search);
t_ConstCharPtr TL_EXPORT  tl_FirstMemory (const char * pc_mem, t_UInt u_len,
                            const char * pc_search, t_UInt u_searchLen);
t_ConstWCharPtr TL_EXPORT tl_FirstMemory (const wchar_t * pc_mem, t_UInt u_len,
                            const wchar_t * pc_search, t_UInt u_searchLen);
t_ConstCharPtr TL_EXPORT  tl_LastChar (const char * pc_mem, t_UInt u_len,
                            char c_search);
t_ConstWCharPtr TL_EXPORT tl_LastChar (const wchar_t * pc_mem, t_UInt u_len,
                            wchar_t c_search);
t_ConstCharPtr TL_EXPORT  tl_LastMemory (const char * pc_mem, t_UInt u_len,
                            const char * pc_search, t_UInt u_searchLen);
t_ConstWCharPtr TL_EXPORT tl_LastMemory (const wchar_t * pc_mem, t_UInt u_len,
                            const wchar_t * pc_search, t_UInt u_searchLen);

//---------------------------------------------------------------------------

template <t_UInt u_len>
  void tl_SwapMemory (void * pv1, void * pv2)
    {
    if (pv1 != pv2)
      {
      char ac_local [u_len];
      tl_CopyMemory (ac_local,     (char *) pv1, u_len);
      tl_CopyMemory ((char *) pv1, (char *) pv2, u_len);
      tl_CopyMemory ((char *) pv2, ac_local,     u_len);
      }
    }

//---------------------------------------------------------------------------

template <class t_obj>
  void tl_SwapObj (t_obj & o1, t_obj & o2)
    {
    if (& o1 != & o2)
      {
      t_obj o_local = o1;
      o1 = o2;
      o2 = o_local;
      }
    }

//===========================================================================

#if defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

//---------------------------------------------------------------------------

inline void tl_wmemcpy
(
wchar_t * pc_dst,
const wchar_t * pc_src,
t_UInt u_len
)
  {
  memcpy (pc_dst, pc_src, u_len << 1);
  }

//---------------------------------------------------------------------------

inline void tl_wmemmove
(
wchar_t * pc_dst,
const wchar_t * pc_src,
t_UInt u_len
)
  {
  memmove (pc_dst, pc_src, u_len << 1);
  }

//---------------------------------------------------------------------------

inline void tl_wmemset
(
wchar_t * pc_dst,
wchar_t c_fill,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    * (pc_dst ++) = c_fill;
  }

//---------------------------------------------------------------------------

inline int tl_wmemcmp
(
const wchar_t * pc1,
const wchar_t * pc2,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    {
    int i_cmp = tl_CompareChar (* (pc1 ++), * (pc2 ++));

    if (i_cmp != 0)
      return i_cmp;
    }

  return 0;
  }

//---------------------------------------------------------------------------

inline const wchar_t * tl_wmemchr
(
const wchar_t * pc_mem,
wchar_t c_search,
t_UInt u_len
)
  {
  while (u_len -- != 0)
    {
    if (* pc_mem == c_search)
      return pc_mem;
    else
      pc_mem ++;
    }

  return 0;
  }

//===========================================================================

#else

#define tl_wmemcpy  wmemcpy
#define tl_wmemmove wmemmove
#define tl_wmemset  wmemset
#define tl_wmemcmp  wmemcmp
#define tl_wmemchr  wmemchr

#endif

//---------------------------------------------------------------------------

inline void TL_EXPORT tl_CopyMemory
(
char * pc_dst,
const char * pc_src,
t_UInt u_len
)
  {
  if ((u_len > 0) && (u_len <= u_MaxIntu))
    {
    TL_ASSERT (pc_dst != 0);
    TL_ASSERT (pc_src != 0);

    memcpy (pc_dst, pc_src, u_len);
    }
  }

//---------------------------------------------------------------------------

inline void TL_EXPORT tl_CopyMemory
(
wchar_t * pc_dst,
const wchar_t * pc_src,
t_UInt u_len
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_dst != 0);
    TL_ASSERT (pc_src != 0);

    tl_wmemcpy (pc_dst, pc_src, u_len);
    }
  }

//---------------------------------------------------------------------------

inline void TL_EXPORT tl_MoveMemory
(
char * pc_dst,
const char * pc_src,
t_UInt u_len
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_dst != 0);
    TL_ASSERT (pc_src != 0);

    memmove (pc_dst, pc_src, u_len);
    }
  }

//---------------------------------------------------------------------------

inline void TL_EXPORT tl_MoveMemory
(
wchar_t * pc_dst,
const wchar_t * pc_src,
t_UInt u_len
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_dst != 0);
    TL_ASSERT (pc_src != 0);

    tl_wmemmove (pc_dst, pc_src, u_len);
    }
  }

//---------------------------------------------------------------------------

inline t_CharPtr TL_EXPORT tl_FillMemory
(
char * pc_dst,
t_UInt u_len,
char c_fill
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_dst != 0);

    memset (pc_dst, c_fill, u_len);
    }

  return pc_dst;
  }

//---------------------------------------------------------------------------

inline t_WCharPtr TL_EXPORT tl_FillMemory
(
wchar_t * pc_dst,
t_UInt u_len,
wchar_t c_fill
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_dst != 0);

    tl_wmemset (pc_dst, c_fill, u_len);
    }

  return pc_dst;
  }

//---------------------------------------------------------------------------

inline int TL_EXPORT tl_CompareChar
(
char c1,
char c2
)
  {
  return ((unsigned) ((unsigned char) c1)) - ((unsigned) ((unsigned char) c2));
  }

//---------------------------------------------------------------------------

inline int TL_EXPORT tl_CompareChar
(
wchar_t c1,
wchar_t c2
)
  {
  return ((unsigned) c1) - ((unsigned) c2);
  }

//---------------------------------------------------------------------------

inline int TL_EXPORT tl_CompareMemory
(
const char * pc1,
const char * pc2,
t_UInt u_len
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc1 != 0);
    TL_ASSERT (pc2 != 0);

    return memcmp (pc1, pc2, u_len);
    }

  return 0;
  }

//---------------------------------------------------------------------------

inline int TL_EXPORT tl_CompareMemory
(
const wchar_t * pc1,
const wchar_t * pc2,
t_UInt u_len
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc1 != 0);
    TL_ASSERT (pc2 != 0);

    return tl_wmemcmp (pc1, pc2, u_len);
    }

  return 0;
  }

//---------------------------------------------------------------------------

inline t_ConstCharPtr TL_EXPORT tl_FirstChar
(
const char * pc_mem,
t_UInt u_len,
char c_search
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_mem != 0);

    return (t_ConstCharPtr) memchr (pc_mem, c_search, u_len);
    }

  return 0;
  }

//---------------------------------------------------------------------------

inline t_ConstWCharPtr TL_EXPORT tl_FirstChar
(
const wchar_t * pc_mem,
t_UInt u_len,
wchar_t c_search
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_mem != 0);

    return (t_ConstWCharPtr) tl_wmemchr (pc_mem, c_search, u_len);
    }

  return 0;
  }

#endif
