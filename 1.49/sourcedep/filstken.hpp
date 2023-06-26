
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
// File sourcedep/filstken.hpp

#ifndef SOURCEDEP_FILSTKEN_HPP
#define SOURCEDEP_FILSTKEN_HPP

#include "tuning/chn/array.h"
#include "sourcedep/scanner.hpp"
#include "sourcedep/filesem.hpp"

//===========================================================================

enum et_CppFileStackStates
  {
  ec_CppFileStackFirstScan,
  ec_CppFileStackCommand
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_CppFileStackEntry
  {
  ct_CppScanner        co_Scanner;
  ct_CppFileSemantic * pco_FileSemantic;
  et_CppFileStackStates eo_State;
  bool                 b_LoadOk;

public:
  inline ct_CppScanner * GetScanner ();

  inline ct_CppFileSemantic * GetFileSemantic () const;
  inline void          SetFileSemantic (ct_CppFileSemantic * pco_fsem);

  inline et_CppFileStackStates GetState () const;
  inline void          SetState (et_CppFileStackStates eo_state);

  inline bool          LoadOk () const;
  inline void          SetLoadOk (bool b);
  };

//---------------------------------------------------------------------------

inline ct_CppScanner * ct_CppFileStackEntry::GetScanner ()
  {
  return & co_Scanner;
  }

//---------------------------------------------------------------------------

inline ct_CppFileSemantic * ct_CppFileStackEntry::GetFileSemantic () const
  {
  return pco_FileSemantic;
  }

//---------------------------------------------------------------------------

inline void ct_CppFileStackEntry::SetFileSemantic
(
ct_CppFileSemantic * pco_fsem
)
  {
  pco_FileSemantic = pco_fsem;
  }

//---------------------------------------------------------------------------

inline et_CppFileStackStates ct_CppFileStackEntry::GetState () const
  {
  return eo_State;
  }

//---------------------------------------------------------------------------

inline void ct_CppFileStackEntry::SetState
(
et_CppFileStackStates eo_state
)
  {
  eo_State = eo_state;
  }

//---------------------------------------------------------------------------

inline bool ct_CppFileStackEntry::LoadOk () const
  {
  return b_LoadOk;
  }

//---------------------------------------------------------------------------

inline void ct_CppFileStackEntry::SetLoadOk
(
bool b
)
  {
  b_LoadOk = b;
  }

#endif
