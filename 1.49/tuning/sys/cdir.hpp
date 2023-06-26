
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
// File tuning/sys/cdir.hpp

#ifndef TUNING_SYS_CDIR_HPP
#define TUNING_SYS_CDIR_HPP

#include "tuning/string.hpp"

//---------------------------------------------------------------------------

bool TL_EXPORT       tl_QueryCurrentDirectory (const char * pc_drive,
                       t_UInt u_driveLen, ct_String & co_currentDirectory);
bool TL_EXPORT       tl_CreateDirectory (const char * pc_name);
bool TL_EXPORT       tl_MoveDirectory (const char * pc_old,
                       const char * pc_new);
bool TL_EXPORT       tl_DeleteDirectory (const char * pc_name);

#endif
