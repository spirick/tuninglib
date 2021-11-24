
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
// File tuning/sys/cinfo.cpp

#include "tuning/sys/cinfo.hpp"
#include "tuning/sys/cprocess.hpp"
#include "tuning/fixblock.h"
#include "tuning/file.hpp"

#define STR(x) #x
#define STRSTR(x) STR (x)

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryFileSystemInfo
(
const char * pc_path,
st_FileSystemInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  ct_FileName co_path;

  if ((pc_path != 0) && (* pc_path != '\0'))
    co_path. AssignAsPath (pc_path);

  if (! co_path. HasPath ())
    co_path. SetPath (pc_SlashStr);

  ULARGE_INTEGER uo_TotalBytes;
  ULARGE_INTEGER uo_FreeBytes;
  ULARGE_INTEGER uo_AvailableBytes;

  if (GetDiskFreeSpaceExA (co_path. GetAllStr (),
      & uo_AvailableBytes, & uo_TotalBytes, & uo_FreeBytes))
    {
    pso_info-> u_TotalBytes = uo_TotalBytes. QuadPart;
    pso_info-> u_FreeBytes = uo_FreeBytes. QuadPart;
    pso_info-> u_AvailableBytes = uo_AvailableBytes. QuadPart;
    return true;
    }

  pso_info-> u_TotalBytes = 0;
  pso_info-> u_FreeBytes = 0;
  pso_info-> u_AvailableBytes = 0;
  return false;
  }

//---------------------------------------------------------------------------

static char ac_CPUName [257];

bool TL_EXPORT tl_QueryHardwareInfo
(
st_HardwareInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  MEMORYSTATUSEX so_stat;
  so_stat. dwLength = sizeof (so_stat);

  if (GlobalMemoryStatusEx (& so_stat) != 0)
    {
    pso_info-> u_TotalBytes = so_stat. ullTotalPhys;
    pso_info-> u_AvailableBytes = so_stat. ullAvailPhys;
    SYSTEM_INFO so_sysinfo;
    GetSystemInfo (& so_sysinfo);
    pso_info-> u_TotalProcessors = so_sysinfo. dwNumberOfProcessors;
    pso_info-> u_AvailableProcessors = so_sysinfo. dwNumberOfProcessors;

    tl_EnterCriticalSection ();

    if (ac_CPUName [0] == '\0')
      {
      HKEY o_cpu0;

      if (RegOpenKeyExA (HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
          0, KEY_READ, & o_cpu0) == ERROR_SUCCESS)
        {
        DWORD o_size = (DWORD) sizeof (ac_CPUName);
        RegQueryValueExA (o_cpu0, "ProcessorNameString", 0, 0, (LPBYTE) ac_CPUName, & o_size);
        RegCloseKey (o_cpu0);
        }

      if (ac_CPUName [0] == '\0')
        ac_CPUName [0] = '?';
      }

    tl_LeaveCriticalSection ();

    pso_info-> pc_CPUName = ac_CPUName;
    return true;
    }

  pso_info-> u_TotalBytes = 0;
  pso_info-> u_AvailableBytes = 0;
  pso_info-> u_TotalProcessors = 0;
  pso_info-> u_AvailableProcessors = 0;
  pso_info-> pc_CPUName = "?";
  return false;
  }

//---------------------------------------------------------------------------

#include <psapi.h>

bool TL_EXPORT tl_QueryProcessMemoryInfo
(
st_ProcessMemoryInfo * pso_info
)
  {
  PROCESS_MEMORY_COUNTERS so_info = { sizeof (PROCESS_MEMORY_COUNTERS) };

  if (GetProcessMemoryInfo (GetCurrentProcess(), & so_info, sizeof (so_info)))
    {
    pso_info-> u_VMBytes = so_info. PagefileUsage;
    pso_info-> u_RSSBytes = so_info. WorkingSetSize;
    return true;
    }

  pso_info-> u_VMBytes = 0;
  pso_info-> u_RSSBytes = 0;
  return false;
  }

//---------------------------------------------------------------------------

#if (_MSC_VER >= 1400) && (_MSC_VER < 1600)
  #include <crtassem.h>
#elif _MSC_VER >= 1600
  #include <crtversion.h>
#endif

#if defined TL_64
  #define COMP_ARCH " x64"
#else
  #define COMP_ARCH " x86"
#endif

