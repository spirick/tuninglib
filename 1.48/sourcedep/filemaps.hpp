
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
// File sourcedep/filemaps.hpp

#ifndef SOURCEDEP_FILEMAPS_HPP
#define SOURCEDEP_FILEMAPS_HPP

#include "tuning/chn/blockptrdlist.h"
#include "sourcedep/dirmaps.hpp"
#include "sourcedep/filemap.hpp"

//---------------------------------------------------------------------------

typedef gct_Chn16BlockPtrDList <ct_FileMap> t_FileMaps;

class TL_EXPORT ct_FileMaps
  {
  friend class TL_EXPORT ct_DirMap;

  t_FileMaps           co_FileMaps;

public:
  inline const t_FileMaps * GetFileMaps () const;
  inline ct_FileMap *  GetFileMap (t_FileMapPtr o_ptr) const;
  bool                 CanDelete (t_FileMapPtr o_ptr) const;

  // Assumes pco_name to be full qualified and unified
  t_FileMapPtr         QueryCreateSerialFile (const ct_FileName * pco_name, bool b_rel);
  };

//---------------------------------------------------------------------------

typedef ct_FileMaps * t_PtrToFileMaps;
t_PtrToFileMaps TL_EXPORT FileMaps ();

//---------------------------------------------------------------------------

inline const t_FileMaps * ct_FileMaps::GetFileMaps () const
  {
  return & co_FileMaps;
  }

//---------------------------------------------------------------------------

inline ct_FileMap * ct_FileMaps::GetFileMap
(
t_FileMapPtr o_ptr
) const
  {
  return co_FileMaps. GetPtr (o_ptr);
  }

#endif
