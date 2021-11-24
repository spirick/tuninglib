
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
// File tuning/printf.cpp

#include "tuning/printf.hpp"
#include <stdio.h>

//===========================================================================

#if defined TL_WINDOWS

//---------------------------------------------------------------------------

int tl_VSprintf (char * * ppc_buffer, const char * pc_format, va_list o_argList)
  {
  int i_len = _vscprintf (pc_format, o_argList);

  if (i_len < 0)
    * ppc_buffer = 0;
  else
    * ppc_buffer = (char *) malloc (i_len + 1);

  if (* ppc_buffer == 0)
    return -1;

  int i_len2 = _vsnprintf_s (* ppc_buffer, i_len + 1, i_len, pc_format, o_argList);

  if (i_len2 < 0)
    {
    free (* ppc_buffer);
    * ppc_buffer = 0;
    return -1;
    }

  TL_ASSERT (i_len == i_len2);
  return i_len2;
  }

//---------------------------------------------------------------------------

int tl_VSprintf (wchar_t * * ppc_buffer, const wchar_t * pc_format, va_list o_argList)
  {
  int i_len = _vscwprintf (pc_format, o_argList);

  if (i_len < 0)
    * ppc_buffer = 0;
  else
    * ppc_buffer = (wchar_t *) malloc ((i_len + 1) * sizeof (wchar_t));

  if (* ppc_buffer == 0)
    return -1;

  int i_len2 = _vsnwprintf_s (* ppc_buffer, i_len + 1, i_len, pc_format, o_argList);

  if (i_len2 < 0)
    {
    free (* ppc_buffer);
    * ppc_buffer = 0;
    return -1;
    }

  TL_ASSERT (i_len == i_len2);
  return i_len2;
  }

//===========================================================================

#else

#include <wchar.h>
#include "tuning/sys/cstring.hpp"

//---------------------------------------------------------------------------

int tl_VSprintf (char * * ppc_buffer, const char * pc_format, va_list o_argList)
  {
  return vasprintf (ppc_buffer, pc_format, o_argList);
  }

//---------------------------------------------------------------------------

int tl_VSprintf (wchar_t * * ppc_buffer, const wchar_t * pc_format, va_list o_argList)
  {
  const int i_VSprintfMax = 1024 - 8;

  #if defined (TL_GNU) && (TL_WCHAR_SIZE == 2)

  * ppc_buffer = (wchar_t *) malloc (i_VSprintfMax * sizeof (wint_t));

  if (* ppc_buffer == 0)
    return -1;

  t_UInt u_formatLen = tl_StringLength (pc_format) + 1;
  wint_t * pc_iformat = (wint_t *) malloc (u_formatLen * sizeof (wint_t));

  if (pc_iformat == 0)
    {
    free (* ppc_buffer);
    * ppc_buffer = 0;
    return -1;
    }

  wint_t * pc_i = pc_iformat;

  while (u_formatLen -- != 0)
    * (pc_i ++) = * (pc_format ++);

  int i_len = vswprintf (* ppc_buffer, i_VSprintfMax, (wchar_t *) pc_iformat, o_argList);
  free (pc_iformat);

  if ((i_len < 0) || (i_len >= i_VSprintfMax))
    {
    free (* ppc_buffer);
    * ppc_buffer = 0;
    return -1;
    }

  unsigned u_len = i_len + 1;
  wchar_t * pc_buffer = * ppc_buffer;
  pc_i = (wint_t *) * ppc_buffer;

  while (u_len -- != 0)
    * (pc_buffer ++) = (wchar_t) * (pc_i ++);

  return i_len;

  #else // ---------------

  * ppc_buffer = (wchar_t *) malloc (i_VSprintfMax * sizeof (wchar_t));

  if (* ppc_buffer == 0)
    return -1;

  int i_len = vswprintf (* ppc_buffer, i_VSprintfMax, pc_format, o_argList);

  if ((i_len < 0) || (i_len >= i_VSprintfMax))
    {
    free (* ppc_buffer);
    * ppc_buffer = 0;
    return -1;
    }

  return i_len;

  #endif
  }

#endif
