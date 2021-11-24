
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
// File tuning/filename.cpp

#include "tuning/filename.hpp"

//---------------------------------------------------------------------------

t_Int ct_FileName::FnReplace
(
t_Size o_start,
t_Size o_end,
const char * pc_repl,
t_Size o_len
)
  {
  TL_ASSERT (o_start <= o_end);
  TL_ASSERT (o_end <= GetLen ());
  TL_ASSERT (pc_repl != 0);

  Replace (o_start, o_end - o_start, ct_String (pc_repl, o_len));

  if (GetLen () > 0)
    {
    t_Int i_slashPos = 0;

    while ((i_slashPos = First (c_AltSlashChar, i_slashPos)) >= 0)
      GetChar (i_slashPos) = c_SlashChar;

    i_slashPos = 1;

    while ((i_slashPos =
            First (pc_SlashStr pc_SlashStr, i_slashPos)) >= 0)
      {
      o_len --;
      Delete (i_slashPos, 1);
      }
    }

  return o_start + o_len - o_end;
  }

//---------------------------------------------------------------------------

void ct_FileName::CheckPathEnd ()
  {
  if (HasPath () && (GetChar (o_Name - 1) != c_SlashChar))
    {
    Insert (o_Name, c_SlashChar);
    o_Name ++;
    o_Ext ++;
    }
  }

//---------------------------------------------------------------------------

ct_FileName::t_Size ct_FileName::ScanDriveOrUNC ()
  {
  if (GetLen () >= 2)
    {
    if ((tl_ToUpper (GetChar (0)) >= 'A') &&
        (tl_ToUpper (GetChar (0)) <= 'Z') &&
        (GetChar (1) == ':'))
      return 2;
    else
      {
      if ((GetChar (0) == '\\') && (GetChar (1) == '\\'))
        {
        t_Int i_slash = First ('\\', 2);

        if (i_slash > 0)
          {
          i_slash = First ('\\', i_slash + 1);

          if (i_slash > 0)
            return i_slash;
          }
        }
      }
    }

  return 0;
  }

//---------------------------------------------------------------------------

ct_FileName::ct_FileName ()
  {
  o_Path = o_Name = o_Ext = 0;
  }

//---------------------------------------------------------------------------

ct_FileName::ct_FileName
(
const char * pc_init
)
  {
  AssignAsName (pc_init);
  }

//---------------------------------------------------------------------------

ct_FileName & ct_FileName::operator =
(
const char * pc_asgn
)
  {
  AssignAsName (pc_asgn);
  return * this;
  }

//---------------------------------------------------------------------------

ct_FileName & ct_FileName::operator =
(
const ct_FileName & co_asgn
)
  {
  Assign (co_asgn);
  o_Path = co_asgn. o_Path;
  o_Name = co_asgn. o_Name;
  o_Ext  = co_asgn. o_Ext;
  return * this;
  }

//---------------------------------------------------------------------------

TL_ISKINDOF (ct_FileName, ct_String)

//---------------------------------------------------------------------------

void ct_FileName::AssignAsPath
(
const char * pc_path,
t_Size u_len
)
  {
  FnReplace (0, GetLen (), pc_path, u_len);
  o_Path = ScanDriveOrUNC ();
  o_Name = o_Ext = GetLen ();
  CheckPathEnd ();
  }

//---------------------------------------------------------------------------

