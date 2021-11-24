
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
// File tuning/timedate.cpp

#include "tuning/timedate.hpp"
#include "tuning/sys/ctimedate.hpp"
#include "tuning/chn/block.h"

//---------------------------------------------------------------------------

ct_TimeDate::ct_TimeDate ()
  {
  Clear ();
  }

//---------------------------------------------------------------------------

ct_TimeDate::ct_TimeDate
(
t_MicroTime i_time
)
  {
  SetTime (i_time);
  }

//---------------------------------------------------------------------------

void ct_TimeDate::Clear ()
  {
  u_Year      = 0;
  u_Month     = 0;
  u_Day       = 0;
  u_DayOfWeek = 0;
  u_Hour      = 0;
  u_Minute    = 0;
  u_Second    = 0;
  u_MicroSecond = 0;
  }

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>

//---------------------------------------------------------------------------

t_MicroTime ct_TimeDate::GetTime () const
  {
  SYSTEMTIME so_sysTime;
  so_sysTime. wYear = u_Year;
  so_sysTime. wMonth = u_Month;
  so_sysTime. wDay = u_Day;
  so_sysTime. wDayOfWeek = 0;
  so_sysTime. wHour = u_Hour;
  so_sysTime. wMinute = u_Minute;
  so_sysTime. wSecond = u_Second;
  so_sysTime. wMilliseconds = 0;
  FILETIME so_fileTime;
  SystemTimeToFileTime (& so_sysTime, & so_fileTime);
  ULARGE_INTEGER uo_li;
  uo_li. u. LowPart = so_fileTime. dwLowDateTime;
  uo_li. u. HighPart = so_fileTime. dwHighDateTime;
  return (uo_li. QuadPart - co_1970) / 10ll + u_MicroSecond;
  }

//---------------------------------------------------------------------------

void ct_TimeDate::SetTime
(
t_MicroTime i_time
)
  {
  ULARGE_INTEGER uo_li;
  uo_li. QuadPart = i_time * 10ll + co_1970;
  FILETIME so_fileTime;
  so_fileTime. dwLowDateTime = uo_li. u. LowPart;
  so_fileTime. dwHighDateTime = uo_li. u. HighPart;
  SYSTEMTIME so_sysTime;
  FileTimeToSystemTime (& so_fileTime, & so_sysTime);
  u_Year = so_sysTime. wYear;
  u_Month = (t_UInt8) so_sysTime. wMonth;
  u_Day = (t_UInt8) so_sysTime. wDay;
  u_DayOfWeek = (t_UInt8) so_sysTime. wDayOfWeek;
  u_Hour = (t_UInt8) so_sysTime. wHour;
  u_Minute = (t_UInt8) so_sysTime. wMinute;
  u_Second = (t_UInt8) so_sysTime. wSecond;
  u_MicroSecond = (t_UInt32) (i_time % co_SecondFactor);
  }

//===========================================================================

#else

#include <time.h>

//---------------------------------------------------------------------------

t_MicroTime ct_TimeDate::GetTime () const
  {
  struct tm so_tm;
  so_tm. tm_year = u_Year - 1900;
  so_tm. tm_mon = u_Month - 1;
  so_tm. tm_mday = u_Day;
  so_tm. tm_wday = 0;
  so_tm. tm_yday = 0;
  so_tm. tm_hour = u_Hour;
  so_tm. tm_min = u_Minute;
  so_tm. tm_sec = u_Second;
  so_tm. tm_isdst = 0;
  return timegm (& so_tm) * co_SecondFactor + u_MicroSecond;
  }

//---------------------------------------------------------------------------

void ct_TimeDate::SetTime
(
t_MicroTime i_time
)
  {
  time_t o_seconds = i_time / co_SecondFactor;
  struct tm so_tm;
  gmtime_r (& o_seconds, & so_tm);
  u_Year = (t_UInt16) so_tm. tm_year + 1900;
  u_Month = (t_UInt8) so_tm. tm_mon + 1;
  u_Day = (t_UInt8) so_tm. tm_mday;
  u_DayOfWeek = (t_UInt8) so_tm. tm_wday;
  u_Hour = (t_UInt8) so_tm. tm_hour;
  u_Minute = (t_UInt8) so_tm. tm_min;
  u_Second = (t_UInt8) so_tm. tm_sec;
  u_MicroSecond = (t_UInt32) (i_time % co_SecondFactor);
  }

#endif

//---------------------------------------------------------------------------

void ct_TimeDate::QueryUTCTime ()
  {
  SetTime (tl_QueryUTCTime ());
  }

//---------------------------------------------------------------------------

void ct_TimeDate::QueryLocalTime ()
  {
  SetTime (tl_QueryLocalTime ());
  }
