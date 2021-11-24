
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
// File sourcedep/filemap.hpp

#ifndef SOURCEDEP_FILEMAP_HPP
#define SOURCEDEP_FILEMAP_HPP

#include "tuning/charblock.h"
#include "tuning/chn/block.h"
#include "sourcedep/globtyps.hpp"
#include "sourcedep/dirmapen.hpp"

//---------------------------------------------------------------------------

const unsigned o_FileMapInternalChange = 0;
const unsigned o_FileMapExternalChange = 1;
const unsigned o_FileMapExternalDelete = 2;
const unsigned o_FileMapEndOfLoad      = 3;

class TL_EXPORT ct_DirMap;
class TL_EXPORT ct_FileMaps;

class TL_EXPORT ct_FileMap
  {
  friend class TL_EXPORT ct_DirMap;
  friend class TL_EXPORT ct_FileMaps;

  ct_DirMapEntry       co_DirInfo;
  ct_DirMap *          pco_DirMap;
  t_FileMapPtr         o_Self;

  unsigned             u_LoadRef;
  typedef gct_CharBlock <ct_Chn_Block, char> t_TextBlock;
  t_TextBlock          co_TextBlock;
  bool                 b_InProject;

                       ct_FileMap (ct_DirMapEntry * pco_init,
                         ct_DirMap * pco_dirMap);

  inline void          SetFileMapPtr (t_FileMapPtr o_ptr);

public:
  inline const ct_DirMapEntry * GetDirInfo () const;
  inline t_FileMapPtr  GetFileMapPtr () const;
  inline ct_DirMap *   GetDirMap () const;
  inline bool          IsEmpty () const;
  void                 QueryName (ct_FileName * pco_name) const;
  void                 QueryAbsRelName (ct_FileName * pco_name,
                         const ct_String co_abs) const;
  inline void          SetInProject ();
  inline bool          InProject () const;

  bool                 IncLoadRef ();
  void                 DecLoadRef ();

  inline const char *  GetTextBegin () const;
  inline const char *  GetTextEnd () const;
  };

//---------------------------------------------------------------------------

inline void ct_FileMap::SetFileMapPtr
(
t_FileMapPtr o_ptr
)
  {
  o_Self = o_ptr;
  }

//---------------------------------------------------------------------------

inline const ct_DirMapEntry * ct_FileMap::GetDirInfo () const
  {
  return & co_DirInfo;
  }

//---------------------------------------------------------------------------

inline t_FileMapPtr ct_FileMap::GetFileMapPtr () const
  {
  return o_Self;
  }

//---------------------------------------------------------------------------

inline ct_DirMap * ct_FileMap::GetDirMap () const
  {
  return pco_DirMap;
  }

//---------------------------------------------------------------------------

inline bool ct_FileMap::IsEmpty () const
  {
  return (u_LoadRef != 0) ? (co_TextBlock. GetCharSize () == 0) :
                            (co_DirInfo. GetFileSize () == 0);
  }

//---------------------------------------------------------------------------

inline void ct_FileMap::SetInProject ()
  {
  b_InProject = true;
  }

//---------------------------------------------------------------------------

inline bool ct_FileMap::InProject () const
  {
  return b_InProject;
  }

//---------------------------------------------------------------------------

inline const char * ct_FileMap::GetTextBegin () const
  {
  return co_TextBlock. GetCharAddr ();
  }

//---------------------------------------------------------------------------

inline const char * ct_FileMap::GetTextEnd () const
  {
  return co_TextBlock. GetCharAddr () + co_TextBlock. GetCharSize ();
  }

#endif