void ct_FileName::AssignAsName
(
const char * pc_name,
t_Size u_len
)
  {
  FnReplace (0, GetLen (), pc_name, u_len);
  o_Path = ScanDriveOrUNC ();

  t_Int i_slash = First (c_SlashChar, o_Path);

  if (i_slash < 0)
    o_Name = o_Path;
  else
    while (i_slash >= 0)
      {
      o_Name = i_slash + 1;
      i_slash = First (c_SlashChar, i_slash + 1);
      }

  if (((o_Name + 1 == GetLen ()) &&
       (CompSubStr (o_Name, '.') == 0)) ||
      ((o_Name + 2 == GetLen ()) &&
       (CompSubStr (o_Name, "..") == 0)))
    o_Ext = GetLen ();
  else
    {
    t_Int i_dot = Last ('.', o_Name);

    if (i_dot < 0)
      o_Ext = GetLen ();
    else
      if (((t_Size) i_dot) + 1 < GetLen ())
        o_Ext = i_dot + 1;
      else
        {
        Delete (i_dot);
        o_Ext = i_dot;
        }
    }
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasDriveOrUNC () const
  {
  return GetDriveLen () > 0;
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasDrive () const
  {
  return HasDriveOrUNC () && (GetChar (1) == ':');
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasUNC () const
  {
  return HasDriveOrUNC () && (GetChar (0) == '\\');
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasPath () const
  {
  return GetPathLen () > 0;
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasName () const
  {
  return GetNameLen () > 0;
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasExt () const
  {
  return GetExtLen () > 0;
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasDot () const
  {
  return (o_Ext > 0) && (GetChar (o_Ext - 1) == '.');
  }

//---------------------------------------------------------------------------

bool ct_FileName::HasWildCards () const
  {
  return (First ('*', o_Name) >= 0) ||
         (First ('?', o_Name) >= 0);
  }

//---------------------------------------------------------------------------

void ct_FileName::SetDrive
(
const char * pc,
t_Size u_len
)
  {
  t_Int i = FnReplace (0, o_Path, pc, u_len);
  o_Path += i;
  o_Name += i;
  o_Ext += i;

  if ((u_len >= 2) && (GetChar (0) == '\\') && (GetChar (1) == '\\') &&
      IsRel () && (o_Path < GetLen ()))
    {
    Insert (o_Path, c_SlashChar);
    o_Name ++;
    o_Ext ++;
    }
  }

//---------------------------------------------------------------------------

void ct_FileName::SetPath
(
const char * pc,
t_Size u_len
)
  {
  t_Int i = FnReplace (o_Path, o_Name, pc, u_len);
  o_Name += i;
  o_Ext += i;

  CheckPathEnd ();
  }

//---------------------------------------------------------------------------

void ct_FileName::SetDrivePath
(
const char * pc,
t_Size u_len
)
  {
  t_Int i = FnReplace (0, o_Name, pc, u_len);

  o_Path = ScanDriveOrUNC ();
  o_Name += i;
  o_Ext += i;

  CheckPathEnd ();
  }

//---------------------------------------------------------------------------

void ct_FileName::SetName
(
const char * pc,
t_Size u_len
)
  {
  o_Ext +=
    FnReplace (o_Name, o_Ext - GetDotLen (), pc, u_len);
  }

//---------------------------------------------------------------------------

void ct_FileName::SetExt
(
const char * pc,
t_Size u_len
)
  {
  if (* pc == '.')
    {
    pc ++;
    u_len --;
    }

  if (u_len == 0)
    {
    if (HasDot ())
      o_Ext --;
    }
  else
    if (! HasDot ())
      Insert (o_Ext ++, '.');

  FnReplace (o_Ext, GetLen (), pc, u_len);
  }

//---------------------------------------------------------------------------

void ct_FileName::SetNameExt
(
const char * pc,
t_Size u_len
)
  {
  FnReplace (o_Name, GetLen (), pc, u_len);

  if (((o_Name + 1 == GetLen ()) &&
       (CompSubStr (o_Name, '.') == 0)) ||
      ((o_Name + 2 == GetLen ()) &&
       (CompSubStr (o_Name, "..") == 0)))
    o_Ext = GetLen ();
  else
    {
    t_Int i_dot = Last ('.', o_Name);

    if (i_dot < 0)
      o_Ext = GetLen ();
    else
      if (((t_Size) i_dot) + 1 < GetLen ())
        o_Ext = i_dot + 1;
      else
        {
        Delete (i_dot);
        o_Ext = i_dot;
        }
    }
  }

//---------------------------------------------------------------------------

void ct_FileName::InsertPath
(
const char * pc_path,
t_Size u_len
)
  {
  ct_FileName co_path;
  co_path. AssignAsPath (pc_path, u_len);

  t_Size o_pathLen = co_path. GetPathLen ();

  if (IsAbs ())
    o_pathLen --;

  Insert (o_Path, co_path. GetPathStr (), o_pathLen);
  o_Name += o_pathLen;
  o_Ext += o_pathLen;
  }

//---------------------------------------------------------------------------

void ct_FileName::InsertDrivePath
(
const char * pc_path,
t_Size u_len
)
  {
  ct_FileName co_path;
  co_path. AssignAsPath (pc_path, u_len);
  CopyDriveFrom (& co_path);

  t_Size o_pathLen = co_path. GetPathLen ();

  if (IsAbs ())
    o_pathLen --;

  Insert (o_Path, co_path. GetPathStr (), o_pathLen);
  o_Name += o_pathLen;
  o_Ext += o_pathLen;
  }

//---------------------------------------------------------------------------

void ct_FileName::AppendPath
(
const char * pc_path,
t_Size u_len
)
  {
  ct_FileName co_path;
  co_path. AssignAsPath (pc_path, u_len);

  if (co_path. IsAbs ())
    co_path. o_Path ++;

  Insert (o_Name, co_path. GetPathStr (), co_path. GetPathLen ());
  o_Name += co_path. GetPathLen ();
  o_Ext += co_path. GetPathLen ();
  }

//---------------------------------------------------------------------------

void ct_FileName::CompressPath ()
  {
  if (HasPath () && (First ("." pc_SlashStr, o_Path) >= 0))
    {
    ct_String co_path (GetPathStr (), GetPathLen ());

    // Delete ".\\"
    t_Int i_dot = 0;

    while ((i_dot = co_path. First ("." pc_SlashStr, i_dot)) >= 0)
      if ((i_dot == 0) || (co_path [i_dot - 1] == c_SlashChar))
        co_path. Delete (i_dot, 2);
      else
        i_dot ++;

    // Delete "path\\..\\"
    t_Int i_last = co_path [0] == c_SlashChar ? 1 : 0;

    while ((i_dot = co_path. First (c_SlashChar, i_last)) >= 0)
      {
      i_dot ++;

      if ((co_path. CompSubStr (i_last, ".." pc_SlashStr) != 0) &&
          (co_path. CompSubStr (i_dot,  ".." pc_SlashStr) == 0))
        {
        co_path. Delete (i_last, i_dot - i_last + 3);
        i_last = co_path [0] == c_SlashChar ? 1 : 0;
        }
      else
        i_last = i_dot;
      }

    SetPath (co_path. GetStr (), co_path. GetLen ());
    }
  }

//-------------------------------------------------------------------------

bool ct_FileName::IsAbs () const
  {
  return HasPath () && (GetChar (o_Path) == c_SlashChar);
  }

//-------------------------------------------------------------------------

bool ct_FileName::IsRel () const
  {
  return ! HasPath () || (GetChar (o_Path) != c_SlashChar);
  }

//---------------------------------------------------------------------------

void ct_FileName::ToAbs
(
const char * pc_currDrivePath,
bool b_withDrive
)
  {
  TL_ASSERT (IsRel ());

  if (b_withDrive)
    {
    InsertDrivePath (pc_currDrivePath);
    CompressPath ();
    }
  else
    {
    SetDrive ("", 0);
    InsertPath (pc_currDrivePath);
    CompressPath ();
    }
  }

//---------------------------------------------------------------------------

void ct_FileName::ToRel
(
const char * pc_currDrivePath,
bool b_withDrive
)
  {
  TL_ASSERT (IsAbs ());

  ct_FileName co_curr;
  co_curr. AssignAsPath (pc_currDrivePath);

  if (b_withDrive)
    CopyDriveFrom (& co_curr);
  else
    SetDrive ("", 0);

  ct_String co_currPath (co_curr. GetPathStr (), co_curr. GetPathLen ());
  ct_String co_thisPath (GetPathStr (), GetPathLen ());

  // Delete leading '\\'
  co_currPath. Delete (0, 1);
  co_thisPath. Delete (0, 1);

  t_Int i_slash;

  while (((i_slash = co_currPath. First (c_SlashChar)) >= 0) &&
         (co_currPath. CompSubStr
          (0, co_thisPath. GetStr (), i_slash + 1) == 0))
    {
    co_currPath. Delete (0, i_slash + 1);
    co_thisPath. Delete (0, i_slash + 1);
    }

  while ((i_slash = co_currPath. First (c_SlashChar)) >= 0)
    {
    co_currPath. Delete (0, i_slash + 1);
    co_thisPath. Insert (0, ".." pc_SlashStr);
    }

  SetPath (co_thisPath. GetStr (), co_thisPath. GetLen ());
  }
