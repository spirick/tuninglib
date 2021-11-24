
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
// File tuning/sys/cdir.cpp

#include "tuning/sys/cdir.hpp"

//===========================================================================

#if defined TL_WINDOWS

#include <windows.h>
#include "tuning/sys/calloc.hpp"
#include "tuning/sys/cmemory.hpp"
#include "tuning/wstring.hpp"

typedef gct_EmptyBaseMiniBlock <ct_Chn_Store> t_DirBlock;

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryCurrentDirectory
(
const char * pc_drive,
t_UInt u_driveLen,
ct_String & co_currentDirectory
)
  {
  co_currentDirectory = "";
  ct_String co_drive (pc_drive, u_driveLen);
  ct_WString co_wDrive;

  if (! co_wDrive. MbConvert (co_drive))
    return false;

  co_wDrive. Append (L'*');
  wchar_t * pc_star = 0;
  t_DirBlock co_curDirBlock;
  co_curDirBlock. SetByteSize (MAX_PATH * sizeof (wchar_t));
  DWORD o_len = GetFullPathNameW (co_wDrive. GetStr (), MAX_PATH,
    (wchar_t *) co_curDirBlock. GetAddr (), & pc_star);

  if (o_len >= MAX_PATH)
    {
    pc_star = 0;
    co_curDirBlock. SetByteSize (o_len * sizeof (wchar_t));
    o_len = GetFullPathNameW (co_wDrive. GetStr (), o_len,
      (wchar_t *) co_curDirBlock. GetAddr (), & pc_star);
    }

  if ((o_len == 0) || (pc_star == 0))
    return false;

  * pc_star = L'\0';
  return co_currentDirectory. MbConvert ((const wchar_t *) co_curDirBlock. GetAddr ());
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CreateDirectory
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  SECURITY_ATTRIBUTES so_attr;
  so_attr. nLength = sizeof (so_attr);
  so_attr. lpSecurityDescriptor = 0;
  so_attr. bInheritHandle = TRUE;
  return CreateDirectoryW (co_name. GetStr (), & so_attr) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MoveDirectory
(
const char * pc_old,
const char * pc_new
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  ct_WString co_old;
  ct_WString co_new;

  if (! co_old. MbConvert (pc_old))
    return false;

  if (! co_new. MbConvert (pc_new))
    return false;

  return MoveFileW (co_old. GetStr (), co_new. GetStr ()) != 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_DeleteDirectory
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  ct_WString co_name;

  if (! co_name. MbConvert (pc_name))
    return false;

  return RemoveDirectoryW (co_name. GetStr ()) != 0;
  }

//===========================================================================

#else // POSIX conform

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

//---------------------------------------------------------------------------

bool TL_EXPORT tl_QueryCurrentDirectory
(
const char * /* pc_drive */,
t_UInt /* u_driveLen */,
ct_String & co_currentDirectory
)
  {
  char * pc_curDir = getcwd (0, 0);

  if (pc_curDir == 0)
    {
    co_currentDirectory = "";
    return false;
    }

  co_currentDirectory = pc_curDir;
  free (pc_curDir);
  return true;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_CreateDirectory
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  return mkdir (pc_name, 0755) == 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_MoveDirectory
(
const char * pc_old,
const char * pc_new
)
  {
  if ((pc_old == 0) || (* pc_old == '\0') ||
      (pc_new == 0) || (* pc_new == '\0'))
    return false;

  return rename (pc_old, pc_new) == 0;
  }

//---------------------------------------------------------------------------

bool TL_EXPORT tl_DeleteDirectory
(
const char * pc_name
)
  {
  if ((pc_name == 0) || (* pc_name == '\0'))
    return false;

  return rmdir (pc_name) == 0;
  }

#endif
