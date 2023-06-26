
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
// File sourcedep/dirmap.cpp

#include "sourcedep/dirmap.hpp"
#include "tuning/chn/array.h"
#include "tuning/stringsort.hpp"
#include "sourcedep/filemaps.hpp"

//---------------------------------------------------------------------------

ct_DirMap::ct_DirMap
(
const ct_String & co_path,
t_DirMapPtr o_self,
bool b_rel
)
  {
  co_Path = co_path;
  o_Self = o_self;
  b_Rel = b_rel;
  b_InProject = false;
  ct_DirScan co_dirScan;
  gct_Chn_Array <ct_DirMapEntry> co_dirMapArray;
  gct_Chn_Array <const char *> co_strArray;
  ct_DirMapEntry * pco_dirMapEntry = 0;
  t_Int * pi_sortIdx = 0;
  co_dirScan. AssignAsPath (co_Path. GetStr ());

  if (co_dirScan. GetPathLen () == 1)
    b_Exists = true; // Root dir
  else
    {
    co_dirScan. AssignAsName (co_Path. GetStr ());
    b_Exists = co_dirScan. FindFirst ();

    if (b_Exists)
      co_dirScan. AbortFind ();
    }

  if (b_Exists)
    {
    co_dirScan. AssignAsPath (co_Path. GetStr ());
    co_dirScan. SetNameExt ("*");

    if (co_dirScan. FindFirst ())
      {
      do
        if (! co_dirScan. IsDirectory ())
          {
          pco_dirMapEntry = co_dirMapArray. GetObj (co_dirMapArray. AddObj ());
          pco_dirMapEntry-> SetName (co_dirScan. GetNameStr ());
          pco_dirMapEntry-> SetFileSize (co_dirScan. GetSize ());
          pco_dirMapEntry-> AddFlag (o_DirMapExists);

          if (co_dirScan. IsArchive ())
            pco_dirMapEntry-> AddFlag (o_DirMapArchive);

          if (co_dirScan. IsHidden ())
            pco_dirMapEntry-> AddFlag (o_DirMapHidden);

          if (co_dirScan. IsReadOnly ())
            pco_dirMapEntry-> AddFlag (o_DirMapReadOnly);

          if (co_dirScan. IsSystem ())
            pco_dirMapEntry-> AddFlag (o_DirMapSystem);

          pco_dirMapEntry-> o_LastWriteTime = co_dirScan. GetLastWriteTime ();
          * co_strArray. GetObj (co_strArray. AddObj ()) =
            pco_dirMapEntry-> GetName ();
          }
      while (co_dirScan. FindNext ());

      ct_StringSort co_sort;
      pi_sortIdx = new t_Int [co_strArray. GetLen ()];

      if (co_strArray. GetLen () > 0)
        co_sort. Sort (co_strArray. GetFirstObj (), pi_sortIdx,
          co_strArray. GetLen ());
      }
    }

  int i_comp;
  unsigned u_sort = 0;
  t_FileMapPtrs::t_Position o_fileMapPtrPtr = co_FileMapPtrs. First ();
  t_FileMapPtr o_fileMapPtr = 0;
  ct_FileMap * pco_fileMap = 0;
  t_FileMaps * const pco_fileMaps = & FileMaps ()-> co_FileMaps;

  for (;;)
    {
    if (u_sort < co_dirMapArray. GetLen ())
      {
      pco_dirMapEntry = co_dirMapArray. GetNthObj (pi_sortIdx [u_sort] + 1);

      if (o_fileMapPtrPtr != 0)
        {
        o_fileMapPtr = * co_FileMapPtrs. GetObj (o_fileMapPtrPtr);
        pco_fileMap = pco_fileMaps-> GetPtr (o_fileMapPtr);
        i_comp = pco_dirMapEntry-> CompNameTo (* pco_fileMap-> GetDirInfo ());
        }
      else
        i_comp = -1;
      }
    else
      if (o_fileMapPtrPtr != 0)
        {
        o_fileMapPtr = * co_FileMapPtrs. GetObj (o_fileMapPtrPtr);
        pco_fileMap = pco_fileMaps-> GetPtr (o_fileMapPtr);
        i_comp = 1;
        }
      else
        break;

    if (i_comp < 0)
      {
      TL_ASSERT (u_sort < co_dirMapArray. GetLen ());

      ct_FileMap * pco_newFileMap = new ct_FileMap (pco_dirMapEntry, this);
      t_FileMapPtr o_newFileMapPtr = pco_fileMaps-> AddPtr (pco_newFileMap);
      pco_newFileMap-> SetFileMapPtr (o_newFileMapPtr);

      t_FileMapPtrs::t_Position o_newFileMapPtrPtr =
        co_FileMapPtrs. AddObjBefore (o_fileMapPtrPtr);
      * co_FileMapPtrs. GetObj (o_newFileMapPtrPtr) = o_newFileMapPtr;
      o_fileMapPtrPtr = co_FileMapPtrs. Next (o_newFileMapPtrPtr);

      u_sort ++;
      }
    else
      if (i_comp > 0)
        {
        TL_ASSERT (o_fileMapPtrPtr != 0);

        if (FileMaps ()-> CanDelete (o_fileMapPtr))
          {
          pco_fileMaps-> DelPtrAndObj (o_fileMapPtr);
          o_fileMapPtrPtr = co_FileMapPtrs. DelObj (o_fileMapPtrPtr);
          }
        else
          {
          pco_fileMap-> co_DirInfo. Clear ();

          o_fileMapPtrPtr = co_FileMapPtrs. Next (o_fileMapPtrPtr);
          }
        }
      else
        {
        TL_ASSERT (u_sort < co_dirMapArray. GetLen ());
        TL_ASSERT (o_fileMapPtrPtr != 0);
        TL_ASSERT (pco_fileMap-> GetDirMap () == this);

        if (! pco_dirMapEntry-> SpecEqualTo (* pco_fileMap-> GetDirInfo ()))
          {
          pco_fileMap-> co_DirInfo = * pco_dirMapEntry;
          }

        u_sort ++;
        o_fileMapPtrPtr = co_FileMapPtrs. Next (o_fileMapPtrPtr);
        }
    }

  delete [] pi_sortIdx;
  }

