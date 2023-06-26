
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
// File samples/tinfo.cpp

#include <stdio.h>
#include "tuning/sys/cinfo.hpp"
#include "tuning/sys/calloc.hpp"
#include "tuning/sys/cmemory.hpp"

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

void PrintProcessMemoryInfo ()
  {
  printf ("\n");
  st_ProcessMemoryInfo so_pmInfo;

  if (tl_QueryProcessMemoryInfo (& so_pmInfo))
    {
    printf ("QueryProcessMemoryInfo: OK\n");
    printf ("  VM  bytes       : ");
    Print64 (so_pmInfo. u_VMBytes);
    printf ("  RSS bytes       : ");
    Print64 (so_pmInfo. u_RSSBytes);
    }
  else
    printf ("QueryProcessMemoryInfo: failed\n");
  }

//---------------------------------------------------------------------------

int main
(
int /*argc*/,
char * * /*argv*/
)
  {
  printf ("\n");
  st_FileSystemInfo so_fsInfo;

  if (tl_QueryFileSystemInfo ("", & so_fsInfo))
    {
    printf ("QueryFileSystemInfo: OK\n");
    printf ("  Total bytes     : ");
    Print64 (so_fsInfo. u_TotalBytes);
    printf ("  Free bytes      : ");
    Print64 (so_fsInfo. u_FreeBytes);
    printf ("  Available bytes : ");
    Print64 (so_fsInfo. u_AvailableBytes);
    }
  else
    printf ("QueryFileSystemInfo: failed\n");

  printf ("\n");
  st_HardwareInfo so_hwInfo;

  if (tl_QueryHardwareInfo (& so_hwInfo))
    {
    printf ("QueryHardwareInfo: OK\n");
    printf ("  Total bytes     : ");
    Print64 (so_hwInfo. u_TotalBytes);
    printf ("  Available bytes : ");
    Print64 (so_hwInfo. u_AvailableBytes);
    printf ("  Total processors: %d\n", so_hwInfo. u_TotalProcessors);
    printf ("  Avail.processors: %d\n", so_hwInfo. u_AvailableProcessors);
    printf ("  CPU name        : %s\n", so_hwInfo. pc_CPUName);
    }
  else
    printf ("QueryHardwareInfo: failed\n");

  PrintProcessMemoryInfo ();
  char * pc = (char *) tl_Alloc (1000000);
  tl_FillMemory (pc, 1000000, 0);
  PrintProcessMemoryInfo ();

  printf ("\n");
  st_CompilerInfo so_compInfo;

  if (tl_QueryCompilerInfo (& so_compInfo))
    {
    printf ("QueryCompilerInfo: OK\n");
    printf ("  Compiler        : %d\n", so_compInfo. eo_Compiler);
    printf ("  Compiler version: %s\n", so_compInfo. pc_CompilerVersion);
    printf ("  Runtime  version: %s\n", so_compInfo. pc_RuntimeVersion);
    }
  else
    printf ("QueryCompilerInfo: failed\n");

  printf ("\n");
  st_SystemInfo so_sysInfo;

  if (tl_QuerySystemInfo (& so_sysInfo))
    {
    printf ("QuerySystemInfo: OK\n");
    printf ("  System          : %d\n", so_sysInfo. eo_System);
    printf ("  System version  : %s\n", so_sysInfo. pc_SystemVersion);
    printf ("  Computer name   : %s\n", so_sysInfo. pc_ComputerName);
    printf ("  User name       : %s\n", so_sysInfo. pc_UserName);
    }
  else
    printf ("QuerySystemInfo: failed\n");

  printf ("\n");
  st_BatteryInfo so_batInfo;

  if (tl_QueryBatteryInfo (& so_batInfo))
    {
    printf ("QueryBatteryInfo: OK\n");
    printf ("  ACLine          : %d\n", so_batInfo. b_ACLine);
    printf ("  Battery found   : %d\n", so_batInfo. b_BatteryFound);
    printf ("  Life percent    : %d\n", so_batInfo. i_LifePercent);
    }
  else
    printf ("QueryBatteryInfo: failed\n");

  return 0;
  }
