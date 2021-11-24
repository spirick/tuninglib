
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
// File sourcedep/module.hpp

#ifndef SOURCEDEP_MODULE_HPP
#define SOURCEDEP_MODULE_HPP

#include "tuning/filename.hpp"
#include "sourcedep/globtyps.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_CppModule
  {
  friend class TL_EXPORT ct_CppProject;

  ct_String            co_Name;
  ct_FileName          co_UnifiedName;
  t_FileMapPtr         o_FileMapPtr;
  };

#endif
