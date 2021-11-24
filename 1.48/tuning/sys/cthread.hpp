
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
// File tuning/sys/cthread.hpp

#ifndef TUNING_SYS_CTHREAD_HPP
#define TUNING_SYS_CTHREAD_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------
// Threads

#if defined TL_MULTI

const unsigned       cu_MaxThreads = 48;

typedef void (* ft_ThreadFunc) (void *);

bool TL_EXPORT       tl_BeginThread (ft_ThreadFunc fo_func, void * pv_param,
                       t_UInt u_stackSize = 8u * 1024u);
void TL_EXPORT       tl_EndThread ();
t_UInt64 TL_EXPORT   tl_ThreadId ();

#endif

#endif
