
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
// File sourcedep/dirmapen.hpp

#ifndef SOURCEDEP_DIRMAPEN_HPP
#define SOURCEDEP_DIRMAPEN_HPP

#include "tuning/dirscan.hpp"
#include "tuning/sys/ctimedate.hpp"

//---------------------------------------------------------------------------

typedef unsigned char t_DirMapFlag;

const t_DirMapFlag o_DirMapExists    = 0x01;
const t_DirMapFlag o_DirMapArchive   = 0x02;
const t_DirMapFlag o_DirMapDirectory = 0x04;
const t_DirMapFlag o_DirMapHidden    = 0x08;
const t_DirMapFlag o_DirMapReadOnly  = 0x10;
const t_DirMapFlag o_DirMapSystem    = 0x20;

class TL_EXPORT ct_DirMapEntry
  {
  friend class TL_EXPORT ct_DirMap;

  ct_String            co_Name;
  t_FileSize           o_Size;
  t_DirMapFlag         o_Flags;
  t_MicroTime          o_LastWriteTime;

  inline void          SetName (const char * pc_name);
  inline void          SetFileSize (t_FileSize o_fs);
  inline void          SetFlags (t_DirMapFlag o_flags);
  inline void          AddFlag (t_DirMapFlag o_flag);
  inline void          Clear ();

public:
  inline               ct_DirMapEntry ();

  inline const char *  GetName () const;
  inline const ct_String * GetNameObj () const;

  inline t_FileSize    GetFileSize () const;

  inline t_DirMapFlag  GetFlags () const;
  inline bool          Exists () const;
  inline bool          IsArchive () const;
  inline bool          IsDirectory () const;
  inline bool          IsHidden () const;
  inline bool          IsReadOnly () const;
  inline bool          IsSystem () const;

  inline int           CompNameTo (const ct_DirMapEntry & co_comp) const;
  inline bool          SpecEqualTo (const ct_DirMapEntry & co_comp) const;
  };

//---------------------------------------------------------------------------

inline void ct_DirMapEntry::SetName
(
const char * pc_name
)
  {
  co_Name = pc_name;

  #ifndef TL_UNIX
    co_Name. ToLower ();
  #endif
  }

//---------------------------------------------------------------------------

inline void ct_DirMapEntry::SetFileSize
(
t_FileSize o_fs
)
  {
  o_Size = o_fs;
  }

//---------------------------------------------------------------------------

inline void ct_DirMapEntry::SetFlags
(
t_DirMapFlag o_flags
)
  {
  o_Flags = o_flags;
  }

//---------------------------------------------------------------------------

inline void ct_DirMapEntry::AddFlag
(
t_DirMapFlag o_flag
)
  {
  o_Flags |= o_flag;
  }

//---------------------------------------------------------------------------

inline void ct_DirMapEntry::Clear ()
  {
  o_Size = 0;
  o_Flags = 0;
  o_LastWriteTime = 0;
  }

//---------------------------------------------------------------------------

inline ct_DirMapEntry::ct_DirMapEntry ()
  {
  o_Flags = 0;
  }

//---------------------------------------------------------------------------

inline const char * ct_DirMapEntry::GetName () const
  {
  return co_Name. GetStr ();
  }

//---------------------------------------------------------------------------

inline const ct_String * ct_DirMapEntry::GetNameObj () const
  {
  return & co_Name;
  }

//---------------------------------------------------------------------------

inline t_FileSize ct_DirMapEntry::GetFileSize () const
  {
  return o_Size;
  }

//---------------------------------------------------------------------------

inline t_DirMapFlag ct_DirMapEntry::GetFlags () const
  {
  return o_Flags;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::Exists () const
  {
  return o_Flags & o_DirMapExists;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::IsArchive () const
  {
  return (o_Flags & o_DirMapArchive) != 0;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::IsDirectory () const
  {
  return (o_Flags & o_DirMapDirectory) != 0;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::IsHidden () const
  {
  return (o_Flags & o_DirMapHidden) != 0;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::IsReadOnly () const
  {
  return (o_Flags & o_DirMapReadOnly) != 0;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::IsSystem () const
  {
  return (o_Flags & o_DirMapSystem) != 0;
  }

//---------------------------------------------------------------------------

inline int ct_DirMapEntry::CompNameTo
(
const ct_DirMapEntry & co_comp
) const
  {
  return co_Name. CompTo (co_comp. co_Name);
  }

//---------------------------------------------------------------------------

inline bool ct_DirMapEntry::SpecEqualTo
(
const ct_DirMapEntry & co_comp
) const
  {
  return
    (o_Size          == co_comp. o_Size) &&
    (o_Flags         == co_comp. o_Flags) &&
    (o_LastWriteTime == co_comp. o_LastWriteTime);
  }

#endif
