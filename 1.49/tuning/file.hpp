
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
// File tuning/file.hpp

#ifndef TUNING_FILE_HPP
#define TUNING_FILE_HPP

#include "tuning/sys/cfile.hpp"
#include "tuning/filename.hpp"

//---------------------------------------------------------------------------

class TL_EXPORT ct_File: public ct_FileName
  {
  t_FileId             o_FileId;

  // Do not copy this object
                       ct_File (const ct_File &);
  ct_File &            operator = (const ct_File &);

public:
                       ct_File ();
                       ct_File (const char * pc_init);
                       ct_File (const ct_FileName & co_init);
                       ~ct_File ();
  ct_File &            operator = (const char * pc_asgn);
  ct_File &            operator = (const ct_FileName & co_asgn);

  TL_CLASSID (ct_File)

  bool                 TryOpen (bool b_readOnly = true, bool b_sequential = true,
                         t_UInt32 u_milliSec = 0);
  bool                 Open (bool b_readOnly = true, bool b_sequential = true);
  bool                 Create (bool b_createNew = false);
  bool                 Close ();

  bool                 Load (ct_String * pco_str);
  bool                 Save (const ct_String * pco_str);

  bool                 Exists ();
  bool                 Move (const char * pc_new);
  bool                 Copy (const char * pc_new, bool b_overwrite = true);
  bool                 Delete ();

  // Services for opened files
  bool                 QuerySize (t_FileSize & o_size) const;
  bool                 QueryPos (t_FileSize & o_pos) const;
  bool                 EndOfFile (bool & b_eof) const;
  bool                 SeekAbs (t_FileSize o_pos) const;
  bool                 SeekRel (t_FileSize o_pos) const;
  bool                 Truncate (t_FileSize o_size) const;
  bool                 Read (void * pv_dst, t_FileSize o_len) const;
  bool                 Write (const void * pv_src, t_FileSize o_len) const;
  };

#endif
