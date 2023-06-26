
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
  int i_num = (int) (size_t) pv_param;
  printf ("Thread %d Start\n", i_num);
  IncCount ();
  WaitForCount (i_num + 2);
  printf ("Thread %d Mid\n", i_num);
  IncCount ();
  WaitForCount (i_num + 5);
  printf ("Thread %d End\n", i_num);
  IncCount ();
  }

//---------------------------------------------------------------------------

void ThreadMain2
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  SetCount (1);
  printf ("Thread %d EnterCrTh ()\n", i_num);
  EnterCriticalThSection ();
  printf ("Thread %d Success\n", i_num);
  printf ("Thread %d LeaveCrTh ()\n", i_num);
  LeaveCriticalThSection ();
  printf ("Thread %d End\n", i_num);
  SetCount (2);
  }

//---------------------------------------------------------------------------

void TryEnterCriticalThSectionNum
(
int i_num
)
  {
  printf ("Thread %d TryEnterCrTh ()\n", i_num);

  if (TryEnterCriticalThSection ())
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d LeaveCrTh ()\n", i_num);
    LeaveCriticalThSection ();
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
  TryEnterCriticalThSectionNum (i_num);
  SetCount (1);
  WaitForCount (2);
  TryEnterCriticalThSectionNum (i_num);
  printf ("Thread %d End\n", i_num);
  SetCount (3);
  }

//---------------------------------------------------------------------------

void ThreadMain4
(
void * pv_param
)
  {
  int i_num = (int) (size_t) pv_param;
  SetCount (1);
  printf ("Thread %d EnterCrPr ()\n", i_num);
  EnterCriticalPrSection ();
  printf ("Thread %d Success\n", i_num);
  printf ("Thread %d LeaveCrPr ()\n", i_num);
  LeaveCriticalPrSection ();
  printf ("Thread %d End\n", i_num);
  SetCount (2);
  }

//---------------------------------------------------------------------------

void TryEnterCriticalPrSectionNum
(
int i_num
)
  {
  printf ("Thread %d TryEnterCrPr ()\n", i_num);

  if (TryEnterCriticalPrSection ())
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d LeaveCrPr ()\n", i_num);
    LeaveCriticalPrSection ();
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
  TryEnterCriticalPrSectionNum (i_num);
  SetCount (1);
  WaitForCount (2);
  TryEnterCriticalPrSectionNum (i_num);
  printf ("Thread %d End\n", i_num);
  SetCount (3);
  }

//---------------------------------------------------------------------------

void TryEnterCriticalPrSectionNum
(
int i_num,
t_UInt32 u_milliSec
)
  {
  printf ("Thread %d TryEnterCrPr (%d)\n", i_num, u_milliSec);

  if (TryEnterCriticalPrSection (u_milliSec))
    {
    printf ("Thread %d Success\n", i_num);
    printf ("Thread %d LeaveCrPr ()\n", i_num);
    LeaveCriticalPrSection ();
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
  TryEnterCriticalPrSectionNum (i_num, 100);
  SetCount (1);
  WaitForCount (2);
  TryEnterCriticalPrSectionNum (i_num, 100);
  SetCount (3);
  TryEnterCriticalPrSectionNum (i_num, 100);
  printf ("Thread %d End\n", i_num);
  SetCount (4);
  }

//---------------------------------------------------------------------------

void ThreadMain7
(
void *
)
  {
  ct_Int co_local;
  SetCount (1);
  tl_EndThread ();
  }

//---------------------------------------------------------------------------

int TThread ()
  {
  SetCount (0);
  if (! tl_BeginThread (ThreadMain1, (void *) 1)) return 1;
  WaitForCount (1);
  if (! tl_BeginThread (ThreadMain1, (void *) 2)) return 1;
  WaitForCount (2);
  if (! tl_BeginThread (ThreadMain1, (void *) 3)) return 1;
  WaitForCount (9);
  printf ("\n");

  EnterCriticalThSection ();
  EnterCriticalThSection ();
  printf ("In nested critical Th section\n");
  LeaveCriticalThSection ();
  LeaveCriticalThSection ();
  printf ("\n");

  SetCount (0);
  printf ("Main EnterCrTh ()\n");
  EnterCriticalThSection ();
  printf ("Main Success\n");
  printf ("Begin Thread 4\n");
  if (! tl_BeginThread (ThreadMain2, (void *) 4)) return 1;
  WaitForCount (1);
  printf ("Main LeaveCrTh ()\n");
  LeaveCriticalThSection ();
  WaitForCount (2);
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 5\n");
  if (! tl_BeginThread (ThreadMain3, (void *) 5)) return 1;
  WaitForCount (1);
  printf ("Main EnterCrTh ()\n");
  EnterCriticalThSection ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  printf ("Main LeaveCrTh ()\n");
  LeaveCriticalThSection ();
  printf ("\n");

  #if defined TL_WINDOWS

  EnterCriticalPrSection ();
  EnterCriticalPrSection ();
  printf ("In nested critical Pr section\n");
  LeaveCriticalPrSection ();
  LeaveCriticalPrSection ();
  printf ("\n");

  #endif

  SetCount (0);
  printf ("Main EnterCrPr ()\n");
  EnterCriticalPrSection ();
  printf ("Main Success\n");
  printf ("Begin Thread 6\n");
  if (! tl_BeginThread (ThreadMain4, (void *) 6)) return 1;
  WaitForCount (1);
  printf ("Main LeaveCrPr ()\n");
  LeaveCriticalPrSection ();
  WaitForCount (2);
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 7\n");
  if (! tl_BeginThread (ThreadMain5, (void *) 7)) return 1;
  WaitForCount (1);
  printf ("Main EnterCrPr ()\n");
  EnterCriticalPrSection ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  printf ("Main LeaveCrPr ()\n");
  LeaveCriticalPrSection ();
  printf ("\n");

  SetCount (0);
  printf ("Begin Thread 8\n");
  if (! tl_BeginThread (ThreadMain6, (void *) 8)) return 1;
  WaitForCount (1);
  printf ("Main EnterCrPr ()\n");
  EnterCriticalPrSection ();
  printf ("Main Success\n");
  SetCount (2);
  WaitForCount (3);
  tl_Delay (10);
  printf ("Main LeaveCrPr ()\n");
  LeaveCriticalPrSection ();
  WaitForCount (4);
  printf ("\n");

  SetCount (0);
  printf ("Before end test\n");
  if (! tl_BeginThread (ThreadMain7, 0)) return 1;
  WaitForCount (1);
  tl_Delay (10);
  printf ("After end test\n");
  printf ("\n");
  return 0;
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

  int i_ret = TThread ();

  #ifdef MY_MUTEX

  delete PCO_ThMutex;
  delete PCO_PrMutex;

  #else

  tl_DeleteCriticalSection ();
  tl_DeleteCriticalIPSection ();

  #endif

  printf ("End main\n");
  return i_ret;
  }

#endif
