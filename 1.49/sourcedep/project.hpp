
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
// File sourcedep/project.hpp

#ifndef SOURCEDEP_PROJECT_HPP
#define SOURCEDEP_PROJECT_HPP

#include "tuning/filename.hpp"
#include "tuning/string.hpp"
#include "tuning/chn/array.h"
#include "sourcedep/module.hpp"
#include "sourcedep/parser.hpp"

//---------------------------------------------------------------------------

typedef unsigned t_IncludePtr;
typedef gct_Chn32Array <ct_CppModule> t_CppModules;

class TL_EXPORT ct_CppProject
  {
  friend class TL_EXPORT ct_CppParser;

  bool                 b_Messages;
  ct_FileName          co_FileName;

  ct_String            co_IncludeStr;
  gct_Chn32Array <ct_String> co_IncludeDirs;
  gct_Chn32Array <bool>      co_IncludeRels;

  t_CppModules         co_Modules;

  void                 SetFileMapPtr (t_CppModulePtr o_mptr,
                         t_FileMapPtr o_fileMapPtr);

  gct_Chn32Array <void *> co_FileSemantics;

  inline ct_CppFileSemantic * * GetPtr (t_FileMapPtr o_ptr);
  ct_CppFileSemantic * QueryCreateFileSemantic (t_FileMapPtr o_ptr);

  ct_CppParser         co_Parser;

public:
                       ct_CppProject (const char * pc_name);

  inline bool                  Messages () const    { return b_Messages; }
  inline void                  SetMessages (bool b) { b_Messages = b; }
  inline const ct_FileName *   GetFileName () const;

  inline t_IncludePtr  FirstIncludeDir () const;
  inline t_IncludePtr  NextIncludeDir (t_IncludePtr o_ptr) const;
  inline const char *  GetIncludeDir (t_IncludePtr o_ptr) const;
  inline bool          GetIncludeRel (t_IncludePtr o_ptr) const;
  inline t_UInt        GetIncludeDirLen (t_IncludePtr o_ptr) const;
  void                 SetIncludeStr (const char * pc_include);

  inline t_CppModulePtr       FirstModulePtr () const;
  inline t_CppModulePtr       NextModulePtr (t_CppModulePtr o_mptr) const;
  inline const ct_FileName *  GetUnifiedName (t_CppModulePtr o_mptr) const;
  inline t_FileMapPtr         GetFileMapPtr (t_CppModulePtr o_mptr) const;

  t_CppModulePtr       AddModuleBefore (t_CppModulePtr o_mptr,
                         const char * pc_name);

  ct_CppFileSemantic * QueryFileSemantic (t_FileMapPtr o_ptr);

  inline ct_CppParser *        GetParser ();

  void                 UnifyFileName (ct_FileName * pco_name);
  };

//---------------------------------------------------------------------------

inline const ct_FileName * ct_CppProject::GetFileName () const
  {
  return & co_FileName;
  }

//---------------------------------------------------------------------------

inline t_IncludePtr ct_CppProject::FirstIncludeDir () const
  {
  return co_IncludeDirs. First ();
  }

//---------------------------------------------------------------------------

inline t_IncludePtr ct_CppProject::NextIncludeDir
(
t_IncludePtr o_ptr
) const
  {
  return co_IncludeDirs. Next (o_ptr);
  }

//---------------------------------------------------------------------------

inline const char * ct_CppProject::GetIncludeDir
(
t_IncludePtr o_ptr
) const
  {
  return co_IncludeDirs. GetObj (o_ptr)-> GetStr ();
  }

//---------------------------------------------------------------------------

inline bool ct_CppProject::GetIncludeRel
(
t_IncludePtr o_ptr
) const
  {
  return * co_IncludeRels. GetObj (o_ptr);
  }

//---------------------------------------------------------------------------

inline t_UInt ct_CppProject::GetIncludeDirLen
(
t_IncludePtr o_ptr
) const
  {
  return co_IncludeDirs. GetObj (o_ptr)-> GetLen ();
  }

//---------------------------------------------------------------------------

inline t_CppModulePtr ct_CppProject::FirstModulePtr () const
  {
  return co_Modules. First ();
  }

//---------------------------------------------------------------------------

inline t_CppModulePtr ct_CppProject::NextModulePtr
(
t_CppModulePtr o_mptr
) const
  {
  return co_Modules. Next (o_mptr);
  }

//---------------------------------------------------------------------------

inline const ct_FileName * ct_CppProject::GetUnifiedName
(
t_CppModulePtr o_mptr
) const
  {
  return & co_Modules. GetObj (o_mptr)-> co_UnifiedName;
  }

//---------------------------------------------------------------------------

inline t_FileMapPtr ct_CppProject::GetFileMapPtr
(
t_CppModulePtr o_mptr
) const
  {
  return co_Modules. GetObj (o_mptr)-> o_FileMapPtr;
  }

//---------------------------------------------------------------------------

inline ct_CppParser * ct_CppProject::GetParser ()
  {
  return & co_Parser;
  }

#endif
