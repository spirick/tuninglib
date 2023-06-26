
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
// File sourcedep/scanner.hpp

#ifndef SOURCEDEP_SCANNER_HPP
#define SOURCEDEP_SCANNER_HPP

#include "sourcedep/token.hpp"
#include "sourcedep/globtyps.hpp"

//---------------------------------------------------------------------------

enum et_CppScanLangs
  {
  ec_LangCpp,
  ec_LangPrep,
  ec_LangPrepBegin,
  ec_LangInclude,
  ec_LangSkipPrep
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_FileMap;

class TL_EXPORT ct_CppScanner
  {
  et_CppScanLangs      eo_Language;
  et_CppScanLangs      eo_OldLanguage;
  const char *         pc_Begin;
  const char *         pc_End;
  const char *         pc_Scan;
  unsigned             u_LineToken;
  const char *         pc_Token;

  inline bool          InText ();
  inline bool          EndOfText ();
  inline const char *  FirstChar (char c);
  t_Token              ScanAsm ();
  inline t_Token       ScanCComment ();
  inline t_Token       ScanCppComment ();
  inline void          TestUL ();
  t_Token              ScanFloat ();
  inline t_Token       ScanNull ();
  inline t_Token       ScanNumber ();
  t_Token              ScanString (char c_end, t_Token o_ret);
  t_Token              ScanIdf ();
  inline t_Token       PrepEnd ();

public:
  void                 Initialize (const char * pc_begin,
                         const char * pc_end);

  t_Token              ScanToken ();
  inline t_TextSize    GetTokenLength () const;
  inline const char *  GetTokenString () const;
  };

//---------------------------------------------------------------------------

inline t_TextSize ct_CppScanner::GetTokenLength () const
  {
  return (t_TextSize) (pc_Scan - pc_Token);
  }

//---------------------------------------------------------------------------

inline const char * ct_CppScanner::GetTokenString () const
  {
  return pc_Token;
  }

#endif
