
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
// File tuning/printf.hpp

#ifndef TUNING_PRINTF_HPP
#define TUNING_PRINTF_HPP

#include "tuning/defs.hpp"
#include <stdarg.h>
#include <stdlib.h>

//---------------------------------------------------------------------------

int TL_EXPORT        tl_VSprintf (char * * ppc_buffer, const char * pc_format, va_list o_argList);
int TL_EXPORT        tl_VSprintf (wchar_t * * ppc_buffer, const wchar_t * pc_format, va_list o_argList);

#endif
