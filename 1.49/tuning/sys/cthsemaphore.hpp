
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
// File tuning/sys/cthsemaphore.hpp

#ifndef TUNING_SYS_CTHSEMAPHORE_HPP
#define TUNING_SYS_CTHSEMAPHORE_HPP

#include "tuning/sys/creserror.hpp"

//---------------------------------------------------------------------------
// Thread semaphore

const unsigned       cu_ThSemaphoreDataSize = 96; // Multiple of sizeof (t_UInt64)

class TL_EXPORT ct_ThSemaphore
  {

#if defined TL_MULTI

private:
  // Don't use char buffer because of alignment
  t_UInt64             au_Data [cu_ThSemaphoreDataSize / sizeof (t_UInt64)];
  bool                 b_InitSuccess;

  // Do not copy this object
                       ct_ThSemaphore (const ct_ThSemaphore &);
  ct_ThSemaphore &     operator = (const ct_ThSemaphore &);

public:
                       ct_ThSemaphore (t_Int32 i_initValue = 1);
                       ~ct_ThSemaphore ();

  bool                 GetInitSuccess () const;
  et_ResError          TryAcquire (bool & b_success, t_UInt32 u_milliSec = 0);
  et_ResError          Acquire ();
  et_ResError          Release ();

#else

public:
                       ct_ThSemaphore (t_Int32 = 1) { }

  bool                 GetInitSuccess () const { return true; }
  et_ResError          TryAcquire (bool & b_success, t_UInt32 = 0)
                         { b_success = true; return ec_ResOK; }
  et_ResError          Acquire () { return ec_ResOK; }
  et_ResError          Release () { return ec_ResOK; }

#endif

  };

#endif
