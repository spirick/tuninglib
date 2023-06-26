
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
// File sourcedep/dirmaps.hpp

#ifndef SOURCEDEP_DIRMAPS_HPP
#define SOURCEDEP_DIRMAPS_HPP

#include "tuning/chn/blockptrdlist.h"
#include "sourcedep/dirmap.hpp"

//---------------------------------------------------------------------------

typedef gct_Chn16BlockPtrDList <ct_DirMap> t_DirMaps;

class TL_EXPORT ct_DirMaps
  {
  friend class TL_EXPORT ct_FileMaps;

  t_DirMaps            co_DirMaps;

public:
  inline const t_DirMaps * GetDirMaps () const;
  inline ct_DirMap *   GetDirMap (t_DirMapPtr o_ptr) const;

  t_DirMapPtr          QueryCreateDirMap (const ct_String & co_path, bool b_rel);
  };

//---------------------------------------------------------------------------

typedef ct_DirMaps * t_PtrToDirMaps;
t_PtrToDirMaps TL_EXPORT DirMaps ();

//---------------------------------------------------------------------------

inline const t_DirMaps * ct_DirMaps::GetDirMaps () const
  {
  return & co_DirMaps;
  }

//---------------------------------------------------------------------------

inline ct_DirMap * ct_DirMaps::GetDirMap
(
t_DirMapPtr o_ptr
) const
  {
  return co_DirMaps. GetPtr (o_ptr);
  }

#endif
