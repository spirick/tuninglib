
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
// File tuning/rnd/packstore.h

#ifndef TUNING_RND_PACKSTORE_H
#define TUNING_RND_PACKSTORE_H

#include "tuning/rnd/store.hpp"
#include "tuning/packstore.h"

class ct_Rnd_PackStore:
  public gct_PackStore <ct_Rnd_Store> { };

#endif
