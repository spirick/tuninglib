
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
// File tuning/filename.hpp

#ifndef TUNING_FILENAME_HPP
#define TUNING_FILENAME_HPP

#include "tuning/string.hpp"
#include "tuning/sys/cstring.hpp"

//---------------------------------------------------------------------------

#if defined TL_UNIX

  const char         c_SlashChar    = '/';
  const char         c_AltSlashChar = '\\';
  #define            pc_SlashStr      "/"
  #define            pc_AltSlashStr   "\\"

#else

  const char         c_SlashChar    = '\\';
  const char         c_AltSlashChar = '/';
  #define            pc_SlashStr      "\\"
  #define            pc_AltSlashStr   "/"

#endif

//---------------------------------------------------------------------------

class TL_EXPORT ct_FileName: public ct_String
  {
  t_Size               o_Path;
  t_Size               o_Name;
  t_Size               o_Ext;

  t_Int                FnReplace (t_Size o_start, t_Size o_end,
                         const char * pc_repl, t_Size o_len);
  void                 CheckPathEnd ();
  t_Size               ScanDriveOrUNC ();

public:
                       ct_FileName ();
                       ct_FileName (const char * pc_init);
  ct_FileName &        operator = (const char * pc_asgn);
  ct_FileName &        operator = (const ct_FileName & co_asgn);

  TL_CLASSID (ct_FileName)

  inline void          AssignAsPath (const char * pc_path);
  void                 AssignAsPath (const char * pc_path, t_Size u_len);
  inline void          AssignAsPath (const ct_String & co_path);
  inline void          AssignAsName (const char * pc_name);
  void                 AssignAsName (const char * pc_name, t_Size u_len);
  inline void          AssignAsName (const ct_String & co_name);

  // Substring existence
  bool                 HasDriveOrUNC () const;
  bool                 HasDrive () const;
  bool                 HasUNC () const;
  bool                 HasPath () const;
  bool                 HasName () const;
  bool                 HasExt () const;
  bool                 HasDot () const;
  bool                 HasWildCards () const;

  // Substring length
  inline t_Size        GetDriveLen () const;
  inline t_Size        GetPathLen () const;
  inline t_Size        GetPurePathLen () const;
  inline t_Size        GetDrivePathLen () const;
  inline t_Size        GetPureDrivePathLen () const;
  inline t_Size        GetNameLen () const;
  inline t_Size        GetExtLen () const;
  inline t_Size        GetNameExtLen () const;
  inline t_Size        GetDotLen () const;
  inline t_Size        GetAllLen () const;

  // Substring access
  inline t_Size        GetDriveOffs () const;
  inline t_Size        GetPathOffs () const;
  inline t_Size        GetNameOffs () const;
  inline t_Size        GetExtOffs () const;

  inline const char *  GetDriveStr () const;
  inline const char *  GetPathStr () const;
  inline const char *  GetNameStr () const;
  inline const char *  GetExtStr () const;
  inline const char *  GetAllStr () const;

  inline ct_String     GetDrive () const;
  inline ct_String     GetPath () const;
  inline ct_String     GetPurePath () const;
  inline ct_String     GetDrivePath () const;
  inline ct_String     GetPureDrivePath () const;
  inline ct_String     GetName () const;
  inline ct_String     GetExt () const;
  inline ct_String     GetNameExt () const;

  inline void          SetDrive (const char * pc);
  void                 SetDrive (const char * pc, t_Size u_len);
  inline void          SetDrive (const ct_String & co);
  inline void          SetPath (const char * pc);
  void                 SetPath (const char * pc, t_Size u_len);
  inline void          SetPath (const ct_String & co);
  inline void          SetDrivePath (const char * pc);
  void                 SetDrivePath (const char * pc, t_Size u_len);
  inline void          SetDrivePath (const ct_String & co);
  inline void          SetName (const char * pc);
  void                 SetName (const char * pc, t_Size u_len);
  inline void          SetName (const ct_String & co);
  inline void          SetExt (const char * pc);
  void                 SetExt (const char * pc, t_Size u_len);
  inline void          SetExt (const ct_String & co);
  inline void          SetNameExt (const char * pc);
  void                 SetNameExt (const char * pc, t_Size u_len);
  inline void          SetNameExt (const ct_String & co);

  inline void          CopyDriveFrom (const ct_FileName * pco_copy);
  inline void          CopyPathFrom (const ct_FileName * pco_copy);
  inline void          CopyDrivePathFrom (const ct_FileName * pco_copy);
  inline void          CopyNameFrom (const ct_FileName * pco_copy);
  inline void          CopyExtFrom (const ct_FileName * pco_copy);
  inline void          CopyNameExtFrom (const ct_FileName * pco_copy);

  // Path manipulation
  inline void          InsertPath (const char * pc_path);
  void                 InsertPath (const char * pc_path, t_Size u_len);
  inline void          InsertPath (const ct_String & co_path);
  inline void          InsertDrivePath (const char * pc_path);
  void                 InsertDrivePath (const char * pc_path, t_Size u_len);
  inline void          InsertDrivePath (const ct_String & co_path);
  inline void          AppendPath (const char * pc_path);
  void                 AppendPath (const char * pc_path, t_Size u_len);
  inline void          AppendPath (const ct_String & co_path);
  void                 CompressPath ();
  bool                 IsAbs () const;
  bool                 IsRel () const;
  void                 ToAbs (const char * pc_currDrivePath,
                         bool b_withDrive = true);
  void                 ToRel (const char * pc_currDrivePath,
                         bool b_withDrive = false);
  };

//---------------------------------------------------------------------------

