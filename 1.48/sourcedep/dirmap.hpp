
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
// File sourcedep/dirmap.hpp

#ifndef SOURCEDEP_DIRMAP_HPP
#define SOURCEDEP_DIRMAP_HPP

#include "tuning/chn/array.h"
#include "sourcedep/globtyps.hpp"
#include "sourcedep/dirmapen.hpp"

//---------------------------------------------------------------------------

typedef gct_Chn_Array <t_FileMapPtr> t_FileMapPtrs;

class TL_EXPORT ct_DirMap
  {
  friend class TL_EXPORT ct_DirMaps;
  friend class TL_EXPORT ct_FileMaps;

  ct_String            co_Path;
  t_DirMapPtr          o_Self;
  bool                 b_Exists;
  bool                 b_Rel;
  bool                 b_InProject;
  t_FileMapPtrs        co_FileMapPtrs;

                       ct_DirMap (const ct_String & co_path,
                         t_DirMapPtr o_self, bool b_rel);

  t_FileMapPtr         SearchName (const char * pc_name) const;

public:
  inline const char *  GetPath () const;
  inline t_UInt        GetPathLen () const;
  inline t_DirMapPtr   GetDirMapPtr () const;
  inline bool          Exists () const;
  inline bool          IsRel () const;
  inline void          SetInProject ();
  inline bool          InProject () const;
  inline bool          operator == (const ct_String & co_path) const;

  inline const t_FileMapPtrs * GetFileMapPtrs () const;
  };

//---------------------------------------------------------------------------

inline const char * ct_DirMap::GetPath () const
  {
  return co_Path. GetStr ();
  }

//---------------------------------------------------------------------------

inline t_UInt ct_DirMap::GetPathLen () const
  {
  return co_Path. GetLen ();
  }

//---------------------------------------------------------------------------

inline t_DirMapPtr ct_DirMap::GetDirMapPtr () const
  {
  return o_Self;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMap::Exists () const
  {
  return b_Exists;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMap::IsRel () const
  {
  return b_Rel;
  }

//---------------------------------------------------------------------------

inline void ct_DirMap::SetInProject ()
  {
  b_InProject = true;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMap::InProject () const
  {
  return b_InProject;
  }

//---------------------------------------------------------------------------

inline bool ct_DirMap::operator ==
(
const ct_String & co_path
) const
  {
  return co_Path == co_path;
  }

//---------------------------------------------------------------------------

inline const t_FileMapPtrs * ct_DirMap::GetFileMapPtrs () const
  {
  return & co_FileMapPtrs;
  }

#endif
