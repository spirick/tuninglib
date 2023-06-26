
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
// File tuning/dir.hpp

#ifndef TUNING_DIR_HPP
#define TUNING_DIR_HPP

#include "tuning/filename.hpp"

//---------------------------------------------------------------------------
// Only the drive and path components of ct_FileName are used

class TL_EXPORT ct_Directory: public ct_FileName
  {
public:
                       ct_Directory () { }
                       ct_Directory (const char * pc_init);
                       ct_Directory (const ct_FileName & co_init);
  ct_Directory &       operator = (const char * pc_asgn);
  ct_Directory &       operator = (const ct_FileName & co_asgn);

  TL_CLASSID (ct_Directory)

  bool                 QueryCurrentDrive ();
  bool                 QueryCurrentDirectory ();
  bool                 QueryCurrentDriveDirectory ();

  bool                 Create ();
  bool                 Exists ();
  bool                 Move (const char * pc_new);
  bool                 Delete ();
  };

#endif
