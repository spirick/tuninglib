
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
// File tuning/sys/ctimedate.cpp

#include "tuning/sys/ctimedate.hpp"
#include "tuning/sys/cprocess.hpp"

//===========================================================================
// Precision time
//===========================================================================

static int i_ResetPrecisionTime;

#if defined TL_WINDOWS

#include <windows.h>

static unsigned u_PerfShift;
static double   d_PerfTick;
static LARGE_INTEGER uo_PerfCount;

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryPrecisionTime ()
  {
  if (i_ResetPrecisionTime == 0)
    {
    tl_EnterCriticalSection ();

    if (i_ResetPrecisionTime == 0)
      {
      i_ResetPrecisionTime = 1;
      QueryPerformanceFrequency (& uo_PerfCount);

      if (uo_PerfCount. QuadPart == 0)
        {
        u_PerfShift = 0;
        d_PerfTick = 0.0;
        }
      else
        {
        u_PerfShift = 0;
        d_PerfTick = 1e6 / uo_PerfCount. QuadPart;

        while (uo_PerfCount. QuadPart > 2000000)
          {
          uo_PerfCount. QuadPart >>= 1;
          u_PerfShift += 1;
          }

        d_PerfTick *= (1 << u_PerfShift);
        QueryPerformanceCounter (& uo_PerfCount);
        }
      }

    tl_LeaveCriticalSection ();
    }

  if (d_PerfTick == 0.0)
    return 0;

  LARGE_INTEGER uo_perfCount;
  QueryPerformanceCounter (& uo_perfCount);
  return (t_Int64) (((double)
    ((uo_perfCount. QuadPart - uo_PerfCount. QuadPart) >> u_PerfShift)) * d_PerfTick);
  }

//===========================================================================

#elif defined TL_UNIX

#include <time.h>
#include <sys/time.h>

static struct timeval so_TimeVal;

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryPrecisionTime ()
  {
  if (i_ResetPrecisionTime == 0)
    {
    tl_EnterCriticalSection ();

    if (i_ResetPrecisionTime == 0)
      {
      i_ResetPrecisionTime = 1;
      gettimeofday (& so_TimeVal, 0);
      }

    tl_LeaveCriticalSection ();
    }

  struct timeval so_timeVal;
  gettimeofday (& so_timeVal, 0);

  return (so_timeVal. tv_sec  - so_TimeVal. tv_sec ) * co_SecondFactor +
         (so_timeVal. tv_usec - so_TimeVal. tv_usec);
  }

#endif

//===========================================================================
// System time and date
//===========================================================================

