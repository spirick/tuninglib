
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
// File samples/tinterlocked.cpp

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

volatile t_Int32 I1;
volatile t_Int32 I2;
volatile t_Int32 I3;
const unsigned cu_TestNum = 1000000;

#define INTERLOCKEDINCREMENT(x) tl_InterlockedIncrement (x)
//#define INTERLOCKEDINCREMENT(x) tl_InterlockedAdd (x, 1)

#define INTERLOCKEDDECREMENT(x) tl_InterlockedDecrement (x)
//#define INTERLOCKEDDECREMENT(x) tl_InterlockedAdd (x, -1)

//---------------------------------------------------------------------------

void ThreadMain1
(
void * pv_param
)
  {
  for (unsigned u = 0; u < cu_TestNum; u ++)
    {
    I1 ++;
    INTERLOCKEDINCREMENT (& I2);
    }

  printf ("End loop %d\n", (int) (t_Int) pv_param);
  INTERLOCKEDINCREMENT (& I3);
  }

//---------------------------------------------------------------------------

void ThreadMain2
(
void * pv_param
)
  {
  for (unsigned u = 0; u < cu_TestNum; u ++)
    {
    I1 --;
    INTERLOCKEDDECREMENT (& I2);
    }

  printf ("End loop %d\n", (int) (t_Int) pv_param);
  INTERLOCKEDINCREMENT (& I3);
  }

//---------------------------------------------------------------------------

int main ()
  {
  printf ("Begin test ...\n");

  if (tl_InterlockedIncrement (& I2) != 1)
    printf ("* * * InterlockedIncrement result error\n");

  if (tl_InterlockedWrite (& I2, 2) != 1)
    printf ("* * * InterlockedWrite result error\n");

  if (tl_InterlockedRead (& I2) != 2)
    printf ("* * * InterlockedRead result error\n");

  if (tl_InterlockedAdd (& I2, -1) != 1)
    printf ("* * * InterlockedAdd result error\n");

  if (tl_InterlockedDecrement (& I2) != 0)
    printf ("* * * InterlockedDecrement result error\n");

  tl_BeginThread (ThreadMain1, (void *) 1);
  tl_BeginThread (ThreadMain2, (void *) 2);
  tl_BeginThread (ThreadMain1, (void *) 3);
  tl_BeginThread (ThreadMain2, (void *) 4);
  tl_BeginThread (ThreadMain1, (void *) 5);

  while (tl_InterlockedRead (& I3) < 5)
    tl_Delay (10);

  printf ("NonLocked: %d\nIntLocked: %d\n", I1, I2);
  return 0;
  }

#endif
