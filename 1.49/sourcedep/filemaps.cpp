
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
// File sourcedep/filemaps.cpp

#include "sourcedep/filemaps.hpp"

//---------------------------------------------------------------------------

static ct_FileMaps * pco_FileMaps = 0;

t_PtrToFileMaps TL_EXPORT FileMaps ()
  {
  if (pco_FileMaps == 0)
    pco_FileMaps = new ct_FileMaps;

  return pco_FileMaps;
  }

//---------------------------------------------------------------------------

bool ct_FileMaps::CanDelete
(
t_FileMapPtr o_ptr
) const
  {
  return (co_FileMaps. GetPtr (o_ptr)-> u_LoadRef == 0);
  }

//---------------------------------------------------------------------------

t_FileMapPtr ct_FileMaps::QueryCreateSerialFile
(
const ct_FileName * pco_name,
bool b_rel
)
  {
  TL_ASSERT (pco_name-> HasPath ());
  TL_ASSERT (pco_name-> HasName ());

  ct_String co_path (pco_name-> GetDriveStr (),
                     pco_name-> GetPureDrivePathLen ());

  t_DirMapPtr o_dirMapPtr = DirMaps ()-> QueryCreateDirMap (co_path, b_rel);
  ct_DirMap * pco_dirMap = DirMaps ()-> GetDirMap (o_dirMapPtr);

  t_FileMapPtr o_fileMapPtr =
    pco_dirMap-> SearchName (pco_name-> GetNameStr ());

  return o_fileMapPtr;
  }
