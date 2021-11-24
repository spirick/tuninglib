
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
// File tuning/wstring.hpp

#ifndef TUNING_WSTRING_HPP
#define TUNING_WSTRING_HPP

#include "tuning/chn/store.hpp"
#include "tuning/nulldatablock.h"
#include "tuning/miniblock.h"
#include "tuning/string.h"

typedef WSTRING_BASE (gct_ObjectBaseMiniBlock, ct_Chn_) ct_Chn_WObjString;
OBJ_STRING_DCL(ct_Chn_WObj)
typedef ct_Chn_WObjectString ct_WString;

#endif
