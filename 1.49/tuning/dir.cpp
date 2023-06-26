
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
// File tuning/dir.cpp

#include "tuning/dir.hpp"
#include "tuning/sys/cdir.hpp"
#include "tuning/dirscan.hpp"

//---------------------------------------------------------------------------

ct_Directory::ct_Directory
(
const char * pc_init
)
  {
  AssignAsPath (pc_init);
  }

//---------------------------------------------------------------------------

ct_Directory::ct_Directory
(
const ct_FileName & co_init
): ct_FileName (co_init)
  {
  }

//---------------------------------------------------------------------------

ct_Directory & ct_Directory::operator =
(
const char * pc_asgn
)
  {
  AssignAsPath (pc_asgn);
  return * this;
  }

//---------------------------------------------------------------------------

ct_Directory & ct_Directory::operator =
(
const ct_FileName & co_asgn
)
  {
  ct_FileName::operator = (co_asgn);
  return * this;
  }

//---------------------------------------------------------------------------

TL_ISKINDOF (ct_Directory, ct_FileName)

//---------------------------------------------------------------------------

bool ct_Directory::QueryCurrentDrive ()
  {
  ct_String co_currentDirectory;

  if (tl_QueryCurrentDirectory ("", 0, co_currentDirectory))
    {
    ct_FileName co_name;
    co_name. AssignAsPath (co_currentDirectory);
    CopyDriveFrom (& co_name);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_Directory::QueryCurrentDirectory ()
  {
  ct_String co_currentDirectory;

  if (tl_QueryCurrentDirectory
      (GetDriveStr (), GetDriveLen (), co_currentDirectory))
    {
    ct_FileName co_name;
    co_name. AssignAsPath (co_currentDirectory);
    CopyPathFrom (& co_name);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_Directory::QueryCurrentDriveDirectory ()
  {
  ct_String co_currentDirectory;

  if (tl_QueryCurrentDirectory
      (GetDriveStr (), GetDriveLen (), co_currentDirectory))
    {
    ct_FileName co_name;
    co_name. AssignAsPath (co_currentDirectory);
    CopyDrivePathFrom (& co_name);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_Directory::Create ()
  {
  return tl_CreateDirectory
    (ct_String (GetDriveStr (), GetPureDrivePathLen ()). GetStr ());
  }

//---------------------------------------------------------------------------

bool ct_Directory::Exists ()
  {
  if (GetPathLen () == 0)
    return false;
  else
    {
    if (GetPathLen () == 1)
      return true;
    else
      {
      ct_DirScan co_dirScan;
      co_dirScan. AssignAsName (GetDriveStr (), GetPureDrivePathLen ());
      return co_dirScan. FindFirst ();
      }
    }
  }

//---------------------------------------------------------------------------

bool ct_Directory::Move
(
const char * pc_new
)
  {
  if (tl_MoveDirectory
    (ct_String (GetDriveStr (), GetPureDrivePathLen ()). GetStr (), pc_new))
    {
    SetDrivePath (pc_new);
    return true;
    }
  else
    return false;
  }

//---------------------------------------------------------------------------

bool ct_Directory::Delete ()
  {
  return tl_DeleteDirectory
    (ct_String (GetDriveStr (), GetPureDrivePathLen ()). GetStr ());
  }
