
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
// File tuning/dirscan.cpp

#include "tuning/dirscan.hpp"
#include "tuning/timedate.hpp"

//---------------------------------------------------------------------------

void ct_DirScan::Clear ()
  {
  SetNameExt ("");
  o_CreationTime = 0;
  o_LastAccessTime = 0;
  o_LastWriteTime = 0;
  o_Size = 0;
  o_Attributes = 0;
  b_Found = false;
  }

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>
#include "tuning/sys/cmemory.hpp"
#include "tuning/wstring.hpp"

#undef FindFirstFile
#undef FindNextFile

//---------------------------------------------------------------------------

struct st_DirInfo
  {
  HANDLE               h_Dir;
  WIN32_FIND_DATAW     so_FindData;
  };

class ct_DirInfoSizeChecker
  {
public:
  ct_DirInfoSizeChecker ()
    { TL_ASSERT (cu_DirScanDataSize >= sizeof (st_DirInfo)); }
  }
static CO_DirInfoSizeChecker;

#define CO_DIRINFO ((st_DirInfo &) (au_Data [0]))

//---------------------------------------------------------------------------

void ct_DirScan::ClearIntern ()
  {
  Clear ();
  CO_DIRINFO. h_Dir = 0;
  }

//---------------------------------------------------------------------------

