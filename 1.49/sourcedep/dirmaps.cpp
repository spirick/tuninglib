
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
// File sourcedep/dirmaps.cpp

#include "sourcedep/dirmaps.hpp"

//---------------------------------------------------------------------------

static ct_DirMaps * pco_DirMaps = 0;

t_PtrToDirMaps TL_EXPORT DirMaps ()
  {
  if (pco_DirMaps == 0)
    pco_DirMaps = new ct_DirMaps;

  return pco_DirMaps;
  }

//---------------------------------------------------------------------------

t_DirMapPtr ct_DirMaps::QueryCreateDirMap
(
const ct_String & co_path,
bool b_rel
)
  {
  t_DirMapPtr o_dptr;

  for (o_dptr = co_DirMaps. First ();
       o_dptr != 0; o_dptr = co_DirMaps. Next (o_dptr))
    if (* co_DirMaps. GetPtr (o_dptr) == co_path)
      return o_dptr;

  o_dptr = co_DirMaps. AddObj ();
  * co_DirMaps. GetObj (o_dptr) = new ct_DirMap (co_path, o_dptr, b_rel);
  return o_dptr;
  }
