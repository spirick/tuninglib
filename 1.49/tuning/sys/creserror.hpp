
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
// File tuning/sys/creserror.hpp

#ifndef TUNING_SYS_CRESERROR_HPP
#define TUNING_SYS_CRESERROR_HPP

#include "tuning/defs.hpp"

//---------------------------------------------------------------------------
// Shared resource error codes

enum et_ResError
  {
  ec_ResOK = 0,
  ec_ResUnknownError,
  ec_ResUninitialized,
  ec_ResAlreadyInitialized,
  ec_ResInvalidKey,
  ec_ResInvalidValue,
  ec_ResNoKey,
  ec_ResAlreadyExists,
  ec_ResAccessDenied,
  ec_ResNotFound,
  ec_ResLockCountMismatch,
  ec_ResLockFailed,
  ec_ResUnlockFailed,
  ec_ResMemMapFailed,
  ec_ResUnmapFailed,
  ec_ResQuerySizeFailed
  };

#endif
