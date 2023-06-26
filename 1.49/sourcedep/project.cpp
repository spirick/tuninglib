
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
// File sourcedep/project.cpp

#include "sourcedep/project.hpp"
#include "tuning/dir.hpp"
#include "sourcedep/filemaps.hpp"

//---------------------------------------------------------------------------

ct_CppProject::ct_CppProject
(
const char * pc_name
):
  b_Messages (false)  
  {
  co_Parser. Init (this);
  co_FileName. AssignAsName (pc_name);

  if ((co_FileName. IsRel ()) || (! co_FileName. HasDrive ()))
    {
    ct_Directory co_dir;
    co_dir. CopyDriveFrom (& co_FileName);
    co_dir. QueryCurrentDriveDirectory ();

    if (co_FileName. IsRel ())
      co_FileName. ToAbs (co_dir. GetAllStr ());
    else
      if (! co_FileName. HasDrive ())
        co_FileName. CopyDriveFrom (& co_dir);
    }

  co_FileName. CompressPath ();

  #ifndef TL_UNIX
    co_FileName. ToLower ();
  #endif
  }

//---------------------------------------------------------------------------

void ct_CppProject::UnifyFileName
(
ct_FileName * pco_name
)
  {
  if ((pco_name-> IsRel ()) || (! pco_name-> HasDrive ()))
    {
    ct_Directory co_dir;

    #ifndef TL_UNIX
      pco_name-> ToLower ();
    #endif

    if ((! pco_name-> HasDrive ()) ||
        (co_FileName. CompSubStr (co_FileName. GetDriveOffs (),
         pco_name-> GetDriveStr (), pco_name-> GetDriveLen ()) == 0))
      co_dir. CopyDrivePathFrom (& co_FileName);
    else
      {
      co_dir. CopyDriveFrom (pco_name);
      co_dir. QueryCurrentDriveDirectory ();
      }

    if (pco_name-> IsRel ())
      pco_name-> ToAbs (co_dir. GetAllStr ());
    else
      if (! pco_name-> HasDrive ())
        pco_name-> CopyDriveFrom (& co_dir);
    }

  pco_name-> CompressPath ();

  #ifndef TL_UNIX
    pco_name-> ToLower ();
  #endif
  }

//---------------------------------------------------------------------------

void ct_CppProject::SetIncludeStr
(
const char * pc_include
)
  {
  ct_String co_str (pc_include);

  co_IncludeStr. Assign (pc_include);
  co_IncludeDirs. DelAll ();
  co_IncludeRels. DelAll ();

  while (co_str. GetLen () > 0)
    {
    t_Int i_newLine = co_str. First ("\n");
    t_Int i_semiColon = co_str. First (";");
    t_Int i_delim;

    if (i_newLine >= 0)
      if (i_semiColon >= 0)
        if (i_newLine > i_semiColon)
          i_delim = i_semiColon;
        else
          i_delim = i_newLine;
      else
        i_delim = i_newLine;
    else
      if (i_semiColon >= 0)
        i_delim = i_semiColon;
      else
        i_delim = co_str. GetLen ();

    ct_String co_dir (co_str. GetStr (), i_delim);

    while ((co_dir. GetLen () > 0) &&
           (co_dir. GetChar (0) <= ' '))
      co_dir. Delete (0, 1);

    while ((co_dir. GetLen () > 0) &&
           (co_dir. GetChar (co_dir. GetLen () - 1) <= ' '))
      co_dir. Delete (co_dir. GetLen () - 1, 1);

    if (co_dir. GetLen () > 0)
      {
      ct_FileName co_name;

      co_name. AssignAsPath (co_dir. GetStr ());
      bool b_rel = co_name. IsRel ();
      UnifyFileName (& co_name);
      co_IncludeDirs. AddObjAfterLast (& co_name);
      co_IncludeRels. AddObjAfterLast (& b_rel);
      }

    if (i_delim < (int) co_str. GetLen ())
      co_str. Delete (0, i_delim + 1);
    else
      co_str. Delete (0);
    }
  }

//---------------------------------------------------------------------------

void ct_CppProject::SetFileMapPtr
(
t_CppModulePtr o_mptr,
t_FileMapPtr o_fileMapPtr
)
  {
  TL_ASSERT (co_Modules. GetObj (o_mptr)-> o_FileMapPtr == 0);
  TL_ASSERT (o_fileMapPtr != 0);

  co_Modules. GetObj (o_mptr)-> o_FileMapPtr = o_fileMapPtr;

  ct_CppFileSemantic * pco_fs = QueryCreateFileSemantic (o_fileMapPtr);

  TL_ASSERT (pco_fs-> GetModulePtr () == 0);

  pco_fs-> SetModulePtr (o_mptr);
  }

//---------------------------------------------------------------------------

t_CppModulePtr ct_CppProject::AddModuleBefore
(
t_CppModulePtr o_mptr,
const char * pc_name
)
  {
  t_CppModulePtr o_newPtr = co_Modules. AddObjBefore (o_mptr);
  ct_CppModule * pco_module = co_Modules. GetObj (o_newPtr);

  pco_module-> co_Name. Assign (pc_name);
  pco_module-> co_UnifiedName. AssignAsName (pc_name);
  UnifyFileName (& pco_module-> co_UnifiedName);
  pco_module-> o_FileMapPtr = 0;

  for (t_CppModulePtr o_ptr = co_Modules. First ();
       o_ptr != 0; o_ptr = co_Modules. Next (o_ptr))
    if ((o_ptr != o_newPtr) &&
        (co_Modules. GetObj (o_ptr)-> co_UnifiedName ==
         pco_module-> co_UnifiedName))
      {
      co_Modules. DelObj (o_newPtr);
      return 0;
      }

  return o_newPtr;
  }

//---------------------------------------------------------------------------

inline ct_CppFileSemantic * * ct_CppProject::GetPtr
(
t_FileMapPtr o_ptr
)
  {
  if (o_ptr > co_FileSemantics. GetLen ())
    {
    void * pv = 0;

    for (t_FileMapPtr o_new = (t_FileMapPtr) (co_FileSemantics. GetLen () + 1);
         o_new <= o_ptr; o_new ++)
      co_FileSemantics. AddObjAfterLast (& pv);
    }

  return (ct_CppFileSemantic * *) co_FileSemantics. GetObj (o_ptr);
  }

//---------------------------------------------------------------------------

ct_CppFileSemantic * ct_CppProject::QueryCreateFileSemantic
(
t_FileMapPtr o_ptr
)
  {
  ct_CppFileSemantic * * ppco_fs = GetPtr (o_ptr);

  if (* ppco_fs == 0)
    * ppco_fs = new ct_CppFileSemantic (o_ptr);

  return * ppco_fs;
  }

//---------------------------------------------------------------------------

ct_CppFileSemantic * ct_CppProject::QueryFileSemantic
(
t_FileMapPtr o_ptr
)
  {
  return * GetPtr (o_ptr);
  }
