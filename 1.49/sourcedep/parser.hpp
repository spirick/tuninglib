
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
// File sourcedep/parser.hpp

#ifndef SOURCEDEP_PARSER_HPP
#define SOURCEDEP_PARSER_HPP

#include "tuning/chn/array.h"
#include "tuning/filename.hpp"
#include "sourcedep/filstken.hpp"

//---------------------------------------------------------------------------

enum et_Messages
  {
  ec_CMsgRecIncludeIgnored,
  ec_CMsgSelfIncludeIgnored,

  ec_CMsgIncludeFileNotFound,
  ec_CMsgPPSyntaxError,

  ec_CMsgCannotOpenFile
  };

//---------------------------------------------------------------------------

class TL_EXPORT ct_CppProject;

class TL_EXPORT ct_CppParser
  {
  typedef gct_Chn32Array <ct_CppFileStackEntry> t_CppFileStack;

  ct_CppProject *      pco_Project;
  t_CppFileStack       o_FileStack;
  unsigned             u_FilesToParse;
  t_Token              o_Token;

  void                 Message (et_Messages eo_msg, const char * pc_str = "");
  void                 PushFile (ct_CppFileSemantic * pco_fileSemantic);
  void                 PopFile ();
  inline t_Token       ScanToken ();
  t_Token              ScanPPToken ();
  bool                 TestText (t_FileMapPtr * po_fmptr,
                         const ct_FileName * pco_fileName,
                         const char * pc_path, t_UInt u_pathLen, bool b_rel);
  bool                 SearchSrcFile (t_FileMapPtr * po_fmptr,
                         const ct_FileName * pco_fileName);
  bool                 SearchStdFile (t_FileMapPtr * po_fmptr,
                         const ct_FileName * pco_fileName);
  void                 PPDirective ();
  t_Token              NextToken ();

public:
                       ct_CppParser ();

  void                 Init (ct_CppProject * pco_project);
  bool                 Reparse ();
  };

#endif
