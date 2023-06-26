
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
// File tuning/sys/cinfo.hpp

#ifndef TUNING_SYS_CINFO_HPP
#define TUNING_SYS_CINFO_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------
// File system information

struct st_FileSystemInfo
  {
  t_UInt64             u_TotalBytes;
  t_UInt64             u_FreeBytes;
  t_UInt64             u_AvailableBytes;
  };

bool TL_EXPORT       tl_QueryFileSystemInfo (const char * pc_path, st_FileSystemInfo * pso_info);

//---------------------------------------------------------------------------
// Hardware information

struct st_HardwareInfo
  {
  t_UInt64             u_TotalBytes;
  t_UInt64             u_AvailableBytes;
  unsigned             u_TotalProcessors;
  unsigned             u_AvailableProcessors;
  const char *         pc_CPUName;
  };

bool TL_EXPORT       tl_QueryHardwareInfo (st_HardwareInfo * pso_info);

//---------------------------------------------------------------------------
// Process memory information

struct st_ProcessMemoryInfo
  {
  t_UInt               u_VMBytes;
  t_UInt               u_RSSBytes;
  };

bool TL_EXPORT       tl_QueryProcessMemoryInfo (st_ProcessMemoryInfo * pso_info);

//---------------------------------------------------------------------------
// Compiler information

enum et_Compiler
  {
  ec_CompilerMSVC,
  ec_CompilerGCC
  };

struct st_CompilerInfo
  {
  et_Compiler          eo_Compiler;
  const char *         pc_CompilerVersion;
  const char *         pc_RuntimeVersion;
  };

bool TL_EXPORT       tl_QueryCompilerInfo (st_CompilerInfo * pso_info);

//---------------------------------------------------------------------------
// Operating system information

enum et_System
  {
  ec_SystemMSWindows,
  ec_SystemLinux
  };

struct st_SystemInfo
  {
  et_System            eo_System;
  const char *         pc_SystemVersion;
  const char *         pc_ComputerName;
  const char *         pc_UserName;
  };

bool TL_EXPORT       tl_QuerySystemInfo (st_SystemInfo * pso_info);

//---------------------------------------------------------------------------
// Battery information

struct st_BatteryInfo
  {
  bool                 b_ACLine;
  bool                 b_BatteryFound;
  int                  i_LifePercent;
  };

bool TL_EXPORT       tl_QueryBatteryInfo (st_BatteryInfo * pso_info);

#endif
