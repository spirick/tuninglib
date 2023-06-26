
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
// File tuning/std/packstore.h

#ifndef TUNING_STD_PACKSTORE_H
#define TUNING_STD_PACKSTORE_H

#include "tuning/std/store.hpp"
#include "tuning/packstore.h"

class ct_Std_PackStore:
  public gct_PackStore <ct_Std_Store> { };

#endif
