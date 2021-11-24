
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
// File samples/tstore.cpp

#include <stdio.h>
#include "tuning/sys/ctimedate.hpp"
#include "tuning/sys/cinfo.hpp"

//---------------------------------------------------------------------------

#if 0

  #include "tuning/std/store.hpp"
  #define GetStore GetStdStore

#elif 0

  #include "tuning/rnd/store.hpp"
  #define GetStore GetRndStore

#else

  #include "tuning/chn/store.hpp"
  #define GetStore GetChnStore

#endif

//---------------------------------------------------------------------------

void Print64
(
t_UInt64 u
)
  {
  printf ("%4u.", (unsigned) (u / 1000000000u));
  u %= 1000000000u;
  printf ("%03u.", (unsigned) (u / 1000000u));
  u %= 1000000u;
  printf ("%03u.", (unsigned) (u / 1000u));
  u %= 1000u;
  printf ("%03u\n", (unsigned) u);
  }

//---------------------------------------------------------------------------

void HeapInfo ()
  {
  st_HeapInfo so_info;
  printf ("\n");

  if (tl_QueryHeapInfo (& so_info))
    {
    printf ("Heap info\n");
    printf ("  AllocEntries");
    Print64 (so_info. u_AllocEntries);
    printf ("  FreeEntries ");
    Print64 (so_info. u_FreeEntries);
    printf ("  AllocSize   ");
    Print64 (so_info. u_AllocSize);
    printf ("  FreeSize    ");
    Print64 (so_info. u_FreeSize);
    printf ("  HeapSize    ");
    Print64 (so_info. u_HeapSize);
    }
  else
    printf ("Heap error\n");

  printf ("\n");
  st_ProcessMemoryInfo so_pmInfo;

  if (tl_QueryProcessMemoryInfo (& so_pmInfo))
    {
    printf ("QueryProcessMemoryInfo: OK\n");
    printf ("  VM  bytes   ");
    Print64 (so_pmInfo. u_VMBytes);
    printf ("  RSS bytes   ");
    Print64 (so_pmInfo. u_RSSBytes);
    }
  else
    printf ("QueryProcessMemoryInfo: failed\n");

  printf ("\n");
  }

//---------------------------------------------------------------------------

const int i_TestSize = 100000;

int main ()
  {
  printf ("%16" TL_FMT_UINT "x\n", GetStore ()-> MaxAlloc ());

  //#ifdef TUNING_CHN_STORE_HPP
  //  GetStore ()-> SetMaxChainExp (10);
  //#endif

  int i, j;
  void * pv;
  static void * apv [i_TestSize];

  HeapInfo ();
  printf ("Start\n");
  t_MicroTime i_time = tl_QueryPrecisionTime ();

  for (i = 0; i < i_TestSize; i ++)
    apv [i] = GetStore ()-> Alloc ((i * 13) % 4001 + 10);

  for (j = 0; j < 10; j ++)
    {
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Realloc (apv [i], (i * 31) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Realloc (apv [i], (i * 71) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Realloc (apv [i], (i * 41) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Realloc (apv [i], (i * 83) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Realloc (apv [i], (i * 61) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      GetStore ()-> Free (apv [i]);
    for (i = 0; i < i_TestSize; i ++)
      apv [i] = GetStore ()-> Alloc ((i * 13) % 4001 + 10);
    for (i = 0; i < i_TestSize; i ++)
      {
      GetStore ()-> Free (apv [i]);
      apv [i] = GetStore ()-> Alloc ((i * 19) % 4001 + 10);
      }
    for (i = 0; i < i_TestSize; i ++)
      {
      pv = apv [i];
      apv [i] = GetStore ()-> Alloc ((i * 49) % 4001 + 10);
      GetStore ()-> Free (pv);
      }
    for (i = 0; i < (i_TestSize / 2); i ++)
      GetStore ()-> Free (apv [i]);
    for (i = 0; i < (i_TestSize / 2); i ++)
      apv [i] = GetStore ()-> Alloc ((i * 31) % 4001 + 10);
    }

  printf ("Ready %d ms\n", (int) ((tl_QueryPrecisionTime () - i_time) / co_MilliSecondFactor));
  HeapInfo ();

  printf ("Free used\n");
  for (i = 0; i < i_TestSize; i ++)
    GetStore ()-> Free (apv [i]);
  HeapInfo ();

  printf ("Free unused\n");

  #ifdef TUNING_CHN_STORE_HPP
    GetStore ()-> FreeUnused ();
  #else
    tl_FreeUnused ();
  #endif

  HeapInfo ();
  return 0;
  }