void ct_DirScan::CopyResult ()
  {
  ct_String co_fileName;
  co_fileName. MbConvert (CO_DIRINFO. so_FindData. cFileName);
  SetNameExt (co_fileName);
  ULARGE_INTEGER uo_li;

  if ((CO_DIRINFO. so_FindData. ftCreationTime. dwLowDateTime == 0) &&
      (CO_DIRINFO. so_FindData. ftCreationTime. dwHighDateTime == 0))
    o_CreationTime = 0;
  else
    {
    uo_li. u. LowPart = CO_DIRINFO. so_FindData. ftCreationTime. dwLowDateTime;
    uo_li. u. HighPart = CO_DIRINFO. so_FindData. ftCreationTime. dwHighDateTime;
    o_CreationTime = (uo_li. QuadPart - co_1970) / 10ll;
    }

  if ((CO_DIRINFO. so_FindData. ftLastAccessTime. dwLowDateTime == 0) &&
      (CO_DIRINFO. so_FindData. ftLastAccessTime. dwHighDateTime == 0))
    o_LastAccessTime = 0;
  else
    {
    uo_li. u. LowPart = CO_DIRINFO. so_FindData. ftLastAccessTime. dwLowDateTime;
    uo_li. u. HighPart = CO_DIRINFO. so_FindData. ftLastAccessTime. dwHighDateTime;
    o_LastAccessTime = (uo_li. QuadPart - co_1970) / 10ll;
    }

  uo_li. u. LowPart = CO_DIRINFO. so_FindData. ftLastWriteTime. dwLowDateTime;
  uo_li. u. HighPart = CO_DIRINFO. so_FindData. ftLastWriteTime. dwHighDateTime;
  o_LastWriteTime = (uo_li. QuadPart - co_1970) / 10ll;

  uo_li. u. LowPart = CO_DIRINFO. so_FindData. nFileSizeLow;
  uo_li. u. HighPart = CO_DIRINFO. so_FindData. nFileSizeHigh;
  o_Size = uo_li. QuadPart;

  o_Attributes = 0;

  if ((CO_DIRINFO. so_FindData. dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0)
    o_Attributes |= co_AttrArchive;

  if ((CO_DIRINFO. so_FindData. dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
    o_Attributes |= co_AttrDirectory;

  if ((CO_DIRINFO. so_FindData. dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0)
    o_Attributes |= co_AttrHidden;

  if ((CO_DIRINFO. so_FindData. dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0)
    o_Attributes |= co_AttrReadOnly;

  if ((CO_DIRINFO. so_FindData. dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0)
    o_Attributes |= co_AttrSystem;

  b_Found = true;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindFirst ()
  {
  if (GetAllLen () == 0)
    return false;

  if (b_Found)
    return false;

  ct_WString co_wPath;

  if (! co_wPath. MbConvert (GetAllStr ()))
    return false;

  CO_DIRINFO. h_Dir = FindFirstFileW (co_wPath. GetStr (), & CO_DIRINFO. so_FindData);

  if (CO_DIRINFO. h_Dir != INVALID_HANDLE_VALUE)
    CopyResult ();

  return b_Found;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindNext ()
  {
  if (! b_Found)
    return false;

  if (FindNextFileW (CO_DIRINFO. h_Dir, & CO_DIRINFO. so_FindData))
    CopyResult ();
  else
    AbortFind ();

  return b_Found;
  }

//---------------------------------------------------------------------------

void ct_DirScan::AbortFind ()
  {
  if (b_Found)
    {
    FindClose (CO_DIRINFO. h_Dir);
    ClearIntern ();
    }
  }

//===========================================================================

#else // POSIX conform

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

//---------------------------------------------------------------------------

struct st_DirInfo
  {
  DIR *                pso_Dir;
  dirent *             pso_DirEnt;
  struct stat          so_Stat;
  };

class ct_DirInfoSizeChecker
  {
public:
  ct_DirInfoSizeChecker ()
    { TL_ASSERT (cu_DirScanDataSize >= sizeof (st_DirInfo)); }
  }
static CO_DirInfoSizeChecker;

#define CO_DIRINFO ((st_DirInfo &) (au_Data [0]))

//---------------------------------------------------------------------------

void ct_DirScan::ClearIntern ()
  {
  Clear ();
  CO_DIRINFO. pso_Dir = 0;
  CO_DIRINFO. pso_DirEnt = 0;
  }

//---------------------------------------------------------------------------

void ct_DirScan::CopyResult ()
  {
  o_CreationTime = CO_DIRINFO. so_Stat. st_ctime * co_SecondFactor;
  o_LastAccessTime = CO_DIRINFO. so_Stat. st_atime * co_SecondFactor;
  o_LastWriteTime = CO_DIRINFO. so_Stat. st_mtime * co_SecondFactor;
  o_Size = CO_DIRINFO. so_Stat. st_size;
  o_Attributes = 0;

  if (S_ISDIR (CO_DIRINFO. so_Stat. st_mode))
    o_Attributes |= co_AttrDirectory;

  if ((CO_DIRINFO. so_Stat. st_mode & S_IWUSR) == 0)
    o_Attributes |= co_AttrReadOnly;

  b_Found = true;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindFirst ()
  {
  if (GetAllLen () == 0)
    return false;

  if (b_Found)
    return false;

  if ((GetNameExtLen () == 1) &&
      (* GetNameStr () == '*'))
    {
    SetNameExt ("");
    CO_DIRINFO. pso_Dir = opendir (GetAllStr ());

    if (CO_DIRINFO. pso_Dir != 0)
      {
      b_Found = true;
      return FindNext ();
      }
    }
  else
    {
    if ((! HasWildCards ()) &&
        (GetNameExtLen () != 0) &&
        (stat (GetAllStr (), & CO_DIRINFO. so_Stat) == 0))
      CopyResult ();
    }

  return b_Found;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindNext ()
  {
  if (! b_Found)
    return false;

  if (CO_DIRINFO. pso_Dir == 0)
    return b_Found = false;

  for (;;)
    {
    CO_DIRINFO. pso_DirEnt = readdir (CO_DIRINFO. pso_Dir);

    if (CO_DIRINFO. pso_DirEnt != 0)
      {
      SetNameExt (CO_DIRINFO. pso_DirEnt-> d_name);

      if (stat (GetAllStr (), & CO_DIRINFO. so_Stat) == 0)
        {
        CopyResult ();
        return true;
        }
      }
    else
      {
      AbortFind ();
      return false;
      }
    }
  }

//---------------------------------------------------------------------------

void ct_DirScan::AbortFind ()
  {
  if (b_Found)
    {
    if (CO_DIRINFO. pso_Dir != 0)
      closedir (CO_DIRINFO. pso_Dir);

    ClearIntern ();
    }
  }

#endif

//---------------------------------------------------------------------------

ct_DirScan::ct_DirScan ()
  {
  ClearIntern ();
  }

//---------------------------------------------------------------------------

ct_DirScan::ct_DirScan
(
const char * pc_init
)
  {
  AssignAsName (pc_init);
  ClearIntern ();
  }

//---------------------------------------------------------------------------

ct_DirScan::ct_DirScan
(
const ct_FileName & co_init
): ct_Directory (co_init)
  {
  ClearIntern ();
  }

//---------------------------------------------------------------------------

ct_DirScan::~ct_DirScan ()
  {
  AbortFind ();
  }

//---------------------------------------------------------------------------

ct_DirScan & ct_DirScan::operator =
(
const char * pc_asgn
)
  {
  AbortFind ();
  AssignAsName (pc_asgn);
  return * this;
  }

//---------------------------------------------------------------------------

ct_DirScan & ct_DirScan::operator =
(
const ct_FileName & co_asgn
)
  {
  AbortFind ();
  ct_FileName::operator = (co_asgn);
  return * this;
  }

//---------------------------------------------------------------------------

TL_ISKINDOF (ct_DirScan, ct_Directory)

//---------------------------------------------------------------------------

bool ct_DirScan::FindOnce ()
  {
  AbortFind ();
  return FindFirst ();
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindOnce
(
const char * pc_find
)
  {
  AbortFind ();
  AssignAsName (pc_find);
  return FindFirst ();
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindOnce
(
const ct_FileName & co_find
)
  {
  AbortFind ();
  AssignAsName (co_find);
  return FindFirst ();
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindOncePath ()
  {
  AbortFind ();
  AssignAsName (GetPureDrivePath ());
  return FindFirst ();
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindOncePath
(
const ct_FileName & co_find
)
  {
  AbortFind ();
  AssignAsName (co_find. GetPureDrivePath ());
  return FindFirst ();
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindFirstFile ()
  {
  if (FindFirst ())
    {
    if (! IsDirectory ())
      return true;
    else
      return FindNextFile ();
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindFirstDirectory ()
  {
  if (FindFirst ())
    {
    if (IsDirectory ())
      return true;
    else
      return FindNextDirectory ();
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindNextFile ()
  {
  while (FindNext ())
    if (! IsDirectory ())
      return true;

  return false;
  }

//---------------------------------------------------------------------------

bool ct_DirScan::FindNextDirectory ()
  {
  while (FindNext ())
    if (IsDirectory ())
      return true;

  return false;
  }