#if defined TL_WINDOWS

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryUTCTime ()
  {
  SYSTEMTIME so_sysTime;
  GetSystemTime (& so_sysTime);
  FILETIME so_fileTime;
  SystemTimeToFileTime (& so_sysTime, & so_fileTime);
  ULARGE_INTEGER uo_li;
  uo_li. u. LowPart = so_fileTime. dwLowDateTime;
  uo_li. u. HighPart = so_fileTime. dwHighDateTime;
  return (uo_li. QuadPart - co_1970) / 10ll;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryLocalTime ()
  {
  SYSTEMTIME so_sysTime;
  GetLocalTime (& so_sysTime);
  FILETIME so_fileTime;
  SystemTimeToFileTime (& so_sysTime, & so_fileTime);
  ULARGE_INTEGER uo_li;
  uo_li. u. LowPart = so_fileTime. dwLowDateTime;
  uo_li. u. HighPart = so_fileTime. dwHighDateTime;
  return (uo_li. QuadPart - co_1970) / 10ll;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_UTCToLocalTime
(
t_MicroTime i_time
)
  {
  ULARGE_INTEGER uo_li;
  uo_li. QuadPart = i_time * 10ll + co_1970;
  FILETIME so_fileTime;
  so_fileTime. dwLowDateTime = uo_li. u. LowPart;
  so_fileTime. dwHighDateTime = uo_li. u. HighPart;
  FILETIME so_localFileTime;
  FileTimeToLocalFileTime (& so_fileTime, & so_localFileTime);
  uo_li. u. LowPart = so_localFileTime. dwLowDateTime;
  uo_li. u. HighPart = so_localFileTime. dwHighDateTime;
  return (uo_li. QuadPart - co_1970) / 10ll;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_LocalToUTCTime
(
t_MicroTime i_time
)
  {
  ULARGE_INTEGER uo_li;
  uo_li. QuadPart = i_time * 10ll + co_1970;
  FILETIME so_localFileTime;
  so_localFileTime. dwLowDateTime = uo_li. u. LowPart;
  so_localFileTime. dwHighDateTime = uo_li. u. HighPart;
  FILETIME so_fileTime;
  LocalFileTimeToFileTime (& so_localFileTime, & so_fileTime);
  uo_li. u. LowPart = so_fileTime. dwLowDateTime;
  uo_li. u. HighPart = so_fileTime. dwHighDateTime;
  return (uo_li. QuadPart - co_1970) / 10ll;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryProcessTimes
(
st_UserKernelTime * pso_times
)
  {
  if (pso_times == 0)
    return false;

  FILETIME so_creationTime;
  FILETIME so_exitTime;
  FILETIME so_kernelTime;
  FILETIME so_userTime;

  if (GetProcessTimes (GetCurrentProcess (), & so_creationTime,
      & so_exitTime, & so_kernelTime, & so_userTime))
    {
    ULARGE_INTEGER uo_li;
    uo_li. u. LowPart = so_userTime. dwLowDateTime;
    uo_li. u. HighPart = so_userTime. dwHighDateTime;
    pso_times-> o_UserTime = uo_li. QuadPart / 10ll;
    uo_li. u. LowPart = so_kernelTime. dwLowDateTime;
    uo_li. u. HighPart = so_kernelTime. dwHighDateTime;
    pso_times-> o_KernelTime = uo_li. QuadPart / 10ll;
    return true;
    }

  pso_times-> o_UserTime = 0;
  pso_times-> o_KernelTime = 0;
  return false;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryThreadTimes
(
st_UserKernelTime * pso_times
)
  {
  if (pso_times == 0)
    return false;

  FILETIME so_creationTime;
  FILETIME so_exitTime;
  FILETIME so_kernelTime;
  FILETIME so_userTime;

  if (GetThreadTimes (GetCurrentThread (), & so_creationTime,
      & so_exitTime, & so_kernelTime, & so_userTime))
    {
    ULARGE_INTEGER uo_li;
    uo_li. u. LowPart = so_userTime. dwLowDateTime;
    uo_li. u. HighPart = so_userTime. dwHighDateTime;
    pso_times-> o_UserTime = uo_li. QuadPart / 10ll;
    uo_li. u. LowPart = so_kernelTime. dwLowDateTime;
    uo_li. u. HighPart = so_kernelTime. dwHighDateTime;
    pso_times-> o_KernelTime = uo_li. QuadPart / 10ll;
    return true;
    }

  pso_times-> o_UserTime = 0;
  pso_times-> o_KernelTime = 0;
  return false;
  }

//===========================================================================

#else // POSIX conform

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryUTCTime ()
  {
  struct timeval so_timeVal;
  gettimeofday (& so_timeVal, 0);
  return so_timeVal. tv_sec * co_SecondFactor + so_timeVal. tv_usec;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_QueryLocalTime ()
  {
  struct timeval so_timeVal;
  gettimeofday (& so_timeVal, 0);
  struct tm so_tm;
  tzset ();
  localtime_r (& so_timeVal. tv_sec, & so_tm);
  return timegm (& so_tm) * co_SecondFactor + so_timeVal. tv_usec;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_UTCToLocalTime
(
t_MicroTime i_time
)
  {
  time_t o_seconds = i_time / co_SecondFactor;
  struct tm so_tm;
  tzset ();
  localtime_r (& o_seconds, & so_tm);
  return timegm (& so_tm) * co_SecondFactor + i_time % co_SecondFactor;
  }

//---------------------------------------------------------------------------

t_MicroTime TL_EXPORT tl_LocalToUTCTime
(
t_MicroTime i_time
)
  {
  time_t o_seconds = i_time / co_SecondFactor;
  struct tm so_tm;
  gmtime_r (& o_seconds, & so_tm);
  return mktime (& so_tm) * co_SecondFactor + i_time % co_SecondFactor;
  }

//---------------------------------------------------------------------------

#include <sys/resource.h>

bool TL_EXPORT tl_QueryProcessTimes
(
st_UserKernelTime * pso_times
)
  {
  if (pso_times == 0)
    return false;

  struct rusage so_rusage;

  if (getrusage (RUSAGE_SELF, & so_rusage) == 0)
    {
    pso_times-> o_UserTime = so_rusage. ru_utime. tv_sec * co_SecondFactor +
      so_rusage. ru_utime. tv_usec;
    pso_times-> o_KernelTime = so_rusage. ru_stime. tv_sec * co_SecondFactor +
      so_rusage. ru_stime. tv_usec;
    return true;
    }

  pso_times-> o_UserTime = 0;
  pso_times-> o_KernelTime = 0;
  return false;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryThreadTimes
(
st_UserKernelTime * pso_times
)
  {
  if (pso_times == 0)
    return false;

  struct rusage so_rusage;

  if (getrusage (RUSAGE_THREAD, & so_rusage) == 0)
    {
    pso_times-> o_UserTime = so_rusage. ru_utime. tv_sec * co_SecondFactor +
      so_rusage. ru_utime. tv_usec;
    pso_times-> o_KernelTime = so_rusage. ru_stime. tv_sec * co_SecondFactor +
      so_rusage. ru_stime. tv_usec;
    return true;
    }

  pso_times-> o_UserTime = 0;
  pso_times-> o_KernelTime = 0;
  return false;
  }

#endif
