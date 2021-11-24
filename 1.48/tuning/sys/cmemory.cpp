
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
// File tuning/sys/cmemory.cpp

#include "tuning/sys/cmemory.hpp"

//---------------------------------------------------------------------------

t_ConstCharPtr TL_EXPORT tl_FirstMemory
(
const char * pc_mem,
t_UInt u_len,
const char * pc_search,
t_UInt u_searchLen
)
  {
  if ((u_len > 0) && (u_searchLen > 0))
    {
    TL_ASSERT (pc_mem != 0);
    TL_ASSERT (pc_search != 0);

    char c_search = * pc_search;
    const char * pc_found = (const char *) memchr (pc_mem, c_search, u_len);

    if (u_searchLen == 1)
      return pc_found;

    while (pc_found != 0)
      {
      u_len -= (pc_found - pc_mem);

      if (u_len < u_searchLen)
        return 0;

      if (memcmp (pc_found, pc_search, u_searchLen) == 0)
        return pc_found;

      pc_mem = pc_found + 1;
      pc_found = (char *) memchr (pc_mem, c_search, -- u_len);
      }
    }

  return 0;
  }

//---------------------------------------------------------------------------

t_ConstWCharPtr TL_EXPORT tl_FirstMemory
(
const wchar_t * pc_mem,
t_UInt u_len,
const wchar_t * pc_search,
t_UInt u_searchLen
)
  {
  if ((u_len > 0) && (u_searchLen > 0))
    {
    TL_ASSERT (pc_mem != 0);
    TL_ASSERT (pc_search != 0);

    wchar_t c_search = * pc_search;
    const wchar_t * pc_found = tl_wmemchr (pc_mem, c_search, u_len);

    if (u_searchLen == 1)
      return pc_found;

    while (pc_found != 0)
      {
      u_len -= (pc_found - pc_mem);

      if (u_len < u_searchLen)
        return 0;

      if (memcmp (pc_found, pc_search, u_searchLen << TL_WCHAR_SHIFT) == 0)
        return pc_found;

      pc_mem = pc_found + 1;
      pc_found = tl_wmemchr (pc_mem, c_search, -- u_len);
      }
    }

  return 0;
  }

//---------------------------------------------------------------------------

t_ConstCharPtr TL_EXPORT tl_LastChar
(
const char * pc_mem,
t_UInt u_len,
char c_search
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_mem != 0);

    const char * pc_found = (const char *) memchr (pc_mem, c_search, u_len);

    if (pc_found != 0)
      {
      for (;;)
        {
        u_len -= (pc_found - pc_mem) + 1;

        if (u_len == 0)
          return pc_found;

        pc_mem = pc_found + 1;
        pc_found = (const char *) memchr (pc_mem, c_search, u_len);

        if (pc_found == 0)
          return pc_mem - 1;
        }
      }
    }

  return 0;
  }

//---------------------------------------------------------------------------

t_ConstWCharPtr TL_EXPORT tl_LastChar
(
const wchar_t * pc_mem,
t_UInt u_len,
wchar_t c_search
)
  {
  if (u_len > 0)
    {
    TL_ASSERT (pc_mem != 0);

    const wchar_t * pc_found = tl_wmemchr (pc_mem, c_search, u_len);

    if (pc_found != 0)
      {
      for (;;)
        {
        u_len -= (pc_found - pc_mem) + 1;

        if (u_len == 0)
          return pc_found;

        pc_mem = pc_found + 1;
        pc_found = tl_wmemchr (pc_mem, c_search, u_len);

        if (pc_found == 0)
          return pc_mem - 1;
        }
      }
    }

  return 0;
  }

//---------------------------------------------------------------------------

t_ConstCharPtr TL_EXPORT tl_LastMemory
(
const char * pc_mem,
t_UInt u_len,
const char * pc_search,
t_UInt u_searchLen
)
  {
  if ((u_len > 0) && (u_searchLen > 0))
    {
    TL_ASSERT (pc_mem != 0);
    TL_ASSERT (pc_search != 0);

    char c_search = * pc_search;

    if (u_searchLen == 1)
      return tl_LastChar (pc_mem, u_len, c_search);

    const char * pc_found = (const char *) memchr (pc_mem, c_search, u_len);
    const char * pc_last = 0;

    while (pc_found != 0)
      {
      u_len -= (pc_found - pc_mem);

      if (u_len < u_searchLen)
        break;

      if (memcmp (pc_found, pc_search, u_searchLen) == 0)
        pc_last = pc_found;

      pc_mem = pc_found + 1;
      pc_found = (char *) memchr (pc_mem, c_search, -- u_len);
      }

    return pc_last;
    }

  return 0;
  }

//---------------------------------------------------------------------------

t_ConstWCharPtr TL_EXPORT tl_LastMemory
(
const wchar_t * pc_mem,
t_UInt u_len,
const wchar_t * pc_search,
t_UInt u_searchLen
)
  {
  if ((u_len > 0) && (u_searchLen > 0))
    {
    TL_ASSERT (pc_mem != 0);
    TL_ASSERT (pc_search != 0);

    wchar_t c_search = * pc_search;

    if (u_searchLen == 1)
      return tl_LastChar (pc_mem, u_len, c_search);

    const wchar_t * pc_found = tl_wmemchr (pc_mem, c_search, u_len);
    const wchar_t * pc_last = 0;

    while (pc_found != 0)
      {
      u_len -= (pc_found - pc_mem);

      if (u_len < u_searchLen)
        break;

      if (memcmp (pc_found, pc_search, u_searchLen << TL_WCHAR_SHIFT) == 0)
        pc_last = pc_found;

      pc_mem = pc_found + 1;
      pc_found = tl_wmemchr (pc_mem, c_search, -- u_len);
      }

    return pc_last;
    }

  return 0;
  }