bool TL_EXPORT tl_QueryCompilerInfo
(
st_CompilerInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  pso_info-> eo_Compiler = ec_CompilerMSVC;
  pso_info-> pc_CompilerVersion = STRSTR (_MSC_FULL_VER) "."
    STRSTR (_MSC_BUILD) COMP_ARCH;

  #if defined _CRT_ASSEMBLY_VERSION
    pso_info-> pc_RuntimeVersion = _CRT_ASSEMBLY_VERSION;
  #elif defined _VC_CRT_MAJOR_VERSION
    pso_info-> pc_RuntimeVersion = STRSTR (_VC_CRT_MAJOR_VERSION) "."
      STRSTR (_VC_CRT_MINOR_VERSION) "."
      STRSTR (_VC_CRT_BUILD_VERSION) "."
      STRSTR (_VC_CRT_RBUILD_VERSION);
  #else
    pso_info-> pc_RuntimeVersion = "?";
  #endif

  return true;
  }

//---------------------------------------------------------------------------

#include <lmcons.h>

typedef LONG NTSTATUS;
static const unsigned u_OSVersionSize = 64u;
static gct_String <gct_CharBlock <gct_NullDataBlock <gct_FixBlock
  <t_UInt32, u_OSVersionSize + 1>, char>, char>, ct_Chn32Store> co_OSVersion;
static char ac_ComputerName [MAX_COMPUTERNAME_LENGTH + 1];
static char ac_UserName [UNLEN + 1];

