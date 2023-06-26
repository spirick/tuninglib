
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

static volatile t_Int32 i_Count;

void SetCount
(
t_Int32 i
)
  {
  tl_InterlockedWrite (& i_Count, i);
  }

void IncCount ()
  {
  tl_InterlockedIncrement (& i_Count);
  }

//---------------------------------------------------------------------------

void WaitForCount
(
t_Int32 i
)
  {
  for (;;)
    {
    if (tl_InterlockedRead (& i_Count) == i)
      return;

    tl_Delay (1);
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
  int i_num = (int) (size_t) pv_param;
  SetCount (1);
  printf ("Thread %d ThAcquire ()\n", i_num);
  ThAcquire ();
  printf ("Thread %d Success\n", i_num);
  printf ("Thread %d ThRelease ()\n", i_num);
  ThRelease ();
  printf ("Thread %d End\n", i_num);
  SetCount (2);
  }

//---------------------------------------------------------------------------

void ThTryAcquireNum
(
int i_num
)
  {
  printf ("Thread %d ThTryAcquire ()\n", i_num);

  if (ThTryAcquire ())
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d ThRelease ()\n", i_num);
    ThRelease ();
    }
  else
    printf ("Thread %d No success\n", i_num);
  }

void ThreadMain2
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  ThTryAcquireNum (i_num);
  SetCount (1);
  WaitForCount (2);
  ThTryAcquireNum (i_num);
  printf ("Thread %d End\n", i_num);
  SetCount (3);
  }

//---------------------------------------------------------------------------

void ThTryAcquireNum
(
int i_num,
t_UInt32 u_milliSec
)
  {
  printf ("Thread %d ThTryAcquire (%d)\n", i_num, u_milliSec);

  if (ThTryAcquire (u_milliSec))
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d ThRelease ()\n", i_num);
    ThRelease ();
    }
  else
    printf ("Thread %d No success\n", i_num);
  }

void ThreadMain3
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  ThTryAcquireNum (i_num, 100);
  SetCount (1);
  WaitForCount (2);
  ThTryAcquireNum (i_num, 100);
  SetCount (3);
  ThTryAcquireNum (i_num, 100);
  printf ("Thread %d End\n", i_num);
  SetCount (4);
  }

//---------------------------------------------------------------------------

void ThreadMain4
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  SetCount (1);
  printf ("Thread %d PrAcquire ()\n", i_num);
  PrAcquire ();
  printf ("Thread %d Success\n", i_num);
  printf ("Thread %d PrRelease ()\n", i_num);
  PrRelease ();
  printf ("Thread %d End\n", i_num);
  SetCount (2);
  }

//---------------------------------------------------------------------------

void PrTryAcquireNum
(
int i_num
)
  {
  printf ("Thread %d PrTryAcquire ()\n", i_num);

  if (PrTryAcquire ())
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d PrRelease ()\n", i_num);
    PrRelease ();
    }
  else
    printf ("Thread %d No success\n", i_num);
  }

void ThreadMain5
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  PrTryAcquireNum (i_num);
  SetCount (1);
  WaitForCount (2);
  PrTryAcquireNum (i_num);
  printf ("Thread %d End\n", i_num);
  SetCount (3);
  }

//---------------------------------------------------------------------------

void PrTryAcquireNum
(
int i_num,
t_UInt32 u_milliSec
)
  {
  printf ("Thread %d PrTryAcquire (%d)\n", i_num, u_milliSec);

  if (PrTryAcquire (u_milliSec))
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d PrRelease ()\n", i_num);
    PrRelease ();
    }
  else
    printf ("Thread %d No success\n", i_num);
  }

void ThreadMain6
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  PrTryAcquireNum (i_num, 100);
  SetCount (1);
  WaitForCount (2);
  PrTryAcquireNum (i_num, 100);
  SetCount (3);
  PrTryAcquireNum (i_num, 100);
  printf ("Thread %d End\n", i_num);
  SetCount (4);
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
  int i_num = (int) (size_t) pv_param;

  for (;;)
    {
    ThAcquire ();
    tl_EnterCriticalSection ();
    int i_msg = * co_Messages. GetFirstObj ();
    co_Messages. DelFirstObj ();
    tl_LeaveCriticalSection ();
    printf ("Thread %d received Th message %d\n", i_num, i_msg);
    IncCount ();

    if (i_msg == -1)
      {
      printf ("Thread %d End\n", i_num);
      IncCount ();
      return;
      }
    }
  }

