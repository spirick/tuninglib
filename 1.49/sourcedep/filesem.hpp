
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
// File sourcedep/filesem.hpp

#ifndef SOURCEDEP_FILESEM_HPP
#define SOURCEDEP_FILESEM_HPP

#include "tuning/chn/array.h"
#include "tuning/compcontainer.h"
#include "sourcedep/globtyps.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_FileMap;

class TL_EXPORT ct_CppFileSemantic
  {
  friend class TL_EXPORT ct_CppProject;
  friend class TL_EXPORT ct_CppParser;

  int                  i_IncCount;
  t_FileMapPtr         o_FileMapPtr;
  ct_FileMap *         pco_FileMap;
  t_CppModulePtr       o_ModulePtr;
  gct_CompContainer <gct_Chn32Array <ct_CppFileSemantic *> > co_Includes;
  gct_CompContainer <gct_Chn32Array <ct_CppFileSemantic *> > co_Includings;
  bool                 b_ParseOk;
  bool                 b_InStack;
  bool                 b_InProject;

                       ct_CppFileSemantic (t_FileMapPtr o_fileMapPtr);
                       ~ct_CppFileSemantic ();

  inline bool          InStack () const;
  inline void          SetInStack (bool b);
  void                 SetInProject ();

  void                 Clear ();
  void                 SetModulePtr (t_CppModulePtr o_mptr);
  void                 AddInclude (ct_CppFileSemantic * pco_fsem);
  void                 AddIncluding (ct_CppFileSemantic * pco_fsem);
  void                 SetParseOk ();

public:
  inline int           GetIncCount () const;
  inline void          SetIncCount (int i);

  inline t_FileMapPtr  GetFileMapPtr () const;
  inline ct_FileMap *  GetFileMap () const;
  inline t_CppModulePtr GetModulePtr () const;
  inline bool          ParseOk () const;

  inline int           FirstInclude () const;
  inline int           NextInclude (int i) const;
  inline ct_CppFileSemantic * GetInclude (int i) const;

  inline int           FirstIncluding () const;
  inline int           NextIncluding (int i) const;
  inline ct_CppFileSemantic * GetIncluding (int i) const;
  };

//---------------------------------------------------------------------------

inline int ct_CppFileSemantic::GetIncCount () const
  {
  return i_IncCount;
  }

//---------------------------------------------------------------------------

inline void ct_CppFileSemantic::SetIncCount
(
int i
)
  {
  i_IncCount = i;
  }

//---------------------------------------------------------------------------

inline bool ct_CppFileSemantic::InStack () const
  {
  return b_InStack;
  }

//---------------------------------------------------------------------------

inline void ct_CppFileSemantic::SetInStack
(
bool b
)
  {
  b_InStack = b;
  }

//---------------------------------------------------------------------------

inline t_FileMapPtr ct_CppFileSemantic::GetFileMapPtr () const
  {
  return o_FileMapPtr;
  }

//---------------------------------------------------------------------------

inline ct_FileMap * ct_CppFileSemantic::GetFileMap () const
  {
  return pco_FileMap;
  }

//---------------------------------------------------------------------------

inline t_CppModulePtr ct_CppFileSemantic::GetModulePtr () const
  {
  return o_ModulePtr;
  }

//---------------------------------------------------------------------------

inline bool ct_CppFileSemantic::ParseOk () const
  {
  return b_ParseOk;
  }

//---------------------------------------------------------------------------

inline int ct_CppFileSemantic::FirstInclude () const
  {
  return co_Includes. First ();
  }

//---------------------------------------------------------------------------

inline int ct_CppFileSemantic::NextInclude
(
int i
) const
  {
  return co_Includes. Next (i);
  }

//---------------------------------------------------------------------------

inline ct_CppFileSemantic * ct_CppFileSemantic::GetInclude
(
int i
) const
  {
  return * co_Includes. GetObj (i);
  }

//---------------------------------------------------------------------------

inline int ct_CppFileSemantic::FirstIncluding () const
  {
  return co_Includings. First ();
  }

//---------------------------------------------------------------------------

inline int ct_CppFileSemantic::NextIncluding
(
int i
) const
  {
  return co_Includings. Next (i);
  }

//---------------------------------------------------------------------------

inline ct_CppFileSemantic * ct_CppFileSemantic::GetIncluding
(
int i
) const
  {
  return * co_Includings. GetObj (i);
  }

#endif