bool TL_EXPORT tl_QuerySystemInfo
(
st_SystemInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  tl_EnterCriticalSection ();

  if (co_OSVersion. IsEmpty ())
    {
    co_OSVersion = "?";
    NTSTATUS (WINAPI * pRtlGetVersion) (PRTL_OSVERSIONINFOW);
    * (FARPROC *) & pRtlGetVersion = GetProcAddress (GetModuleHandleA ("ntdll"), "RtlGetVersion");

    if (pRtlGetVersion != 0)
      {
      OSVERSIONINFOW so_vinfo;
      ZeroMemory (& so_vinfo, sizeof (OSVERSIONINFOW));
      so_vinfo. dwOSVersionInfoSize = sizeof (OSVERSIONINFOW);

      if (pRtlGetVersion (& so_vinfo) == 0 /*STATUS_SUCCESS*/)
        {
        co_OSVersion. AssignF ("%d.%d.%d", so_vinfo. dwMajorVersion,
          so_vinfo. dwMinorVersion, so_vinfo. dwBuildNumber);

        if (so_vinfo. szCSDVersion [0] != L'\0')
          {
          co_OSVersion += " ";
          size_t u_bufferSize = sizeof (so_vinfo. szCSDVersion) / sizeof (wchar_t);
          size_t u_pos = 0;

          while ((u_pos < u_bufferSize) &&
                 (co_OSVersion. GetLen () < u_OSVersionSize - 1u))
            {
            unsigned u = so_vinfo. szCSDVersion [u_pos ++];

            if (u == 0)
              break;

            if (u < 128)
              co_OSVersion += (char) u;
            }
          }
        }
      }

    if (co_OSVersion. GetLen () < u_OSVersionSize - 4u)
      {
      SYSTEM_INFO so_sysinfo;
      GetNativeSystemInfo (& so_sysinfo);

      if (so_sysinfo. wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        co_OSVersion += " x64";
      else
        {
        if (so_sysinfo. wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
          co_OSVersion += " x86";
        }
      }

    DWORD u_compSize = MAX_COMPUTERNAME_LENGTH + 1;

    if (GetComputerNameA (ac_ComputerName, & u_compSize) == 0)
      ac_ComputerName [0] = '?';

    DWORD u_userSize = UNLEN + 1;

    if (GetUserNameA (ac_UserName, & u_userSize) == 0)
      ac_UserName [0] = '?';
    }

  tl_LeaveCriticalSection ();

  pso_info-> eo_System = ec_SystemMSWindows;
  pso_info-> pc_SystemVersion = co_OSVersion. GetStr ();
  pso_info-> pc_ComputerName = ac_ComputerName;
  pso_info-> pc_UserName = ac_UserName;
  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryBatteryInfo
(
st_BatteryInfo * pso_info
)
  {
  SYSTEM_POWER_STATUS so_stat;

  if (GetSystemPowerStatus (& so_stat))
    {
    pso_info-> b_ACLine = (so_stat. ACLineStatus == 1);
    pso_info-> b_BatteryFound = (so_stat. BatteryFlag < 128);
    pso_info-> i_LifePercent = so_stat. BatteryLifePercent;
    return true;
    }

  pso_info-> b_ACLine = false;
  pso_info-> b_BatteryFound = false;
  pso_info-> i_LifePercent = 0;
  return false;
  }

//===========================================================================

#elif defined TL_UNIX

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/statvfs.h>

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryFileSystemInfo
(
const char * pc_path,
st_FileSystemInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  ct_FileName co_path;

  if ((pc_path != 0) && (* pc_path != '\0'))
    co_path. AssignAsPath (pc_path);

  if (! co_path. HasPath ())
    co_path. SetPath (pc_SlashStr);

  struct statvfs so_stat;

  if (statvfs (co_path. GetAllStr (), & so_stat) == 0)
    {
    pso_info-> u_TotalBytes = so_stat. f_blocks * (t_UInt64) so_stat. f_frsize;
    pso_info-> u_FreeBytes = so_stat. f_bfree * (t_UInt64) so_stat. f_frsize;
    pso_info-> u_AvailableBytes = so_stat. f_bavail * (t_UInt64) so_stat. f_frsize;
    return true;
    }

  pso_info-> u_TotalBytes = 0;
  pso_info-> u_FreeBytes = 0;
  pso_info-> u_AvailableBytes = 0;
  return false;
  }

//---------------------------------------------------------------------------

static char ac_CPUName [257];

bool TL_EXPORT tl_QueryHardwareInfo
(
st_HardwareInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  tl_EnterCriticalSection ();

  if (ac_CPUName [0] == '\0')
    {
    FILE * p_cpu;

    if ((p_cpu = fopen ("/proc/cpuinfo", "r")) != 0)
      {
      char * pc_line = 0;
      size_t o_len = 0;
      ssize_t o_read;

      while ((o_read = getline (& pc_line, & o_len, p_cpu)) != -1)
        {
        if ((o_read > 10) && (memcmp (pc_line, "model name", 10) == 0))
          {
          if (pc_line [o_read - 1] == '\n')
            pc_line [o_read - 1] = '\0';

          char * pc_value = pc_line + 10;
          char c_curr = * pc_value;

          while ((c_curr != '\0') && (c_curr <= ' '))
            c_curr = * (++ pc_value);

          if (c_curr == ':')
            {
            c_curr = * (++ pc_value);

            while ((c_curr != '\0') && (c_curr <= ' '))
              c_curr = * (++ pc_value);

            if (c_curr != '\0')
              {
              strncpy (ac_CPUName, pc_value, sizeof (ac_CPUName) - 1);
              break;
              }
            }
          }
        }

      free (pc_line);
      fclose (p_cpu);
      }

    if (ac_CPUName [0] == '\0')
      ac_CPUName [0] = '?';
    }

  tl_LeaveCriticalSection ();

  t_UInt64 u_pageSize = sysconf (_SC_PAGE_SIZE);
  pso_info-> u_TotalBytes = sysconf (_SC_PHYS_PAGES) * u_pageSize;
  pso_info-> u_AvailableBytes = sysconf (_SC_AVPHYS_PAGES) * u_pageSize;
  pso_info-> u_TotalProcessors = sysconf (_SC_NPROCESSORS_CONF);
  pso_info-> u_AvailableProcessors = sysconf (_SC_NPROCESSORS_ONLN);
  pso_info-> pc_CPUName = ac_CPUName;
  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryProcessMemoryInfo
(
st_ProcessMemoryInfo * pso_info
)
  {
  FILE * p_statm;

  if ((p_statm = fopen ("/proc/self/statm", "r")) != 0)
    {
    t_UInt u_vmSize;
    t_UInt u_rssSize;
    int i_scan = fscanf (p_statm, "%zu %zu", & u_vmSize, & u_rssSize);
    fclose (p_statm);

    if (i_scan == 2)
      {
      t_UInt u_pageSize = sysconf (_SC_PAGE_SIZE);
      pso_info-> u_VMBytes = u_vmSize * u_pageSize;
      pso_info-> u_RSSBytes = u_rssSize * u_pageSize;
      return true;
      }
    }

  pso_info-> u_VMBytes = 0;
  pso_info-> u_RSSBytes = 0;
  return false;
  }

//---------------------------------------------------------------------------

#include <gnu/libc-version.h>

#if defined TL_64
  #define COMP_ARCH " amd64"
#else
  #define COMP_ARCH " x86"
#endif

bool TL_EXPORT tl_QueryCompilerInfo
(
st_CompilerInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  pso_info-> eo_Compiler = ec_CompilerGCC;
  pso_info-> pc_CompilerVersion = STRSTR (__GNUC__) "."
    STRSTR (__GNUC_MINOR__) "."
    STRSTR (__GNUC_PATCHLEVEL__) COMP_ARCH;
  pso_info-> pc_RuntimeVersion = gnu_get_libc_version ();
  return true;
  }

//---------------------------------------------------------------------------

#include <sys/utsname.h>
#include <pwd.h>

static struct utsname so_UtsName;
static char ac_UserName [257];

bool TL_EXPORT tl_QuerySystemInfo
(
st_SystemInfo * pso_info
)
  {
  if (pso_info == 0)
    return false;

  tl_EnterCriticalSection ();

  if (so_UtsName. sysname [0] == '\0')
    {
    uname (& so_UtsName);
    struct passwd * pso_pwd = getpwuid (geteuid ());

    if ((pso_pwd == 0) || (pso_pwd-> pw_name == 0) ||
        (* pso_pwd-> pw_name == '\0') ||
        (strlen (pso_pwd-> pw_name) >= sizeof (ac_UserName)))
      ac_UserName [0] = '?';
    else
      strcpy (ac_UserName, pso_pwd-> pw_name);
    }

  tl_LeaveCriticalSection ();

  pso_info-> eo_System = ec_SystemLinux;
  pso_info-> pc_SystemVersion = so_UtsName. release;
  pso_info-> pc_ComputerName = so_UtsName. nodename;
  pso_info-> pc_UserName = ac_UserName;
  return true;
  }

//---------------------------------------------------------------------------

static bool ReadBatteryFile
(
const ct_FileName & co_partName,
char * pc_buffer,
size_t u_len
)
  {
  FILE * p_part = fopen (co_partName. GetAllStr (), "r");

  if (p_part != 0)
    {
    char * pc_line = 0;
    size_t o_len = 0;
    ssize_t o_read;

    if ((o_read = getline (& pc_line, & o_len, p_part)) != -1)
      {
      if ((o_read > 0) && (pc_line [o_read - 1] == '\n'))
        pc_line [o_read - 1] = '\0';

      strncpy (pc_buffer, pc_line, u_len);
      }
    else
      pc_buffer [0] = '\0';

    free (pc_line);
    fclose (p_part);
    return pc_buffer [0] != '\0';
    }

  return false;
  }

//--------------------------------------

bool TL_EXPORT tl_QueryBatteryInfo
(
st_BatteryInfo * pso_info
)
  {
  pso_info-> b_ACLine = true;
  pso_info-> b_BatteryFound = false;
  pso_info-> i_LifePercent = 100;
  ct_File co_dirName ("/sys/class/power_supply/BAT0");

  if (! co_dirName. Exists ())
    {
    co_dirName. SetNameExt ("BAT1");

    if (! co_dirName. Exists ())
      return true;
    }

  pso_info-> b_BatteryFound = true;
  char ac_buffer [257];
  ct_FileName co_partName;
  co_partName. AssignAsPath (co_dirName);
  co_partName. SetNameExt ("status");

  if (ReadBatteryFile (co_partName, ac_buffer, sizeof (ac_buffer)))
    {
    if (strncmp ("Full", ac_buffer, sizeof (ac_buffer)) == 0)
      {
      pso_info-> b_ACLine = true;
      pso_info-> i_LifePercent = 100;
      return true;
      }
    else if (strncmp ("Charging", ac_buffer, sizeof (ac_buffer)) == 0)
      pso_info-> b_ACLine = true;
    else if (strncmp ("Discharging", ac_buffer, sizeof (ac_buffer)) == 0)
      pso_info-> b_ACLine = false;
    else
      return false;

    co_partName. SetNameExt ("charge_full_design");

    if (ReadBatteryFile (co_partName, ac_buffer, sizeof (ac_buffer)))
      {
      int i_full = atoi (ac_buffer) / 1000;
      co_partName. SetNameExt ("charge_now");

      if (ReadBatteryFile (co_partName, ac_buffer, sizeof (ac_buffer)))
        {
        int i_now = atoi (ac_buffer) / 1000;

        if (i_full != 0)
          {
          pso_info-> i_LifePercent = (100 * i_now) / i_full;
          return true;
          }
        }
      }
    }

  pso_info-> b_ACLine = false;
  pso_info-> b_BatteryFound = false;
  pso_info-> i_LifePercent = 0;
  return false;
  }

#endif