inline void ct_FileName::AssignAsPath
(
const char * pc_path
)
  {
  AssignAsPath (pc_path, tl_StringLength (pc_path));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::AssignAsPath
(
const ct_String & co_path
)
  {
  AssignAsPath (co_path. GetStr (), co_path. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::AssignAsName
(
const char * pc_name
)
  {
  AssignAsName (pc_name, tl_StringLength (pc_name));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::AssignAsName
(
const ct_String & co_name
)
  {
  AssignAsName (co_name. GetStr (), co_name. GetLen ());
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetDriveLen () const
  {
  return o_Path;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetPathLen () const
  {
  return o_Name - o_Path;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetPurePathLen () const
  {
  t_Size u = GetPathLen ();

  return u > 1 ? u - 1 : u;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetDrivePathLen () const
  {
  return o_Name;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetPureDrivePathLen () const
  {
  t_Size u = GetDrivePathLen ();

  return u > 1 ? u - 1 : u;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetNameLen () const
  {
  return o_Ext - o_Name - GetDotLen ();
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetExtLen () const
  {
  return GetLen () - o_Ext;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetNameExtLen () const
  {
  return GetLen () - o_Name;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetDotLen () const
  {
  return HasDot () ? 1 : 0;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetAllLen () const
  {
  return GetLen ();
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetDriveOffs () const
  {
  return 0;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetPathOffs () const
  {
  return o_Path;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetNameOffs () const
  {
  return o_Name;
  }

//---------------------------------------------------------------------------

inline ct_FileName::t_Size ct_FileName::GetExtOffs () const
  {
  return o_Ext;
  }

//---------------------------------------------------------------------------

inline const char * ct_FileName::GetDriveStr () const
  {
  return GetStr (0);
  }

//---------------------------------------------------------------------------

inline const char * ct_FileName::GetPathStr () const
  {
  return GetStr (o_Path);
  }

//---------------------------------------------------------------------------

inline const char * ct_FileName::GetNameStr () const
  {
  return GetStr (o_Name);
  }

//---------------------------------------------------------------------------

inline const char * ct_FileName::GetExtStr () const
  {
  return GetStr (o_Ext);
  }

//---------------------------------------------------------------------------

inline const char * ct_FileName::GetAllStr () const
  {
  return GetStr ();
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetDrive () const
  {
  return ct_String (GetDriveStr (), GetDriveLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetPath () const
  {
  return ct_String (GetPathStr (), GetPathLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetPurePath () const
  {
  return ct_String (GetPathStr (), GetPurePathLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetDrivePath () const
  {
  return ct_String (GetDriveStr (), GetDrivePathLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetPureDrivePath () const
  {
  return ct_String (GetDriveStr (), GetPureDrivePathLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetName () const
  {
  return ct_String (GetNameStr (), GetNameLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetExt () const
  {
  return ct_String (GetExtStr (), GetExtLen ());
  }

//---------------------------------------------------------------------------

inline ct_String ct_FileName::GetNameExt () const
  {
  return ct_String (GetNameStr (), GetNameExtLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetDrive
(
const char * pc
)
  {
  SetDrive (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetDrive
(
const ct_String & co
)
  {
  SetDrive (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetPath
(
const char * pc
)
  {
  SetPath (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetPath
(
const ct_String & co
)
  {
  SetPath (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetDrivePath
(
const char * pc
)
  {
  SetDrivePath (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetDrivePath
(
const ct_String & co
)
  {
  SetDrivePath (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetName
(
const char * pc
)
  {
  SetName (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetName
(
const ct_String & co
)
  {
  SetName (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetExt
(
const char * pc
)
  {
  SetExt (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetExt
(
const ct_String & co
)
  {
  SetExt (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetNameExt
(
const char * pc
)
  {
  SetNameExt (pc, tl_StringLength (pc));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::SetNameExt
(
const ct_String & co
)
  {
  SetNameExt (co. GetStr (), co. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyDriveFrom
(
const ct_FileName * pco_copy
)
  {
  SetDrive (pco_copy-> GetDriveStr (), pco_copy-> GetDriveLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyPathFrom
(
const ct_FileName * pco_copy
)
  {
  SetPath (pco_copy-> GetPathStr (), pco_copy-> GetPathLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyDrivePathFrom
(
const ct_FileName * pco_copy
)
  {
  SetDrivePath (pco_copy-> GetDriveStr (), pco_copy-> GetDrivePathLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyNameFrom
(
const ct_FileName * pco_copy
)
  {
  SetName (pco_copy-> GetNameStr (), pco_copy-> GetNameLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyExtFrom
(
const ct_FileName * pco_copy
)
  {
  SetExt (pco_copy-> GetExtStr (), pco_copy-> GetExtLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::CopyNameExtFrom
(
const ct_FileName * pco_copy
)
  {
  SetNameExt (pco_copy-> GetNameStr (), pco_copy-> GetNameExtLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::InsertPath
(
const char * pc_path
)
  {
  InsertPath (pc_path, tl_StringLength (pc_path));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::InsertPath
(
const ct_String & co_path
)
  {
  InsertPath (co_path. GetStr (), co_path. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::InsertDrivePath
(
const char * pc_path
)
  {
  InsertDrivePath (pc_path, tl_StringLength (pc_path));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::InsertDrivePath
(
const ct_String & co_path
)
  {
  InsertDrivePath (co_path. GetStr (), co_path. GetLen ());
  }

//---------------------------------------------------------------------------

inline void ct_FileName::AppendPath
(
const char * pc_path
)
  {
  AppendPath (pc_path, tl_StringLength (pc_path));
  }

//---------------------------------------------------------------------------

inline void ct_FileName::AppendPath
(
const ct_String & co_path
)
  {
  AppendPath (co_path. GetStr (), co_path. GetLen ());
  }

#endif
