
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
// File tuning/sys/cstring.hpp

#ifndef TUNING_SYS_CSTRING_HPP
#define TUNING_SYS_CSTRING_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

char TL_EXPORT       tl_ToUpper (char c);
wchar_t TL_EXPORT    tl_ToUpper (wchar_t c);
char TL_EXPORT       tl_ToLower (char c);
wchar_t TL_EXPORT    tl_ToLower (wchar_t c);

bool TL_EXPORT       tl_ToUpper (char * pc_str);
bool TL_EXPORT       tl_ToUpper (wchar_t * pc_str);
bool TL_EXPORT       tl_ToLower (char * pc_str);
bool TL_EXPORT       tl_ToLower (wchar_t * pc_str);

wchar_t TL_EXPORT    tl_ToUpper2 (wchar_t c);
wchar_t TL_EXPORT    tl_ToLower2 (wchar_t c);

bool TL_EXPORT       tl_ToUpper2 (char * pc_str);
bool TL_EXPORT       tl_ToUpper2 (wchar_t * pc_str);
bool TL_EXPORT       tl_ToLower2 (char * pc_str);
bool TL_EXPORT       tl_ToLower2 (wchar_t * pc_str);

t_UInt TL_EXPORT     tl_StringLength (const char * pc);
t_UInt TL_EXPORT     tl_StringLength (const wchar_t * pc);
unsigned TL_EXPORT   tl_StringHash (const char * pc, t_UInt u_length);
unsigned TL_EXPORT   tl_StringHash (const wchar_t * pc, t_UInt u_length);

t_UInt TL_EXPORT     tl_MbConvertCount (char *, const char * pc_src);
bool TL_EXPORT       tl_MbConvert (char * pc_dst, const char * pc_src, t_UInt u_count);
t_UInt TL_EXPORT     tl_MbConvertCount (wchar_t *, const wchar_t * pc_src);
bool TL_EXPORT       tl_MbConvert (wchar_t * pc_dst, const wchar_t * pc_src, t_UInt u_count);
t_UInt TL_EXPORT     tl_MbConvertCount (wchar_t *, const char * pc_src);
bool TL_EXPORT       tl_MbConvert (wchar_t * pc_dst, const char * pc_src, t_UInt u_count);
t_UInt TL_EXPORT     tl_MbConvertCount (char *, const wchar_t * pc_src);
bool TL_EXPORT       tl_MbConvert (char * pc_dst, const wchar_t * pc_src, t_UInt u_count);

#endif
