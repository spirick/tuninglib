
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
// File sourcedep/filemap.cpp

#include "sourcedep/filemap.hpp"
#include "tuning/file.hpp"
#include "sourcedep/dirmap.hpp"

//---------------------------------------------------------------------------

ct_FileMap::ct_FileMap
(
ct_DirMapEntry * pco_init,
ct_DirMap * pco_dirMap
)
  {
  co_DirInfo  = * pco_init;
  pco_DirMap  = pco_dirMap;
  o_Self      = 0;
  u_LoadRef   = 0;
  b_InProject = false;
  }

//---------------------------------------------------------------------------

void ct_FileMap::QueryName
(
ct_FileName * pco_name
) const
  {
  pco_name-> SetDrivePath (pco_DirMap-> GetPath ());
  pco_name-> SetNameExt (co_DirInfo. GetName ());
  }

//---------------------------------------------------------------------------

void ct_FileMap::QueryAbsRelName
(
ct_FileName * pco_name,
const ct_String co_abs
) const
  {
  pco_name-> SetDrivePath (pco_DirMap-> GetPath ());
  pco_name-> SetNameExt (co_DirInfo. GetName ());

  if (pco_DirMap-> IsRel ())
    pco_name-> ToRel (co_abs. GetStr ());
  }

//---------------------------------------------------------------------------

bool ct_FileMap::IncLoadRef ()
  {
  if (u_LoadRef != 0)
    u_LoadRef ++;
  else
    {
    ct_File co_file;
    QueryName (& co_file);

    if (co_file. Open ())
      {
      co_TextBlock. SetCharSize ((t_TextBlock::t_Size) co_DirInfo. GetFileSize ());

      if (co_file. Read (co_TextBlock. GetCharAddr (),
                         co_TextBlock. GetCharSize ()))
        u_LoadRef ++;
      else
        co_TextBlock. SetCharSize (0);

      co_file. Close ();
      }
    }

  return u_LoadRef != 0;
  }

//---------------------------------------------------------------------------

void ct_FileMap::DecLoadRef ()
  {
  TL_ASSERT (u_LoadRef != 0);

  u_LoadRef --;

  if (u_LoadRef == 0)
    co_TextBlock. SetCharSize (0);
  }
