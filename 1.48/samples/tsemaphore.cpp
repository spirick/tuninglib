
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
// File samples/tsemaphore.cpp

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

#include "tuning/std/array.h"
#include "tuning/sys/cprocess.hpp"
#include "tuning/sys/ctimedate.hpp"

static ct_ThSemaphore * PCO_ThSemaphore;
static ct_PrSemaphore * PCO_PrSemaphore;

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

bool ThTryAcquire ()
  {
  bool b_success;

  if (PCO_ThSemaphore-> TryAcquire (b_success) != ec_ResOK)
    {
    printf ("TryAcquire Th semaphore failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

bool ThTryAcquire
(
t_UInt32 u_milliSec
)
  {
  bool b_success;

  if (PCO_ThSemaphore-> TryAcquire (b_success, u_milliSec) != ec_ResOK)
    {
    printf ("TryAcquire Th semaphore failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

void ThAcquire ()
  {
  if (PCO_ThSemaphore-> Acquire () != ec_ResOK)
    {
    printf ("Acquire Th semaphore failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void ThRelease ()
  {
  if (PCO_ThSemaphore-> Release () != ec_ResOK)
    {
    printf ("Release Th semaphore failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

bool PrTryAcquire ()
  {
  bool b_success;

  if (PCO_PrSemaphore-> TryAcquire (b_success) != ec_ResOK)
    {
    printf ("TryAcquire Pr semaphore failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

bool PrTryAcquire
(
t_UInt32 u_milliSec
)
  {
  bool b_success;

  if (PCO_PrSemaphore-> TryAcquire (b_success, u_milliSec) != ec_ResOK)
    {
    printf ("TryAcquire Pr semaphore failed\n");
    tl_EndProcess (1);
    }

  return b_success;
  }

//---------------------------------------------------------------------------

void PrAcquire ()
  {
  if (PCO_PrSemaphore-> Acquire () != ec_ResOK)
    {
    printf ("Acquire Pr semaphore failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void PrRelease ()
  {
  if (PCO_PrSemaphore-> Release () != ec_ResOK)
    {
    printf ("Release Pr semaphore failed\n");
    tl_EndProcess (1);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain1
(
void * pv_param
)
  {
  for (int i = 0; i < 10; i ++)
    {
    ThAcquire ();
    printf ("Thread %d in critical Th section (%d)\n", (int) (size_t) pv_param, i);
    ThRelease ();
    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain2
(
void * pv_param
)
  {
  for (int i = 0; i < 15; i ++)
    {
    if (ThTryAcquire ())
      {
      printf ("Thread %d in critical Th section (%d)\n", (int) (size_t) pv_param, i);
      ThRelease ();
      }
    else
      printf ("Thread %d NOT in critical Th section (%d)\n", (int) (size_t) pv_param, i);

    BusyWait (100);
    }
  }

//---------------------------------------------------------------------------

void ThreadMain3
(
void * pv_param
)
  {
  for (int i = 0; i < 12; i ++)
    {
    t_MicroTime i_time = tl_QueryPrecisionTime ();

    if (ThTryAcquire (198))
      {
      printf ("Thread %d in critical Th section (%d)", (int) (size_t) pv_param, i);
      ThRelease ();
      }
    else
      printf ("Thread %d NOT in critical Th section (%d)", (int) (size_t) pv_param, i);

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

void ThreadMain4
(
void * pv_param
)
  {
  for (int i = 0; i < 10; i ++)
    {
    PrAcquire ();
    printf ("Thread %d in critical Pr section (%d)\n", (int) (size_t) pv_param, i);
    PrRelease ();
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
    if (PrTryAcquire ())
      {
      printf ("Thread %d in critical Pr section (%d)\n", (int) (size_t) pv_param, i);
      PrRelease ();
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

    if (PrTryAcquire (198))
      {
      printf ("Thread %d in critical Pr section (%d)", (int) (size_t) pv_param, i);
      PrRelease ();
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

gct_Std_Array <int> co_Messages;

//---------------------------------------------------------------------------

void SendThMessage
(
int i_msg
)
  {
  tl_EnterCriticalSection ();
  co_Messages. AddObj (& i_msg);
  tl_LeaveCriticalSection ();
  ThRelease ();
  }

//---------------------------------------------------------------------------

void SendPrMessage
(
int i_msg
)
  {
  tl_EnterCriticalPrSection ();
  co_Messages. AddObj (& i_msg);
  tl_LeaveCriticalPrSection ();
  PrRelease ();
  }

//---------------------------------------------------------------------------

void ThreadMain7
(
void * pv_param
)
  {
  for (;;)
    {
    ThAcquire ();
    tl_EnterCriticalSection ();
    int i_msg = * co_Messages. GetFirstObj ();
    co_Messages. DelFirstObj ();
    tl_LeaveCriticalSection ();
    printf ("Thread %d received Th message %d\n", (int) (size_t) pv_param, i_msg);

    if (i_msg == -1)
      return;
    }
  }

//---------------------------------------------------------------------------

void ThreadMain8
(
void * pv_param
)
  {
  for (;;)
    {
    PrAcquire ();
    tl_EnterCriticalPrSection ();
    int i_msg = * co_Messages. GetFirstObj ();
    co_Messages. DelFirstObj ();
    tl_LeaveCriticalPrSection ();
    printf ("Thread %d received Pr message %d\n", (int) (size_t) pv_param, i_msg);

    if (i_msg == -1)
      return;
    }
  }

//---------------------------------------------------------------------------

int main ()
  {
  PCO_ThSemaphore = new ct_ThSemaphore (1);

  if (! PCO_ThSemaphore-> GetInitSuccess ())
    {
    printf ("Init Th semaphore failed\n");
    return 1;
    }

  PCO_PrSemaphore = new ct_PrSemaphore;
  PCO_PrSemaphore-> Create (1);

  if (! PCO_PrSemaphore-> GetInitSuccess ())
    {
    printf ("Init Pr semaphore failed\n");
    return 1;
    }

  // Mutex like synchronization
  if (! tl_BeginThread (ThreadMain1, (void *) 1)) return 1;
  BusyWait (450);
  ThAcquire ();
  printf ("Main waites in critical Th section\n");
  BusyWait (500);
  printf ("Main leaves critical Th section\n");
  ThRelease ();
  BusyWait (1000);
  printf ("\n");

  if (! tl_BeginThread (ThreadMain2, (void *) 2)) return 1;
  BusyWait (450);
  ThAcquire ();
  printf ("Main waites in critical Th section\n");
  BusyWait (500);
  printf ("Main leaves critical Th section\n");
  ThRelease ();
  BusyWait (1000);
  printf ("\n");

  tl_Delay (10);
  if (! tl_BeginThread (ThreadMain3, (void *) 3)) return 1;
  BusyWait (450);
  ThAcquire ();
  printf ("Main waites in critical Th section\n");
  BusyWait (500);
  printf ("Main leaves critical Th section\n");
  ThRelease ();
  BusyWait (1000);
  printf ("\n");

  // Change semaphore count from 1 to 0
  ThAcquire ();

  // Semaphore like synchronization
  printf ("Main sends Th messages 1, 2\n");
  SendThMessage (1);
  SendThMessage (2);
  printf ("Main starts thread 4\n");
  if (! tl_BeginThread (ThreadMain7, (void *) 4)) return 1;
  BusyWait (50);
  printf ("Main sends Th messages 3, 4\n");
  SendThMessage (3);
  SendThMessage (4);
  SendThMessage (-1);
  BusyWait (50);
  printf ("\n");

  // Mutex like synchronization
  if (! tl_BeginThread (ThreadMain4, (void *) 5)) return 1;
  BusyWait (450);
  PrAcquire ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  PrRelease ();
  BusyWait (1000);
  printf ("\n");

  if (! tl_BeginThread (ThreadMain5, (void *) 6)) return 1;
  BusyWait (450);
  PrAcquire ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  PrRelease ();
  BusyWait (1000);
  printf ("\n");

  tl_Delay (10);
  if (! tl_BeginThread (ThreadMain6, (void *) 7)) return 1;
  BusyWait (450);
  PrAcquire ();
  printf ("Main waites in critical Pr section\n");
  BusyWait (500);
  printf ("Main leaves critical Pr section\n");
  PrRelease ();
  BusyWait (1000);
  printf ("\n");

  // Change semaphore count from 1 to 0
  PrAcquire ();

  // Semaphore like synchronization
  printf ("Main sends Pr messages 1, 2\n");
  SendPrMessage (1);
  SendPrMessage (2);
  printf ("Main starts thread 8\n");
  if (! tl_BeginThread (ThreadMain8, (void *) 8)) return 1;
  BusyWait (50);
  printf ("Main sends Pr messages 3, 4\n");
  SendPrMessage (3);
  SendPrMessage (4);
  SendPrMessage (-1);
  BusyWait (50);
  printf ("\n");

  delete PCO_ThSemaphore;
  delete PCO_PrSemaphore;
  printf ("End main\n");
  return 0;
  }

#endif
