
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
// File samples/tthread.cpp

#include <stdio.h>
#include "tuning/defs.hpp"

#if !defined TL_MULTI

//---------------------------------------------------------------------------

int main ()
  {
  printf ("Multi threading is disabled\n");
  return 0;
  }

#else

#include "tuning/sys/cprocess.hpp"
#include "tuning/sys/ctimedate.hpp"
#include "samples/int.hpp"

#define MY_MUTEX

#ifdef MY_MUTEX

static ct_ThMutex * PCO_ThMutex;
// Don't use char buffer because of alignment
static t_UInt AU_ThMutexBuffer [(sizeof (ct_ThMutex) / sizeof (t_UInt)) + 1];

static ct_PrMutex * PCO_PrMutex;
// Don't use char buffer because of alignment
static t_UInt AU_PrMutexBuffer [(sizeof (ct_PrMutex) / sizeof (t_UInt)) + 1];

#endif

//---------------------------------------------------------------------------

int Round10
(
int i
)
  {
  return ((i + 5) / 10) * 10;
  }

//---------------------------------------------------------------------------
// tl_Delay is too inaccurate

void BusyWait
(
int i_milliSec
)
  {
  t_MicroTime i_time = tl_QueryPrecisionTime ();
  int i_diff = 0;

  while (i_diff < i_milliSec)
    {
    tl_RelinquishTimeSlice ();
    i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);
    }
  }

//---------------------------------------------------------------------------