//---------------------------------------------------------------------------

void ThreadMain8
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;

  for (;;)
    {
    PrAcquire ();
    tl_EnterCriticalPrSection ();
    int i_msg = * co_Messages. GetFirstObj ();
    co_Messages. DelFirstObj ();
    tl_LeaveCriticalPrSection ();
    printf ("Thread %d received Pr message %d\n", i_num, i_msg);
    IncCount ();

    if (i_msg == -1)
      {
      printf ("Thread %d End\n", i_num);
      IncCount ();
      return;
      }
    }
  }

//---------------------------------------------------------------------------

int TSemaphore ()
  {
  // Mutex like synchronization
  SetCount (0);
  printf ("Main ThAcquire ()\n");
  ThAcquire ();
  printf ("Main Success\n");
  printf ("Begin Thread 1\n");
  if (! tl_BeginThread (ThreadMain1, (void *) 1)) return 1;
  WaitForCount (1);
  printf ("Main ThRelease ()\n");
  ThRelease ();
  WaitForCount (2);
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 2\n");
  if (! tl_BeginThread (ThreadMain2, (void *) 2)) return 1;
  WaitForCount (1);
  printf ("Main ThAcquire ()\n");
  ThAcquire ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  printf ("Main ThRelease ()\n");
  ThRelease ();
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 3\n");
  if (! tl_BeginThread (ThreadMain3, (void *) 3)) return 1;
  WaitForCount (1);
  printf ("Main ThAcquire ()\n");
  ThAcquire ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  tl_Delay (10);
  printf ("Main ThRelease ()\n");
  ThRelease ();
  WaitForCount (4);
  printf ("\n");

  // Change semaphore count from 1 to 0
  ThAcquire ();

  // Semaphore like synchronization
  printf ("Main sends Th messages 1, 2\n");
  SendThMessage (1);
  SendThMessage (2);
  SetCount (0);
  printf ("Begin Thread 4\n");
  if (! tl_BeginThread (ThreadMain7, (void *) 4)) return 1;
  WaitForCount (2);
  printf ("Main sends Th messages 3, 4\n");
  SendThMessage (3);
  SendThMessage (4);
  SendThMessage (-1);
  WaitForCount (6);
  printf ("\n");

  // Mutex like synchronization
  SetCount (0);
  printf ("Main PrAcquire ()\n");
  PrAcquire ();
  printf ("Main Success\n");
  printf ("Begin Thread 5\n");
  if (! tl_BeginThread (ThreadMain4, (void *) 5)) return 1;
  WaitForCount (1);
  printf ("Main PrRelease ()\n");
  PrRelease ();
  WaitForCount (2);
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 6\n");
  if (! tl_BeginThread (ThreadMain5, (void *) 6)) return 1;
  WaitForCount (1);
  printf ("Main PrAcquire ()\n");
  PrAcquire ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  printf ("Main PrRelease ()\n");
  PrRelease ();
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 7\n");
  if (! tl_BeginThread (ThreadMain6, (void *) 7)) return 1;
  WaitForCount (1);
  printf ("Main PrAcquire ()\n");
  PrAcquire ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  tl_Delay (10);
  printf ("Main PrRelease ()\n");
  PrRelease ();
  WaitForCount (4);
  printf ("\n");

  // Change semaphore count from 1 to 0
  PrAcquire ();

  // Semaphore like synchronization
  printf ("Main sends Pr messages 1, 2\n");
  SendPrMessage (1);
  SendPrMessage (2);
  SetCount (0);
  printf ("Begin Thread 8\n");
  if (! tl_BeginThread (ThreadMain8, (void *) 8)) return 1;
  WaitForCount (2);
  printf ("Main sends Pr messages 3, 4\n");
  SendPrMessage (3);
  SendPrMessage (4);
  SendPrMessage (-1);
  WaitForCount (6);
  printf ("\n");

  return 0;
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

  int i_ret = TSemaphore ();

  delete PCO_ThSemaphore;
  delete PCO_PrSemaphore;
  printf ("End main\n");
  return i_ret;
  }

#endif
