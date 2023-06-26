
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
// File tuning/string.hpp

#ifndef TUNING_STRING_HPP
#define TUNING_STRING_HPP

#include "tuning/chn/store.hpp"
#include "tuning/nulldatablock.h"
#include "tuning/miniblock.h"
#include "tuning/string.h"

typedef STRING_BASE (gct_ObjectBaseMiniBlock, ct_Chn_) ct_Chn_ObjString;
OBJ_STRING_DCL(ct_Chn_Obj)
typedef ct_Chn_ObjectString ct_String;

#endif