bool TryEnterCriticalThSection ()
  {
  bool b_success;

  #ifdef MY_MUTEX
  if (PCO_ThMutex-> TryLock (b_success) != ec_ResOK)
  #else
  if (tl_TryEnterCriticalSection (b_success) != ec_ResOK)
  #endif
    {
    printf ("TryLock Th mutex failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

void EnterCriticalThSection ()
  {
  #ifdef MY_MUTEX
  if (PCO_ThMutex-> Lock () != ec_ResOK)
  #else
  if (tl_EnterCriticalSection () != ec_ResOK)
  #endif
    {
    printf ("Lock Th mutex failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void LeaveCriticalThSection ()
  {
  #ifdef MY_MUTEX
  if (PCO_ThMutex-> Unlock () != ec_ResOK)
  #else
  if (tl_LeaveCriticalSection () != ec_ResOK)
  #endif
    {
    printf ("Unlock Th mutex failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

bool TryEnterCriticalPrSection ()
  {
  bool b_success;

  #ifdef MY_MUTEX
  if (PCO_PrMutex-> TryLock (b_success) != ec_ResOK)
  #else
  if (tl_TryEnterCriticalIPSection (b_success) != ec_ResOK)
  #endif
    {
    printf ("TryLock Pr mutex failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

bool TryEnterCriticalPrSection
(
t_UInt32 u_milliSec
)
  {
  bool b_success;

  #ifdef MY_MUTEX
  if (PCO_PrMutex-> TryLock (b_success, u_milliSec) != ec_ResOK)
  #else
  if (tl_TryEnterCriticalIPSection (b_success, u_milliSec) != ec_ResOK)
  #endif
    {
    printf ("TryLock Pr mutex failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

void EnterCriticalPrSection ()
  {
  #ifdef MY_MUTEX
  if (PCO_PrMutex-> Lock () != ec_ResOK)
  #else
  if (tl_EnterCriticalIPSection () != ec_ResOK)
  #endif
    {
    printf ("Lock Pr mutex failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void LeaveCriticalPrSection ()
  {
  #ifdef MY_MUTEX
  if (PCO_PrMutex-> Unlock () != ec_ResOK)
  #else
  if (tl_LeaveCriticalIPSection () != ec_ResOK)
  #endif
    {
    printf ("Unlock Pr mutex failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain1
(
void * pv_param
)
  {
  // printf ("Begin thread %d %" TL_FMT_UINT64 "d\n", (int) (size_t) pv_param, tl_ThreadId ());
  printf ("Begin thread %d\n", (int) (size_t) pv_param);
  BusyWait (333);
  printf ("Pause thread %d\n", (int) (size_t) pv_param);
  BusyWait (333);
  printf ("End   thread %d\n", (int) (size_t) pv_param);
  }

//---------------------------------------------------------------------------

void ThreadMain2
(
void * pv_param
)
  {
  for (int i = 0; i < 10; i ++)
    {
    EnterCriticalThSection ();
    printf ("Thread %d in critical Th section (%d)\n", (int) (size_t) pv_param, i);
    LeaveCriticalThSection ();
    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain3
(
void * pv_param
)
  {
  for (int i = 0; i < 15; i ++)
    {
    if (TryEnterCriticalThSection ())
      {
      printf ("Thread %d in critical Th section (%d)\n", (int) (size_t) pv_param, i);
      LeaveCriticalThSection ();
      }
    else
      printf ("Thread %d NOT in critical Th section (%d)\n", (int) (size_t) pv_param, i);

    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain4
(
void * pv_param
)
  {
  for (int i = 0; i < 10; i ++)
    {
    EnterCriticalPrSection ();
    printf ("Thread %d in critical Pr section (%d)\n", (int) (size_t) pv_param, i);
    LeaveCriticalPrSection ();
    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain5
(
void * pv_param
)
  {
  for (int i = 0; i < 15; i ++)
    {
    if (TryEnterCriticalPrSection ())
      {
      printf ("Thread %d in critical Pr section (%d)\n", (int) (size_t) pv_param, i);
      LeaveCriticalPrSection ();
      }
    else
      printf ("Thread %d NOT in critical Pr section (%d)\n", (int) (size_t) pv_param, i);

    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain6
(
void * pv_param
)
  {
  for (int i = 0; i < 12; i ++)
    {
    t_MicroTime i_time = tl_QueryPrecisionTime ();

    if (TryEnterCriticalPrSection (198))
      {
      printf ("Thread %d in critical Pr section (%d)", (int) (size_t) pv_param, i);
      LeaveCriticalPrSection ();
      }
    else
      printf ("Thread %d NOT in critical Pr section (%d)", (int) (size_t) pv_param, i);

    int i_diff = (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor);

    if (i_diff < 10)
      {
      printf ("\n");
      BusyWait (100);
      }
    else
      printf (" waited %d milliseconds\n", Round10 (i_diff));
    }
  }

//---------------------------------------------------------------------------

void ThreadMain7
(
void *
)
  {
  ct_Int co_local;
  tl_EndThread ();
  }

//---------------------------------------------------------------------------

int main ()
  {
  #ifdef MY_MUTEX

  PCO_ThMutex = new (AU_ThMutexBuffer) ct_ThMutex;

  if (! PCO_ThMutex-> GetInitSuccess ())
    {
    printf ("Init Th mutex failed\n");
    return 1;
    }

  PCO_PrMutex = new (AU_PrMutexBuffer) ct_PrMutex;
  PCO_PrMutex-> Create ();

  if (! PCO_PrMutex-> GetInitSuccess ())
    {
    printf ("Init Pr mutex failed\n");
    return 1;
    }

  #else

  if (! tl_CriticalSectionInitSuccess ())
    {
    printf ("Init Th mutex failed\n");
    return 1;
    }

  if (! tl_CriticalIPSectionInitSuccess ())
    {
    printf ("Init Pr mutex failed\n");
    return 1;
    }

  #endif

  if (! tl_BeginThread (ThreadMain1, (void *) 1)) return 1;
  BusyWait (111);
  if (! tl_BeginThread (ThreadMain1, (void *) 2)) return 1;
  BusyWait (111);
  if (! tl_BeginThread (ThreadMain1, (void *) 3)) return 1;
  BusyWait (1000);
  printf ("\n");

  EnterCriticalThSection ();
  EnterCriticalThSection ();
  printf ("In nested critical Th section\n");
  LeaveCriticalThSection ();
  LeaveCriticalThSection ();
  printf ("\n");

  if (! tl_BeginThread (ThreadMain2, (void *) 4)) return 1;
  BusyWait (450);
  EnterCriticalThSection ();
  printf ("Main waites in critical Th section\n");
  BusyWait (500);
  printf ("Main leaves critical Th section\n");
  LeaveCriticalThSection ();
  BusyWait (1000);
  printf ("\n");

  if (! tl_BeginThread (ThreadMain3, (void *) 5)) return 1;
  BusyWait (450);
  EnterCriticalThSection ();
  printf ("Main waites in critical Th section\n");
  BusyWait (500);
  printf ("Main leaves critical Th section\n");
  LeaveCriticalThSection ();
  BusyWait (1000);
  printf ("\n");

  #if defined TL_WINDOWS

  EnterCriticalPrSection ();
  EnterCriticalPrSection ();
  printf ("In nested critical Pr section\n");
  LeaveCriticalPrSection ();
  LeaveCriticalPrSection ();
  printf ("\n");

  #endif

  if (! tl_BeginThread (ThreadMain4, (void *) 6)) return 1;
  BusyWait (450);
  EnterCriticalPrSection ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  LeaveCriticalPrSection ();
  BusyWait (1000);
  printf ("\n");

  if (! tl_BeginThread (ThreadMain5, (void *) 7)) return 1;
  BusyWait (450);
  EnterCriticalPrSection ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  LeaveCriticalPrSection ();
  BusyWait (1000);
  printf ("\n");

  tl_Delay (10);
  if (! tl_BeginThread (ThreadMain6, (void *) 8)) return 1;
  BusyWait (450);
  EnterCriticalPrSection ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  LeaveCriticalPrSection ();
  BusyWait (1000);
  printf ("\n");

  printf ("Before end test\n");
  if (! tl_BeginThread (ThreadMain7, 0)) return 1;
  BusyWait (100);
  printf ("After end test\n");
  printf ("\n");

  #ifdef MY_MUTEX

  delete PCO_ThMutex;
  delete PCO_PrMutex;

  #else

  tl_DeleteCriticalSection ();
  tl_DeleteCriticalIPSection ();

  #endif

  printf ("End main\n");
  return 0;
  }

#endif
