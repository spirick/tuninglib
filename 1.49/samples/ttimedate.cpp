
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
// File samples/ttimedate.cpp

#include <stdio.h>
#include "tuning/timedate.hpp"
#include "tuning/md5.hpp"
#include "tuning/uuid.hpp"
#include "tuning/sys/ctimedate.hpp"
#include "tuning/sys/cprocess.hpp"

//---------------------------------------------------------------------------

const char ac_TestData [] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//---------------------------------------------------------------------------

t_UInt CalcSomething ()
  {
  t_UInt u_sum = 0;
  t_UInt u_len = tl_StringLength (ac_TestData);

  for (unsigned u = 0; u < 1000000; u ++)
    {
    ct_MD5 co_md5 (ac_TestData, u_len);
    ct_UUID co_uuid (co_md5. GetResult ());
    u_sum += co_uuid. GetHash ();
    }

  return u_sum;
  }

//---------------------------------------------------------------------------

void ThreadMain1
(
void * /*pv_param*/
)
  {
  CalcSomething ();
  }

//---------------------------------------------------------------------------

int main ()
  {
  t_MicroTime o_UTCTime = tl_QueryUTCTime ();
  t_MicroTime o_localTime = tl_QueryLocalTime ();
  printf ("UTC      time: %" TL_FMT_INT64 "d\n", o_UTCTime);
  printf ("To UTC   time: %" TL_FMT_INT64 "d\n", tl_LocalToUTCTime (o_localTime));
  printf ("Local    time: %" TL_FMT_INT64 "d\n", o_localTime);
  printf ("To Local time: %" TL_FMT_INT64 "d\n", tl_UTCToLocalTime (o_UTCTime));
  printf ("Diff     time: %" TL_FMT_INT64 "d\n", o_localTime - o_UTCTime);

  ct_TimeDate co_localTime, co_precTime;

  for (int i = 0; i < 20; i ++)
    {
    co_localTime. QueryLocalTime ();
    co_precTime. SetTime (tl_QueryPrecisionTime ());
    printf ("Local time: %02d-%02d-%04d (%d) %02d:%02d:%02d.%06d   ",
      co_localTime. GetMonth (),
      co_localTime. GetDay (),
      co_localTime. GetYear (),
      co_localTime. GetDayOfWeek (),
      co_localTime. GetHour (),
      co_localTime. GetMinute (),
      co_localTime. GetSecond (),
      co_localTime. GetMicroSecond ());
    printf ("Precision time: %02d:%02d:%02d.%06d\n",
      co_precTime. GetHour (),
      co_precTime. GetMinute (),
      co_precTime. GetSecond (),
      co_precTime. GetMicroSecond ());
    }

  t_MicroTime o_testTime = 1234567890123456ll;
  co_localTime. SetTime (o_testTime);

  if (co_localTime. GetTime () != o_testTime)
    {
    printf ("* * * Set/GetTime mismatch\n");
    return 1;
    }

  if (! tl_BeginThread (ThreadMain1, (void *) 1))
    {
    printf ("* * * BeginThread failed\n");
    return 1;
    }

  CalcSomething ();
  tl_Delay (200);

  st_UserKernelTime so_processTimes;
  st_UserKernelTime so_threadTimes;

  if (! tl_QueryProcessTimes (& so_processTimes))
    {
    printf ("* * * QueryProcessTimes failed\n");
    return 1;
    }

  if (! tl_QueryThreadTimes (& so_threadTimes))
    {
    printf ("* * * QueryThreadTimes failed\n");
    return 1;
    }

  printf ("Process user   time: %" TL_FMT_INT64 "d\n", so_processTimes. o_UserTime);
  printf ("Process kernel time: %" TL_FMT_INT64 "d\n", so_processTimes. o_KernelTime);
  printf ("Thread  user   time: %" TL_FMT_INT64 "d\n", so_threadTimes. o_UserTime);
  printf ("Thread  kernel time: %" TL_FMT_INT64 "d\n", so_threadTimes. o_KernelTime);

  return 0;
  }
