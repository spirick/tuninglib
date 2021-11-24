
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
// File tuning/sys/ctimedate.hpp

#ifndef TUNING_SYS_CTIMEDAT_HPP
#define TUNING_SYS_CTIMEDAT_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------

typedef t_Int64      t_MicroTime;
const t_MicroTime    co_MicroSecondFactor =           1ll;
const t_MicroTime    co_MilliSecondFactor =        1000ll;
const t_MicroTime    co_SecondFactor      =     1000000ll;
const t_MicroTime    co_MinuteFactor      =    60000000ll;
const t_MicroTime    co_HourFactor        =  3600000000ll;
const t_MicroTime    co_DayFactor         = 86400000000ll;

const t_MicroTime    co_1970 = 116444736000000000ll;

//---------------------------------------------------------------------------
// Precision time in microseconds

t_MicroTime TL_EXPORT tl_QueryPrecisionTime ();

//---------------------------------------------------------------------------
// Number of microseconds since 1970-01-01 00:00:00 +0000 (UTC)

t_MicroTime TL_EXPORT tl_QueryUTCTime ();
t_MicroTime TL_EXPORT tl_QueryLocalTime ();

//---------------------------------------------------------------------------
// Time conversion

t_MicroTime TL_EXPORT tl_UTCToLocalTime (t_MicroTime i_time);
t_MicroTime TL_EXPORT tl_LocalToUTCTime (t_MicroTime i_time);

//---------------------------------------------------------------------------
// User and kernel mode times in microseconds

struct st_UserKernelTime
  {
  t_MicroTime          o_UserTime;
  t_MicroTime          o_KernelTime;
  };

bool TL_EXPORT       tl_QueryProcessTimes (st_UserKernelTime * pso_times);
bool TL_EXPORT       tl_QueryThreadTimes (st_UserKernelTime * pso_times);

#endif
