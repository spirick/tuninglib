
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
// File sourcedep/filesem.cpp

#include "sourcedep/filesem.hpp"
#include "sourcedep/filemaps.hpp"
#include "sourcedep/parser.hpp"

//---------------------------------------------------------------------------

ct_CppFileSemantic::ct_CppFileSemantic
(
t_FileMapPtr o_fileMapPtr
)
  {
  i_IncCount = -1;
  o_FileMapPtr = o_fileMapPtr;
  pco_FileMap = FileMaps ()-> GetFileMap (o_fileMapPtr);
  o_ModulePtr = 0;
  b_ParseOk = false;
  b_InStack = false;
  }

//---------------------------------------------------------------------------

ct_CppFileSemantic::~ct_CppFileSemantic ()
  {
  Clear ();

  ct_CppFileSemantic * pco_this = this;

  for (unsigned u_inc = co_Includings. First ();
       u_inc != 0; u_inc = co_Includings. Next (u_inc))
    (* co_Includings. GetObj (u_inc))->
      co_Includes. DelFirstEqualObj (& pco_this);

  co_Includings. FreeAll ();
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::SetInProject ()
  {
  if (! b_InProject)
    {
    b_InProject = true;

    for (unsigned u_inc = co_Includes. First ();
         u_inc != 0; u_inc = co_Includes. Next (u_inc))
      (* co_Includes. GetObj (u_inc))-> SetInProject ();
    }
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::Clear ()
  {
  ct_CppFileSemantic * pco_this = this;

  for (unsigned u_inc = co_Includes. First ();
       u_inc != 0; u_inc = co_Includes. Next (u_inc))
    {
    ct_CppFileSemantic * pco_fsem = * co_Includes. GetObj (u_inc);
    pco_fsem-> co_Includings. DelFirstEqualObj (& pco_this);
    }

  co_Includes. FreeAll ();
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::SetModulePtr
(
t_CppModulePtr o_mptr
)
  {
  o_ModulePtr = o_mptr;
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::AddInclude
(
ct_CppFileSemantic * pco_fsem
)
  {
  TL_ASSERT (pco_fsem != this);

  co_Includes. AddObj (& pco_fsem);
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::AddIncluding
(
ct_CppFileSemantic * pco_fsem
)
  {
  TL_ASSERT (pco_fsem != this);

  co_Includings. AddObj (& pco_fsem);
  }

//---------------------------------------------------------------------------

void ct_CppFileSemantic::SetParseOk ()
  {
  b_ParseOk = true;
  }
