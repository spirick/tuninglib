
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
// File tuning/sys/cprocess.hpp

#ifndef TUNING_SYS_CPROCESS_HPP
#define TUNING_SYS_CPROCESS_HPP

#include "tuning/sys/cthread.hpp"
#include "tuning/sys/cthmutex.hpp"
#include "tuning/sys/cthsemaphore.hpp"
#include "tuning/sys/cprmutex.hpp"
#include "tuning/sys/cprsemaphore.hpp"
#include "tuning/sys/csharedmem.hpp"

//---------------------------------------------------------------------------
// Interlocked memory access

t_Int32 TL_EXPORT    tl_InterlockedRead (volatile t_Int32 * pi_value);
t_Int32 TL_EXPORT    tl_InterlockedWrite (volatile t_Int32 * pi_value, t_Int32 i_new);
t_Int32 TL_EXPORT    tl_InterlockedAdd (volatile t_Int32 * pi_value, t_Int32 i_add);
t_Int32 TL_EXPORT    tl_InterlockedIncrement (volatile t_Int32 * pi_value);
t_Int32 TL_EXPORT    tl_InterlockedDecrement (volatile t_Int32 * pi_value);

//---------------------------------------------------------------------------
// Delay

void TL_EXPORT       tl_Delay (int i_milliSec);
void TL_EXPORT       tl_RelinquishTimeSlice ();

//---------------------------------------------------------------------------
// Environment variable

ct_String TL_EXPORT  tl_GetEnv (const char * pc_name);
ct_String TL_EXPORT  tl_GetTempPath ();

//---------------------------------------------------------------------------
// Processes

void TL_EXPORT       tl_EndProcess (unsigned u_exitCode);
int TL_EXPORT        tl_ProcessId ();
bool TL_EXPORT       tl_IsProcessRunning (int i_processId);
int TL_EXPORT        tl_Exec (const char * pc_path, unsigned u_params,
                       const char * * ppc_params, bool b_wait = false);

#endif