//---------------------------------------------------------------------------
// Assumes array type of co_FileMapPtrs

t_FileMapPtr ct_DirMap::SearchName
(
const char * pc_name
) const
  {
  if (co_FileMapPtrs. GetLen () == 0)
    return 0;
  else
    {
    t_FileMaps * const pco_fileMaps = & FileMaps ()-> co_FileMaps;
    t_FileMapPtr * po_min = co_FileMapPtrs. GetObj (1);
    int i_comp = pco_fileMaps->
      GetPtr (* po_min)-> GetDirInfo ()-> GetNameObj ()-> CompTo (pc_name);

    if (co_FileMapPtrs. GetLen () == 1)
      {
      if (i_comp == 0)
        return * po_min;
      else
        return 0;
      }
    else
      {
      if (i_comp > 0)
        return 0;

      if (i_comp == 0)
        return * po_min;

      t_FileMapPtr * po_max = co_FileMapPtrs. GetObj (co_FileMapPtrs. Last ());

      i_comp = pco_fileMaps->
        GetPtr (* po_max)-> GetDirInfo ()-> GetNameObj ()-> CompTo (pc_name);

      if (i_comp < 0)
        return 0;

      if (i_comp == 0)
        return * po_max;

      for (;;)
        {
        t_FileMapPtr * po_mid = po_min + ((po_max - po_min) / 2);

        if ((po_mid == po_min) || (po_mid == po_max))
          return 0;

        i_comp = pco_fileMaps->
          GetPtr (* po_mid)-> GetDirInfo ()-> GetNameObj ()-> CompTo (pc_name);

        if (i_comp == 0)
          return * po_mid;

        if (i_comp < 0)
          po_min = po_mid;
        else
          po_max = po_mid;
        }
      }
    }
  }
